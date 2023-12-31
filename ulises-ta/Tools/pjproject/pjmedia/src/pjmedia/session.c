/* $Id: session.c 2394 2008-12-23 17:27:53Z bennylp $ */
/* 
 * Copyright (C) 2008-2009 Teluu Inc. (http://www.teluu.com)
 * Copyright (C) 2003-2008 Benny Prijono <benny@prijono.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */
#include <pjmedia/session.h>
#include <pjmedia/errno.h>
#include <pj/log.h>
#include <pj/os.h> 
#include <pj/pool.h>
#include <pj/string.h>
#include <pj/assert.h>
#include <pj/ctype.h>
#include <pj/rand.h>


struct pjmedia_session
{
    pj_pool_t		   *pool;
    pjmedia_endpt	   *endpt;
    unsigned		    stream_cnt;
    pjmedia_stream_info	    stream_info[PJMEDIA_MAX_SDP_MEDIA];
    pjmedia_stream	   *stream[PJMEDIA_MAX_SDP_MEDIA];
    void		   *user_data;
};

#define THIS_FILE		"session.c"

#ifndef PJMEDIA_SESSION_SIZE
#   define PJMEDIA_SESSION_SIZE	(10*1024)
#endif

#ifndef PJMEDIA_SESSION_INC
#   define PJMEDIA_SESSION_INC	1024
#endif


static const pj_str_t ID_AUDIO = { "audio", 5};
static const pj_str_t ID_VIDEO = { "video", 5};
static const pj_str_t ID_APPLICATION = { "application", 11};
static const pj_str_t ID_IN = { "IN", 2 };
static const pj_str_t ID_IP4 = { "IP4", 3};
static const pj_str_t ID_IP6 = { "IP6", 3};
static const pj_str_t ID_RTP_AVP = { "RTP/AVP", 7 };
static const pj_str_t ID_RTP_SAVP = { "RTP/SAVP", 8 };
//static const pj_str_t ID_SDP_NAME = { "pjmedia", 7 };
static const pj_str_t ID_RTPMAP = { "rtpmap", 6 };
static const pj_str_t ID_TELEPHONE_EVENT = { "telephone-event", 15 };

static const pj_str_t STR_INACTIVE = { "inactive", 8 };
static const pj_str_t STR_SENDRECV = { "sendrecv", 8 };
static const pj_str_t STR_SENDONLY = { "sendonly", 8 };
static const pj_str_t STR_RECVONLY = { "recvonly", 8 };


/*
 * Parse fmtp for specified format/payload type.
 */
static void parse_fmtp( pj_pool_t *pool,
			const pjmedia_sdp_media *m,
			unsigned pt,
			pjmedia_codec_fmtp *fmtp)
{
    const pjmedia_sdp_attr *attr;
    pjmedia_sdp_fmtp sdp_fmtp;
    char *p, *p_end, fmt_buf[8];
    pj_str_t fmt;

    pj_assert(m && fmtp);

    pj_bzero(fmtp, sizeof(pjmedia_codec_fmtp));

    /* Get "fmtp" attribute for the format */
    pj_ansi_sprintf(fmt_buf, "%d", pt);
    fmt = pj_str(fmt_buf);
    attr = pjmedia_sdp_media_find_attr2(m, "fmtp", &fmt);
    if (attr == NULL)
	return;

    /* Parse "fmtp" attribute */
    if (pjmedia_sdp_attr_get_fmtp(attr, &sdp_fmtp) != PJ_SUCCESS)
	return;

    /* Prepare parsing */
    p = sdp_fmtp.fmt_param.ptr;
    p_end = p + sdp_fmtp.fmt_param.slen;

    /* Parse */
    while (p < p_end) {
	char *token, *start, *end;

	/* Skip whitespaces */
	while (p < p_end && (*p == ' ' || *p == '\t')) ++p;
	if (p == p_end)
	    break;

	/* Get token */
	start = p;
	while (p < p_end && *p != ';' && *p != '=') ++p;
	end = p - 1;

	/* Right trim */
	while (end >= start && (*end == ' '  || *end == '\t' || 
				*end == '\r' || *end == '\n' ))
	    --end;

	/* Forward a char after trimming */
	++end;

	/* Store token */
	if (end > start) {
	    token = (char*)pj_pool_alloc(pool, end - start);
	    pj_ansi_strncpy(token, start, end - start);
	    if (*p == '=')
		/* Got param name */
		pj_strset(&fmtp->param[fmtp->cnt].name, token, end - start);
	    else
		/* Got param value */
		pj_strset(&fmtp->param[fmtp->cnt++].val, token, end - start);
	} else if (*p != '=') {
	    ++fmtp->cnt;
	}

	/* Next */
	++p;
    }
}


/*
 * Create stream info from SDP media line.
 */
