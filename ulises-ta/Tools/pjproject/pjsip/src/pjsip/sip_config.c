/* $Id: sip_config.c 2394 2008-12-23 17:27:53Z bennylp $ */
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

#include <pjsip/sip_config.h>

/* pjsip configuration instance, initialized with default values */
pjsip_cfg_t pjsip_sip_cfg_var =
{
    /* Transaction settings */
    {
       PJSIP_MAX_TSX_COUNT,
       PJSIP_T1_TIMEOUT,
       PJSIP_T2_TIMEOUT,
       PJSIP_T4_TIMEOUT,
       PJSIP_TD_TIMEOUT,
		 PJSIP_TSX_TIMEOUT,
		 PJSIP_INV_PROCEEDING_IA_TIMEOUT,
		 PJSIP_INV_PROCEEDING_DIA_TIMEOUT,
		 PJSIP_INV_PROCEEDING_RD_TIMEOUT
    },

    /* Client registration client */
    {
	PJSIP_REGISTER_CLIENT_CHECK_CONTACT
    }
};


#ifdef PJ_DLL
PJ_DEF(pjsip_cfg_t*) pjsip_cfg(void)
{
    return &pjsip_sip_cfg_var;
}
#endif