PJ_DEF(pj_status_t) pjmedia_stream_info_from_sdp(
					   pjmedia_stream_info *si,
					   pj_pool_t *pool,
					   pjmedia_endpt *endpt,
					   const pjmedia_sdp_session *local,
					   const pjmedia_sdp_session *remote,
					   unsigned stream_idx)
{    
	pjmedia_codec_mgr *mgr;
    const pjmedia_sdp_attr *attr;
    pjmedia_sdp_media *local_m;
	pj_bool_t local_m_has_123_codec = PJ_FALSE;
	unsigned fmti_index_123_codec_is = 0;
    const pjmedia_sdp_media *rem_m;
    const pjmedia_sdp_conn *local_conn;
    const pjmedia_sdp_conn *rem_conn;
    int rem_af, local_af;
    pj_sockaddr local_addr;
    pjmedia_sdp_rtpmap *rtpmap;
    unsigned i, pt, fmti;
    pj_status_t status;
	const pjmedia_sdp_attr *attr_pttid;
    
    /* Validate arguments: */
    PJ_ASSERT_RETURN(pool && si && local && remote, PJ_EINVAL);
    PJ_ASSERT_RETURN(stream_idx < local->media_count, PJ_EINVAL);
    PJ_ASSERT_RETURN(stream_idx < remote->media_count, PJ_EINVAL);


	/* Get codec manager. */
    mgr = pjmedia_endpt_get_codec_mgr(endpt);

    /* Keep SDP shortcuts */
    local_m = local->media[stream_idx];
    rem_m = remote->media[stream_idx];

    local_conn = local_m->conn ? local_m->conn : local->conn;
    if (local_conn == NULL)
		return PJMEDIA_SDP_EMISSINGCONN;

    rem_conn = rem_m->conn ? rem_m->conn : remote->conn;
    if (rem_conn == NULL)
		return PJMEDIA_SDP_EMISSINGCONN;

    /* Reset: */
    pj_bzero(si, sizeof(*si));

#if PJMEDIA_HAS_RTCP_XR && PJMEDIA_STREAM_ENABLE_XR
    /* Set default RTCP XR enabled/disabled */
    si->rtcp_xr_enabled = PJ_TRUE;
#endif

    /* Media type: */

    if (pj_stricmp(&local_m->desc.media, &ID_AUDIO) == 0) 
	{
		si->type = PJMEDIA_TYPE_AUDIO;
    } 
	else if (pj_stricmp(&local_m->desc.media, &ID_VIDEO) == 0) 
	{
	si->type = PJMEDIA_TYPE_VIDEO;
    } 
	else 
	{
	si->type = PJMEDIA_TYPE_UNKNOWN;

	/* Avoid rejecting call because of unrecognized media, 
	 * just return PJ_SUCCESS, this media will be deactivated later.
	 */
	//return PJMEDIA_EINVALIMEDIATYPE;
	return PJ_SUCCESS;
    }

    /* Transport protocol */

    /* At this point, transport type must be compatible, 
     * the transport instance will do more validation later.
     */
    status = pjmedia_sdp_transport_cmp(&rem_m->desc.transport, 
				       &local_m->desc.transport);
    if (status != PJ_SUCCESS)
		return PJMEDIA_SDPNEG_EINVANSTP;


	if (pj_stricmp(&local_m->desc.transport, &ID_RTP_AVP) == 0) 
	{
		si->proto = PJMEDIA_TP_PROTO_RTP_AVP;
    } 
	else if (pj_stricmp(&local_m->desc.transport, &ID_RTP_SAVP) == 0) 
	{
		si->proto = PJMEDIA_TP_PROTO_RTP_SAVP;
    } 
	else 
	{
		si->proto = PJMEDIA_TP_PROTO_UNKNOWN;
		return PJ_SUCCESS;
    }

    /* Check address family in remote SDP */
    rem_af = pj_AF_UNSPEC();
    if (pj_stricmp(&rem_conn->net_type, &ID_IN)==0) 
	{
		if (pj_stricmp(&rem_conn->addr_type, &ID_IP4)==0) 
		{
			rem_af = pj_AF_INET();
		} 
		else if (pj_stricmp(&rem_conn->addr_type, &ID_IP6)==0) 
		{
			rem_af = pj_AF_INET6();
		}
    }

    if (rem_af==pj_AF_UNSPEC()) 
	{
		/* Unsupported address family */
		return PJ_EAFNOTSUP;
    }

    /* Set remote address: */
    status = pj_sockaddr_init(rem_af, &si->rem_addr, &rem_conn->addr, 
			      rem_m->desc.port);
    if (status != PJ_SUCCESS) 
	{
		/* Invalid IP address. */
		return PJMEDIA_EINVALIDIP;
    }

    /* Check address family of local info */
    local_af = pj_AF_UNSPEC();
    if (pj_stricmp(&local_conn->net_type, &ID_IN)==0) 
	{
		if (pj_stricmp(&local_conn->addr_type, &ID_IP4)==0) 
		{
			local_af = pj_AF_INET();
		} 
		else if (pj_stricmp(&local_conn->addr_type, &ID_IP6)==0) 
		{
			local_af = pj_AF_INET6();
		}
    }

    if (local_af==pj_AF_UNSPEC()) 
	{
		/* Unsupported address family */
		return PJ_SUCCESS;
    }

    /* Set remote address: */
    status = pj_sockaddr_init(local_af, &local_addr, &local_conn->addr, 
			      local_m->desc.port);
    if (status != PJ_SUCCESS) 
	{
		/* Invalid IP address. */
		return PJMEDIA_EINVALIDIP;
    }

    /* Local and remote address family must match */
    if (local_af != rem_af)
		return PJ_EAFNOTSUP;

    /* Media direction: */

    if (local_m->desc.port == 0 || 
	pj_sockaddr_has_addr(&local_addr)==PJ_FALSE ||
	pj_sockaddr_has_addr(&si->rem_addr)==PJ_FALSE ||
	pjmedia_sdp_media_find_attr(local_m, &STR_INACTIVE, NULL)!=NULL)
    {
		/* Inactive stream. */
		si->dir = PJMEDIA_DIR_NONE;
    } 
	else if (pjmedia_sdp_media_find_attr(local_m, &STR_SENDONLY, NULL)!=NULL) 
	{
		/* Send only stream. */
		si->dir = PJMEDIA_DIR_ENCODING;
    } 
	else if (pjmedia_sdp_media_find_attr(local_m, &STR_RECVONLY, NULL)!=NULL) 
	{
		/* Recv only stream. */
		si->dir = PJMEDIA_DIR_DECODING;
    } 
	else 
	{
		/* Send and receive stream. */
		si->dir = PJMEDIA_DIR_ENCODING_DECODING;
    }

    /* No need to do anything else if stream is rejected */
    if (local_m->desc.port == 0) 
	{
		return PJ_SUCCESS;
    }

    /* If "rtcp" attribute is present in the SDP, set the RTCP address
     * from that attribute. Otherwise, calculate from RTP address.
     */
    attr = pjmedia_sdp_attr_find2(rem_m->attr_count, rem_m->attr,
				  "rtcp", NULL);
    if (attr) 
	{
		pjmedia_sdp_rtcp_attr rtcp;
		status = pjmedia_sdp_attr_get_rtcp(attr, &rtcp);
		if (status == PJ_SUCCESS) 
		{
			if (rtcp.addr.slen) 
			{
				status = pj_sockaddr_init(rem_af, &si->rem_rtcp, &rtcp.addr,
							  (pj_uint16_t)rtcp.port);
			} 
			else 
			{
				pj_sockaddr_init(rem_af, &si->rem_rtcp, NULL, 
						 (pj_uint16_t)rtcp.port);
				pj_memcpy(pj_sockaddr_get_addr(&si->rem_rtcp),
						  pj_sockaddr_get_addr(&si->rem_addr),
					  pj_sockaddr_get_addr_len(&si->rem_addr));
			}
		}
    }
    
    if (!pj_sockaddr_has_addr(&si->rem_rtcp)) 
	{
		int rtcp_port;

		pj_memcpy(&si->rem_rtcp, &si->rem_addr, sizeof(pj_sockaddr));
		rtcp_port = pj_sockaddr_get_port(&si->rem_addr) + 1;
		pj_sockaddr_set_port(&si->rem_rtcp, (pj_uint16_t)rtcp_port);
    }


    /* Get the payload number for receive channel. */
    /*
       Previously we used to rely on fmt[0] being the selected codec,
       but some UA sends telephone-event as fmt[0] and this would
       cause assert failure below.

       Thanks Chris Hamilton <chamilton .at. cs.dal.ca> for this patch.

    // And codec must be numeric!
    if (!pj_isdigit(*local_m->desc.fmt[0].ptr) || 
	!pj_isdigit(*rem_m->desc.fmt[0].ptr))
    {
	return PJMEDIA_EINVALIDPT;
    }

    pt = pj_strtoul(&local_m->desc.fmt[0]);
    pj_assert(PJMEDIA_RTP_PT_TELEPHONE_EVENTS==0 ||
	      pt != PJMEDIA_RTP_PT_TELEPHONE_EVENTS);
    */

    /* This is to suppress MSVC warning about uninitialized var */
    pt = 0;

    /* Find the first codec which is not telephone-event */
	/* local_m contiene los codecs locales que coinciden con los del remoto*/
	/*Buscamos el primero que no sea telephone-event y que no sea el 123*/
	/*Si no lo encontramos entonces nos quedaremos con el 123*/
    for ( fmti = 0; fmti < local_m->desc.fmt_count; ++fmti ) 
	{
		if (!pj_isdigit(*local_m->desc.fmt[fmti].ptr))
		{
			return PJMEDIA_EINVALIDPT;
		}

		pt = pj_strtoul(&local_m->desc.fmt[fmti]);
		
		if (PJMEDIA_RTP_PT_TELEPHONE_EVENTS == 0 || pt != PJMEDIA_RTP_PT_TELEPHONE_EVENTS)
		{
			if (pt == 123)
			{
				local_m_has_123_codec = PJ_TRUE;
				fmti_index_123_codec_is = fmti;
				continue;
			}
			break;
		}
    }

	if (fmti >= local_m->desc.fmt_count)
	{
		if (local_m_has_123_codec)
		{
			//El unico codec que se ha encontrado que no es del tipo telephone-event es el 123
			//Es con el que nos quedaremos
			pt = 123;
			fmti = fmti_index_123_codec_is;
		}
		else
		{
			return PJMEDIA_EINVALIDPT;
		}
	}

    /* Get codec info.
     * For static payload types, get the info from codec manager.
     * For dynamic payload types, MUST get the rtpmap.
     */
    if (pt < 96 || pt == 123) 
	{
		pj_bool_t has_rtpmap;

		rtpmap = NULL;
		has_rtpmap = PJ_TRUE;

		attr = pjmedia_sdp_media_find_attr(local_m, &ID_RTPMAP, 
						   &local_m->desc.fmt[fmti]);
		if (attr == NULL) 
		{
			has_rtpmap = PJ_FALSE;
		}
		if (attr != NULL) 
		{
			status = pjmedia_sdp_attr_to_rtpmap(pool, attr, &rtpmap);
			if (status != PJ_SUCCESS)
			has_rtpmap = PJ_FALSE;
		}

		/* Build codec format info: */
		if (has_rtpmap) 
		{
			si->fmt.type = si->type;
			si->fmt.pt = pj_strtoul(&local_m->desc.fmt[fmti]);
			pj_strdup(pool, &si->fmt.encoding_name, &rtpmap->enc_name);
			si->fmt.clock_rate = rtpmap->clock_rate;
	    
	#if defined(PJMEDIA_HANDLE_G722_MPEG_BUG) && (PJMEDIA_HANDLE_G722_MPEG_BUG != 0)
			/* The session info should have the actual clock rate, because 
			 * this info is used for calculationg buffer size, etc in stream 
			 */
			if (si->fmt.pt == PJMEDIA_RTP_PT_G722)
				si->fmt.clock_rate = 16000;
	#endif

			/* For audio codecs, rtpmap parameters denotes the number of
			 * channels.
			 */
			if (si->type == PJMEDIA_TYPE_AUDIO && rtpmap->param.slen) 
			{
				si->fmt.channel_cnt = (unsigned) pj_strtoul(&rtpmap->param);
			} 
			else 
			{
				si->fmt.channel_cnt = 1;
			}
		} 
		else 
		{	    
			const pjmedia_codec_info *p_info;

			status = pjmedia_codec_mgr_get_codec_info( mgr, pt, &p_info);
			if (status != PJ_SUCCESS)
				return status;

			pj_memcpy(&si->fmt, p_info, sizeof(pjmedia_codec_info));
		}

		/* For static payload type, pt's are symetric */
		si->tx_pt = pt;
    } 
	else 
	{
		attr = pjmedia_sdp_media_find_attr(local_m, &ID_RTPMAP, 
						   &local_m->desc.fmt[fmti]);
		if (attr == NULL)
			return PJMEDIA_EMISSINGRTPMAP;

		status = pjmedia_sdp_attr_to_rtpmap(pool, attr, &rtpmap);
		if (status != PJ_SUCCESS)
			return status;

		/* Build codec format info: */

		si->fmt.type = si->type;
		si->fmt.pt = pj_strtoul(&local_m->desc.fmt[fmti]);
		pj_strdup(pool, &si->fmt.encoding_name, &rtpmap->enc_name);
		si->fmt.clock_rate = rtpmap->clock_rate;

		/* For audio codecs, rtpmap parameters denotes the number of
		 * channels.
		 */
		if (si->type == PJMEDIA_TYPE_AUDIO && rtpmap->param.slen) 
		{
			si->fmt.channel_cnt = (unsigned) pj_strtoul(&rtpmap->param);
		} 
		else 
		{
			si->fmt.channel_cnt = 1;
		}

		/* Determine payload type for outgoing channel, by finding
		 * dynamic payload type in remote SDP that matches the answer.
		 */
		si->tx_pt = 0xFFFF;
		for (i=0; i<rem_m->desc.fmt_count; ++i) 
		{
			unsigned rpt;
			pjmedia_sdp_attr *r_attr;
			pjmedia_sdp_rtpmap r_rtpmap;

			rpt = pj_strtoul(&rem_m->desc.fmt[i]);
			if (rpt < 96)
				continue;

			r_attr = pjmedia_sdp_media_find_attr(rem_m, &ID_RTPMAP,
							 &rem_m->desc.fmt[i]);
			if (!r_attr)
				continue;

			if (pjmedia_sdp_attr_get_rtpmap(r_attr, &r_rtpmap) != PJ_SUCCESS)
				continue;

			if (!pj_stricmp(&rtpmap->enc_name, &r_rtpmap.enc_name) &&
				rtpmap->clock_rate == r_rtpmap.clock_rate)
			{
				/* Found matched codec. */
				si->tx_pt = rpt;
				break;
			}
		}

		if (si->tx_pt == 0xFFFF)
		    return PJMEDIA_EMISSINGRTPMAP;
    }
  
    /* Now that we have codec info, get the codec param. */
    si->param = PJ_POOL_ALLOC_T(pool, pjmedia_codec_param);
    status = pjmedia_codec_mgr_get_default_param(mgr, &si->fmt, si->param);

    /* Get remote fmtp for our encoder. */
    parse_fmtp(pool, rem_m, si->tx_pt, &si->param->setting.enc_fmtp);

    /* Get local fmtp for our decoder. */
    parse_fmtp(pool, local_m, si->fmt.pt, &si->param->setting.dec_fmtp);

    /* Get remote maxptime for our encoder. */
    attr = pjmedia_sdp_attr_find2(rem_m->attr_count, rem_m->attr, "maxptime", NULL);
    if (attr) 
	{
		pj_str_t tmp_val = attr->value;

		pj_strltrim(&tmp_val);
		si->tx_maxptime = pj_strtoul(&tmp_val);
    }

    /* When direction is NONE (it means SDP negotiation has failed) we don't
     * need to return a failure here, as returning failure will cause
     * the whole SDP to be rejected. See ticket #:
     *	http://
     *
     * Thanks Alain Totouom 
     */
    if (status != PJ_SUCCESS && si->dir != PJMEDIA_DIR_NONE)
		return status;

    /* Get incomming payload type for telephone-events */
    si->rx_event_pt = -1;
    for (i=0; i<local_m->attr_count; ++i) 
	{
		pjmedia_sdp_rtpmap r;

		attr = local_m->attr[i];
		if (pj_strcmp(&attr->name, &ID_RTPMAP) != 0)
			continue;
		if (pjmedia_sdp_attr_get_rtpmap(attr, &r) != PJ_SUCCESS)
			continue;
		if (pj_strcmp(&r.enc_name, &ID_TELEPHONE_EVENT) == 0) 
		{
			si->rx_event_pt = pj_strtoul(&r.pt);
			break;
		}
    }

    /* Get outgoing payload type for telephone-events */
    si->tx_event_pt = -1;
    for (i=0; i<rem_m->attr_count; ++i) 
	{
		pjmedia_sdp_rtpmap r;

		attr = rem_m->attr[i];
		if (pj_strcmp(&attr->name, &ID_RTPMAP) != 0)
			continue;
		if (pjmedia_sdp_attr_get_rtpmap(attr, &r) != PJ_SUCCESS)
			continue;
		if (pj_strcmp(&r.enc_name, &ID_TELEPHONE_EVENT) == 0) 
		{
			si->tx_event_pt = pj_strtoul(&r.pt);
			break;
		}
    }

    /* Leave SSRC to random. */
    si->ssrc = pj_rand();

    /* Set default jitter buffer parameter. */
    si->jb_init = si->jb_max = si->jb_min_pre = si->jb_max_pre = -1;

	si->ka_type = PJMEDIA_STREAM_ENABLE_KA;	
	si->ka_remote_timeout = PJMEDIA_STREAM_KA_TIMEOUT;	
	si->ka_local_interval = PJMEDIA_STREAM_KA_INTERVAL;

	attr_pttid = pjmedia_sdp_attr_find2(rem_m->attr_count, rem_m->attr, "PTT-id", NULL);
	if (!attr_pttid)
	{
		attr_pttid = pjmedia_sdp_attr_find2(rem_m->attr_count, rem_m->attr, "ptt-id", NULL);
	}

	attr = pjmedia_sdp_attr_find2(local_m->attr_count, local_m->attr, "type", NULL);
    // PJ_LOG(4, (THIS_FILE, "Decodificando SDP...attr(type)%s", attr ? "existe" : "no existe"));

	if (attr) 
	{
		if ((pj_strnicmp2(&attr->value, "Radio",5) == 0) || 
			(pj_strnicmp2(&attr->value, "Coupling",8) == 0) ||
			(pj_strnicmp2(&attr->value, "Radio-Rxonly",12) == 0) || 
			(pj_strnicmp2(&attr->value, "Radio-TxRx",10) == 0) || 
			(pj_strnicmp2(&attr->value, "Radio-Idle",10) == 0) )
		{
			unsigned r2s_period, r2s_multiplier;

			attr = pjmedia_sdp_attr_find2(rem_m->attr_count, rem_m->attr, "R2S-KeepAlivePeriod", NULL);
			r2s_period = attr ? pj_strtoul(&attr->value) : PJMEDIA_DEFAULT_R2S_KA_PERIOD;
			attr = pjmedia_sdp_attr_find2(rem_m->attr_count, rem_m->attr, "R2S-KeepAliveMultiplier", NULL);
			r2s_multiplier = attr ? pj_strtoul(&attr->value) : PJMEDIA_DEFAULT_R2S_KA_MULTIPLIER;

			si->ka_type = PJMEDIA_STREAM_KA_R2S;
			si->ka_remote_timeout = r2s_multiplier * r2s_period;

			attr = pjmedia_sdp_attr_find2(local_m->attr_count, local_m->attr, "R2S-KeepAlivePeriod", NULL);
			si->ka_local_interval = attr ? pj_strtoul(&attr->value) : PJMEDIA_DEFAULT_R2S_KA_PERIOD;			

			if (!attr_pttid)
			{
				attr = pjmedia_sdp_attr_find2(local_m->attr_count, local_m->attr, "PTT-id", NULL);
				if (!attr)
				{
					attr = pjmedia_sdp_attr_find2(local_m->attr_count, local_m->attr, "ptt-id", NULL);
				}
				if (attr)
				{
					//Tomamos el pttid del valor local de ptt-id
					si->pttId = (unsigned short)(pj_strtoul(&attr->value));
				}
			}
			else
			{
				si->pttId = (unsigned short)(attr_pttid ? pj_strtoul(&attr_pttid->value) : 0);
			}
			si->rtp_ext_enabled = PJ_TRUE;			
		}
	}

	si->bss_method[0] = '\0';
	attr = pjmedia_sdp_media_find_attr2(rem_m, "bss", NULL);
	if (attr)
	{
		if (attr->value.slen < sizeof(si->bss_method))
		{
			strncpy(si->bss_method, attr->value.ptr, attr->value.slen);
			si->bss_method[attr->value.slen] = '\0';
		}
	}

    return PJ_SUCCESS;
}


/*
 * Initialize session info from SDP session descriptors.
 */
PJ_DEF(pj_status_t) pjmedia_session_info_from_sdp( pj_pool_t *pool,
			       pjmedia_endpt *endpt,
			       unsigned max_streams,
			       pjmedia_session_info *si,
			       const pjmedia_sdp_session *local,
			       const pjmedia_sdp_session *remote)
{
    unsigned i;

    PJ_ASSERT_RETURN(pool && endpt && si && local && remote, PJ_EINVAL);

    si->stream_cnt = max_streams;
    if (si->stream_cnt > local->media_count)
	si->stream_cnt = local->media_count;

    for (i=0; i<si->stream_cnt; ++i) {
	pj_status_t status;

	status = pjmedia_stream_info_from_sdp( &si->stream_info[i], pool,
					       endpt, 
					       local, remote, i);
	if (status != PJ_SUCCESS)
	{
		si->stream_cnt = 0;   
	    return status;
	}
    }

    return PJ_SUCCESS;
}

/**
 * Modify session.
 */
PJ_DEF(pj_status_t) pjmedia_radio_session_modify(pjmedia_session* session, const pjmedia_session_info* si)
{
	pj_status_t status = PJ_SUCCESS;

	if (session->stream_cnt != 1 || si->stream_cnt != 1)
	{
		return PJ_EBUG;
	}

	status = pjmedia_radio_stream_modify(session->stream[0], &si->stream_info[0]);
	return status;
}


/**
 * Create new session.
 */
PJ_DEF(pj_status_t) pjmedia_session_create( pjmedia_endpt *endpt, 
					    const pjmedia_session_info *si,
					    pjmedia_transport *transports[],
					    void *user_data,
					    pjmedia_session **p_session )
{
    pj_pool_t *pool;
    pjmedia_session *session = NULL;
    int i; /* Must be signed */
    pj_status_t status;

    /* Verify arguments. */
    PJ_ASSERT_RETURN(endpt && si && p_session, PJ_EINVAL);
	*p_session = NULL;

    /* Create pool for the session. */
    pool = pjmedia_endpt_create_pool( endpt, "session", 
				      PJMEDIA_SESSION_SIZE, 
				      PJMEDIA_SESSION_INC);
    PJ_ASSERT_RETURN(pool != NULL, PJ_ENOMEM);

    session = PJ_POOL_ZALLOC_T(pool, pjmedia_session);
    session->pool = pool;
    session->endpt = endpt;

	for (i = 0; i < PJMEDIA_MAX_SDP_MEDIA; i++)
	{
		session->stream[i] = NULL;
	}

    session->stream_cnt = si->stream_cnt;
    session->user_data = user_data;	

    /* Copy stream info (this simple memcpy may break sometime) */
    pj_memcpy(session->stream_info, si->stream_info,
	      si->stream_cnt * sizeof(pjmedia_stream_info));

    /*
     * Now create and start the stream!
     */
    for (i=0; i<(int)si->stream_cnt; ++i) {

	/* Create the stream */
	status = pjmedia_stream_create(endpt, session->pool,
				       &session->stream_info[i],
				       (transports?transports[i]:NULL),
				       session,
				       &session->stream[i]);
	if (status == PJ_SUCCESS)
	{
	    status = pjmedia_stream_start(session->stream[i]);
	}

	if (status != PJ_SUCCESS) {

	    //for ( --i; i>=0; ++i) {
		for ( --i; i>=0; --i) {
		pjmedia_stream_destroy(session->stream[i]);
		session->stream[i] = NULL;
	    }

	    pj_pool_release(session->pool);
	    return status;
	}
    }


    /* Done. */
    *p_session = session;
    return PJ_SUCCESS;
}


/*
 * Get session info.
 */
PJ_DEF(pj_status_t) pjmedia_session_get_info( pjmedia_session *session,
					      pjmedia_session_info *info )
{
    PJ_ASSERT_RETURN(session && info, PJ_EINVAL);

    info->stream_cnt = session->stream_cnt;
    pj_memcpy(info->stream_info, session->stream_info,
	      session->stream_cnt * sizeof(pjmedia_stream_info));

    return PJ_SUCCESS;
}

/*
 * Get user data.
 */
PJ_DEF(void*) pjmedia_session_get_user_data( pjmedia_session *session)
{
    return (session? session->user_data : NULL);
}

/**
 * Destroy media session.
 */
PJ_DEF(pj_status_t) pjmedia_session_destroy (pjmedia_session *session)
{
    unsigned i;
	unsigned int stream_cnt;

    PJ_ASSERT_RETURN(session, PJ_EINVAL);

	stream_cnt = session->stream_cnt;
	session->stream_cnt = 0;		

    for (i=0; i<stream_cnt; ++i) {
	
	pjmedia_stream_destroy(session->stream[i]);
	session->stream[i] = NULL;


    }

    pj_pool_release (session->pool);

    return PJ_SUCCESS;
}


/**
 * Activate all stream in media session.
 *
 */
PJ_DEF(pj_status_t) pjmedia_session_resume(pjmedia_session *session,
					   pjmedia_dir dir)
{
    unsigned i;

    PJ_ASSERT_RETURN(session, PJ_EINVAL);

    for (i=0; i<session->stream_cnt; ++i) {
	pjmedia_session_resume_stream(session, i, dir);
    }

    return PJ_SUCCESS;
}


/**
 * Suspend receipt and transmission of all stream in media session.
 *
 */
PJ_DEF(pj_status_t) pjmedia_session_pause(pjmedia_session *session,
					  pjmedia_dir dir)
{
    unsigned i;

    PJ_ASSERT_RETURN(session, PJ_EINVAL);

    for (i=0; i<session->stream_cnt; ++i) {
	pjmedia_session_pause_stream(session, i, dir);
    }

    return PJ_SUCCESS;
}


/**
 * Suspend receipt and transmission of individual stream in media session.
 */
PJ_DEF(pj_status_t) pjmedia_session_pause_stream( pjmedia_session *session,
						  unsigned index,
						  pjmedia_dir dir)
{
    PJ_ASSERT_RETURN(session && index < session->stream_cnt, PJ_EINVAL);

    return pjmedia_stream_pause(session->stream[index], dir);
}


/**
 * Activate individual stream in media session.
 *
 */
PJ_DEF(pj_status_t) pjmedia_session_resume_stream( pjmedia_session *session,
						   unsigned index,
						   pjmedia_dir dir)
{
    PJ_ASSERT_RETURN(session && index < session->stream_cnt, PJ_EINVAL);

    return pjmedia_stream_resume(session->stream[index], dir);
}

/**
 * Enumerate media stream in the session.
 */
PJ_DEF(pj_status_t) pjmedia_session_enum_streams(const pjmedia_session *session,
						 unsigned *count, 
						 pjmedia_stream_info info[])
{
    unsigned i;

    PJ_ASSERT_RETURN(session && count && *count && info, PJ_EINVAL);

    if (*count > session->stream_cnt)
	*count = session->stream_cnt;

    for (i=0; i<*count; ++i) {
	pj_memcpy(&info[i], &session->stream[i], sizeof(pjmedia_stream_info));
    }

    return PJ_SUCCESS;
}


/*
 * Get the port interface.
 */
PJ_DEF(pj_status_t) pjmedia_session_get_port(  pjmedia_session *session,
					       unsigned index,
					       pjmedia_port **p_port)
{
    return pjmedia_stream_get_port( session->stream[index], p_port);
}

/*
 * Get statistics
 */
PJ_DEF(pj_status_t) pjmedia_session_get_stream_stat( pjmedia_session *session,
						     unsigned index,
						     pjmedia_rtcp_stat *stat)
{
    PJ_ASSERT_RETURN(session && stat && index < session->stream_cnt, 
		     PJ_EINVAL);

    return pjmedia_stream_get_stat(session->stream[index], stat);
}


#if defined(PJMEDIA_HAS_RTCP_XR) && (PJMEDIA_HAS_RTCP_XR != 0)
/*
 * Get extended statistics
 */
PJ_DEF(pj_status_t) pjmedia_session_get_stream_stat_xr(
					     pjmedia_session *session,
					     unsigned index,
					     pjmedia_rtcp_xr_stat *stat_xr)
{
    PJ_ASSERT_RETURN(session && stat_xr && index < session->stream_cnt, 
		     PJ_EINVAL);

    return pjmedia_stream_get_stat_xr(session->stream[index], stat_xr);
}
#endif

PJ_DEF(pj_status_t) pjmedia_session_get_stream_stat_jbuf(
					    pjmedia_session *session,
					    unsigned index,
					    pjmedia_jb_state *state)
{
    PJ_ASSERT_RETURN(session && state && index < session->stream_cnt, PJ_EINVAL);
    return pjmedia_stream_get_stat_jbuf(session->stream[index], state);
}

/* NUCLEO-2010*/ 
PJ_DEF(pjmedia_stream*) pjmedia_session_get_stream( pjmedia_session *session,
	unsigned index)
{
	PJ_ASSERT_RETURN(session && index < session->stream_cnt, NULL);

	return session->stream[index];
}
/* FM */
/* NUCLEO-2014. */
/*-- 
		PJ_DEF(void) pjmedia_session_enable_ec( pjmedia_session *session,
																	  unsigned index)
		{
			pj_assert(session && index < session->stream_cnt);
			pjmedia_stream_enable_ec(session->pool, session->stream[index]);
		}
*/
PJ_DEF(void) pjmedia_session_enable_ec( pjmedia_session *session,
															  unsigned index, unsigned tail_ms, unsigned latency_ms, unsigned options)
{
	pj_assert(session && index < session->stream_cnt);
	pjmedia_stream_enable_ec(session->pool, session->stream[index], tail_ms, latency_ms, options);
}

/* FM */

/*
 *	Actualiza los parametros del grupo climax en todos los streams de una sesion
 */
PJ_DEF(void) pjmedia_session_set_climax_param( pjmedia_session *session, pj_bool_t NTP_synchronized)
{
	unsigned int i;
	
	if (session == NULL) return;
	if (session->stream_cnt > 2) return;

	for (i = 0; i < session->stream_cnt; i++)
	{
		if (session->stream[i] != NULL)
		{
			if (session->stream_info[i].type > PJMEDIA_TYPE_NONE && session->stream_info[i].type <= PJMEDIA_TYPE_APPLICATION)
			{
				pjmedia_stream_set_climax_param(session->stream[i], NTP_synchronized);
			}
			else 
			{
			}
		}
	}
}

PJ_DEF(void) pjmedia_session_set_request_MAM(pjmedia_session *session, pjmedia_stream * stream)
{
	if (session == NULL || stream == NULL) return;

	if (session->pool != NULL && stream != NULL)
		pjmedia_stream_set_request_MAM(stream);
}

PJ_DEF(void) pjmedia_session_force_request_MAM(pjmedia_session *session)
{
	unsigned int i;

	if (session == NULL) return;
	if (session->stream_cnt > 2) return;

	for (i = 0; i < session->stream_cnt; i++)
	{
		if (session->pool != NULL && session->stream[i] != NULL)
		{
			if (session->stream_info[i].type > PJMEDIA_TYPE_NONE && session->stream_info[i].type <= PJMEDIA_TYPE_APPLICATION)
			{
				pjmedia_stream_set_request_MAM(session->stream[i]);
			}
			else 
			{
			}
		}
	}
}

PJ_DEF(void) pjmedia_session_force_send_KA_packet(pjmedia_session *session)
{
	unsigned int i;

	if (session == NULL) return;
	if (session->stream_cnt > 2) return;

	for (i = 0; i < session->stream_cnt; i++)
	{
		if (session->pool != NULL && session->stream[i] != NULL)
		{
			if (session->stream_info[i].type > PJMEDIA_TYPE_NONE && session->stream_info[i].type <= PJMEDIA_TYPE_APPLICATION)
			{
				pjmedia_stream_force_send_KA_packet(session->stream[i]);
			}
			else 
			{
			}
		}
	}
}

/*
 * Funci�n para forzar a que se capture las extensiones de cabecera
 */
PJ_DEF(void) pjmedia_session_reset_ext_header( pjmedia_session *session)
{
	unsigned i;

	if (session == NULL) return;
	if (session->stream_cnt > 2) return;

	for (i = 0; i < session->stream_cnt; i++)
	{		
		if (session->pool != NULL && session->stream[i] != NULL)
		{
			if (session->stream_info[i].type > PJMEDIA_TYPE_NONE && session->stream_info[i].type <= PJMEDIA_TYPE_APPLICATION)
			{
				pjmedia_stream_reset_ext_header(session->stream[i]);
			}
			else 
			{
			}
		}
	}
}

/*
 * Dial DTMF digit to the stream, using RFC 2833 mechanism.
 */
PJ_DEF(pj_status_t) pjmedia_session_dial_dtmf( pjmedia_session *session,
					       unsigned index,
					       const pj_str_t *ascii_digits )
{
    PJ_ASSERT_RETURN(session && ascii_digits, PJ_EINVAL);
    return pjmedia_stream_dial_dtmf(session->stream[index], ascii_digits);
}

/*
 * Check if the specified stream has received DTMF digits.
 */
PJ_DEF(pj_status_t) pjmedia_session_check_dtmf( pjmedia_session *session,
					        unsigned index )
{
    PJ_ASSERT_RETURN(session, PJ_EINVAL);
    return pjmedia_stream_check_dtmf(session->stream[index]);
}


/*
 * Retrieve DTMF digits from the specified stream.
 */
PJ_DEF(pj_status_t) pjmedia_session_get_dtmf( pjmedia_session *session,
					      unsigned index,
					      char *ascii_digits,
					      unsigned *size )
{
    PJ_ASSERT_RETURN(session && ascii_digits && size, PJ_EINVAL);
    return pjmedia_stream_get_dtmf(session->stream[index], ascii_digits, size);
}

/*
 * Install DTMF callback.
 */
PJ_DEF(pj_status_t) pjmedia_session_set_dtmf_callback(pjmedia_session *session,
				  unsigned index,
				  void (*cb)(pjmedia_stream*, 
				 	     void *user_data, 
					     int digit), 
				  void *user_data)
{
    PJ_ASSERT_RETURN(session && index < session->stream_cnt, PJ_EINVAL);
    return pjmedia_stream_set_dtmf_callback(session->stream[index], cb, user_data);
}

PJ_DEF(void) pjmedia_session_force_set_impairments(pjmedia_session* session, int Perdidos, int Duplicados, int LatMin, int LatMax)
{
	pjmedia_stream* stream = NULL;
	pj_assert(session);
	stream = pjmedia_session_get_stream(session, 0);
	if (stream != NULL)
		pjmedia_stream_force_set_impairments(session->pool, stream, Perdidos, Duplicados, LatMin, LatMax);
}

