/*
 * The Spread Toolkit.
 *
 * The contents of this file are subject to the Spread Open-Source
 * License, Version 1.0 (the ``License''); you may not use
 * this file except in compliance with the License.  You may obtain a
 * copy of the License at:
 *
 * http://www.spread.org/license/
 *
 * or in the file ``license.txt'' found in this distribution.
 *
 * Software distributed under the License is distributed on an AS IS basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Creators of Spread are:
 *  Yair Amir, Michal Miskin-Amir, Jonathan Stanton, John Schultz.
 *
 *  Copyright (C) 1993-2014 Spread Concepts LLC <info@spreadconcepts.com>
 *
 *  All Rights Reserved.
 *
 * Major Contributor(s):
 * ---------------
 *    Amy Babay            babay@cs.jhu.edu - accelerated ring protocol.
 *    Ryan Caudy           rcaudy@gmail.com - contributions to process groups.
 *    Claudiu Danilov      claudiu@acm.org - scalable wide area support.
 *    Cristina Nita-Rotaru crisn@cs.purdue.edu - group communication security.
 *    Theo Schlossnagle    jesus@omniti.com - Perl, autoconf, old skiplist.
 *    Dan Schoenblum       dansch@cnds.jhu.edu - Java interface.
 *
 */


#define	status_ext

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

 //#include "arch.h"
#include "mutex.h"

#ifdef _REENTRANT

#ifndef 	ARCH_PC_WIN95

#include        <sys/types.h>
#include        <sys/socket.h>
#include 	<pthread.h>

#else		/* ARCH_PC_WIN95 */

#include	<windows.h>
#include        <winsock2.h>
#include <iphlpapi.h>
#define ioctl   ioctlsocket
WSADATA		WSAData;

#endif		/* ARCH_PC_WIN95 */

#endif /* _REENTRANT */

#include "spu_scatter.h"
#include "dllconfiguration.h"
#include "spu_events.h"
#include "status.h"
#include "net_types.h"
#include "spu_data_link.h"
#include "spu_alarm.h"
#include "TestSpread.h"

#define TESTSP_SELECT_TIMEOUT 1
#define TESTSP_SELECT_ERROR 2
#define TESTSP_RECV_ERROR 3
#define TESTSP_ERROR -1

static TestSP_report_proc TestReportProc[TestSP_MAX_PROCS];
static int numTestReportProc = 0;
static int status_requested = 0;
static	mutex_type	Status_mutex;

static	channel		SendChan;
static	sys_scatter	Pack_scat;
static	packet_header	Pack;

static	configuration	Cn;
static  proc            My;
static	int		My_port;
static	char* My_name;
static  char		Config_file[80];

static DWORD Read_thread_id;
static int Read_thread_routine_exit = 0;

static	sys_scatter	Report_scat;
static	packet_header	Report_pack;
static  channel         Report_socket;

static	int	Report_message(mailbox fd, int dummy, void* dummy_p);

static	HANDLE		Read_thread;
static	DWORD WINAPI    Read_thread_routine(void*);

static int read_configuration(char* error, int size_error);

Report_Spread_Status_Cb _report_Spread_Status_Cb = NULL;

/*
	Esta función se debe llamar para inicializar la librería.

Parámetros:
sp_config_file. Esta función inicializa la librería con el fichero de configuración del spread. que es spread.conf. Ese fichero suele estar donde está instalado el mcast. Y en el caso de que se llame desde el servicio del MCAST, sólo es necesario que el string sea "spread.conf".
proc_name. Es el nombre del proc. En el caso de Ulises es "PICT01" , "PICT02", etc. El que está en el parámetro Settings.Default.IdHost del servicio MCAST.
report_Spread_Status_Cb. La callback que se llama cuando se reciben los estados.

Si la función retorna error (un entero negativo), en char* error, int size_error retorna un string con la descripción del error.
*/
__declspec(dllexport) int Init_Spread_Status(char* sp_config_file, char *proc_name, Report_Spread_Status_Cb report_Spread_Status_Cb, char* error, int size_error)
{
	int     ret = 0;

	_report_Spread_Status_Cb = report_Spread_Status_Cb;

	Mutex_init(&Status_mutex);
	numTestReportProc = 0;
	memset(&TestReportProc, 0, sizeof(TestReportProc));

	ret = WSAStartup(MAKEWORD(2, 0), &WSAData);
	if (ret != 0)
	{
		snprintf(error, size_error, "Init_Spread_Status: winsock initialization error %d\n", ret);
		return -1;
	}

	strncpy(Config_file, sp_config_file, sizeof(Config_file) - 1);
	if (read_configuration(error, size_error) < 0)
	{
		WSACleanup();
		return -1;
	}

	proc p;
	if (Conf_proc_by_name(proc_name, &p) < 0)
	{
		snprintf(error, size_error, "Init_Spread_Status: proc name is not in spread configuration file\n");
		return -1;
	}		

	Pack_scat.elements[0].len = sizeof(packet_header);
	Pack_scat.elements[0].buf = (char*)&Pack;

	Pack.proc_id = My.id;
	My_port = 65043;
	Pack.seq = My_port;
	Pack.memb_id.proc_id = 15051963;

	Report_scat.num_elements = 2;
	Report_scat.elements[0].buf = (char*)&Report_pack;
	Report_scat.elements[0].len = sizeof(packet_header);
	Report_scat.elements[1].buf = (char*)&GlobalStatus;
	Report_scat.elements[1].len = sizeof(status);
			
	SendChan = DL_init_channel(SEND_CHANNEL, My_port+1, 0, p.id);
	if (SendChan < 0)
	{
		WSACleanup();
		snprintf(error, size_error, "Init_Spread_Status: SendChan initialization error %d\n", SendChan);
		return -1;
	}

	Report_socket = DL_init_channel(RECV_CHANNEL, My_port, 0, p.id);
	if (Report_socket < 0)
	{
		DL_close_channel(SendChan);
		WSACleanup();
		snprintf(error, size_error, "Init_Spread_Status: Report_socket initialization error %d\n", Report_socket);
		return -1;
	}

	//E_init(); /* both reentrent and non-reentrant code uses events */

	Read_thread = CreateThread(NULL, 0, Read_thread_routine, NULL, 0, &Read_thread_id);
	if (Read_thread <= 0)
	{
		DL_close_channel(SendChan);
		DL_close_channel(Report_socket);
		WSACleanup();
		snprintf(error, size_error, "Init_Spread_Status: Read_thread initialization error\n");
		return -1;
	}

	return 0;
}

/*
Se debe llamar antes de salir de la aplicación.
*/
__declspec(dllexport) int End_Spread_Status(void)
{
	Read_thread_routine_exit = 1;
	WaitForSingleObject(Read_thread, 10000);
	DL_close_channel(Report_socket);
	DL_close_channel(SendChan);
	WSACleanup();
	return 0;
}

/*
Se llama cada vez que se quiere recibir los estados. 
Es necesario que pase un tiempo de unos 10 segundos mínimo entre llamadas a esta función para que dé tiempo a recibir los estados.

Si la función retorna error (un entero negativo), en char* error, int size_error retorna un string con la descripción del error. 
*/
__declspec(dllexport) int Get_Spread_Status(char *error, int size_error)
{
	int32	proc_id;
	proc	p;
	int	proc_index;
	int	j, k;

	Mutex_lock(&Status_mutex);
	numTestReportProc = 0;
	memset(&TestReportProc, 0, sizeof(TestReportProc));
	status_requested = 1;
	Mutex_unlock(&Status_mutex);

	Pack.type = STATUS_TYPE;
	Pack.type = Set_endian(Pack.type);
	Pack.conf_hash = MONITOR_HASH;
	Pack.data_len = 0;
	Pack_scat.num_elements = 1;
	for (j = 0; j < Cn.num_segments; j++)
	{
		for (k = 0; k < Cn.segments[j].num_procs; k++)
		{
			proc_id = Cn.segments[j].procs[k]->id;
			proc_index = Conf_proc_by_id(proc_id, &p);
			DL_send(SendChan, p.id, p.port, &Pack_scat);
		}
	}
	Sleep(1000);

	return 0;
}

static	int	Report_message(mailbox fd, int dummy, void* dummy_p)
{
	int i;
	proc	p;
	proc	leader_p;
	int	ret;
	int	ret1, ret2;
	static	int32	last_mes;
	static	int32	last_aru;
	static	int32	last_sec;
	struct timeval timeout;
	fd_set fds;

	timeout.tv_sec = 3;
	timeout.tv_usec = 0;

	last_mes = GlobalStatus.message_delivered;
	last_aru = GlobalStatus.aru;
	last_sec = GlobalStatus.sec;

	TestSP_report_proc TestSP;
	
	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	int nfds;
#ifdef _WIN32
	nfds = 0;
#else
	nfds = s + 1;
#endif

	int sret = select(nfds, &fds, NULL, NULL, &timeout);
	if (sret == 0) {
		return TESTSP_SELECT_TIMEOUT;
	}
	else if (sret < 0)
	{
		printf("Report_message: error in select\n");
		return TESTSP_SELECT_ERROR;
	}

	ret = DL_recv(fd, &Report_scat);
	if (ret <= 0) {
		Alarm(DEBUG, "Report_messsage: DL_recv failed with ret %d, errno %d\n", ret, sock_errno);
		printf("Report_messsage: DL_recv failed with ret %d, errno %d\n", ret, sock_errno);
		return TESTSP_RECV_ERROR;
	}
		
	if (!Same_endian(Report_pack.type))
	{
		GlobalStatus.sec = Flip_int32(GlobalStatus.sec);
		GlobalStatus.state = Flip_int32(GlobalStatus.state);
		GlobalStatus.gstate = Flip_int32(GlobalStatus.gstate);
		GlobalStatus.packet_sent = Flip_int32(GlobalStatus.packet_sent);
		GlobalStatus.packet_recv = Flip_int32(GlobalStatus.packet_recv);
		GlobalStatus.packet_delivered = Flip_int32(GlobalStatus.packet_delivered);
		GlobalStatus.retrans = Flip_int32(GlobalStatus.retrans);
		GlobalStatus.u_retrans = Flip_int32(GlobalStatus.u_retrans);
		GlobalStatus.s_retrans = Flip_int32(GlobalStatus.s_retrans);
		GlobalStatus.b_retrans = Flip_int32(GlobalStatus.b_retrans);
		GlobalStatus.aru = Flip_int32(GlobalStatus.aru);
		GlobalStatus.my_aru = Flip_int32(GlobalStatus.my_aru);
		GlobalStatus.highest_seq = Flip_int32(GlobalStatus.highest_seq);
		GlobalStatus.token_hurry = Flip_int32(GlobalStatus.token_hurry);
		GlobalStatus.token_rounds = Flip_int32(GlobalStatus.token_rounds);
		GlobalStatus.my_id = Flip_int32(GlobalStatus.my_id);
		GlobalStatus.leader_id = Flip_int32(GlobalStatus.leader_id);
		GlobalStatus.message_delivered = Flip_int32(GlobalStatus.message_delivered);
		GlobalStatus.membership_changes = Flip_int16(GlobalStatus.membership_changes);
		GlobalStatus.num_procs = Flip_int16(GlobalStatus.num_procs);
		GlobalStatus.num_segments = Flip_int16(GlobalStatus.num_segments);
		GlobalStatus.window = Flip_int16(GlobalStatus.window);
		GlobalStatus.personal_window = Flip_int16(GlobalStatus.personal_window);
		GlobalStatus.accelerated_ring = Flip_int16(GlobalStatus.accelerated_ring);
		GlobalStatus.accelerated_window = Flip_int16(GlobalStatus.accelerated_window);
		GlobalStatus.num_sessions = Flip_int16(GlobalStatus.num_sessions);
		GlobalStatus.num_groups = Flip_int16(GlobalStatus.num_groups);
		GlobalStatus.major_version = Flip_int16(GlobalStatus.major_version);
		GlobalStatus.minor_version = Flip_int16(GlobalStatus.minor_version);
		GlobalStatus.patch_version = Flip_int16(GlobalStatus.patch_version);
	}

	ret1 = Conf_proc_by_id(GlobalStatus.my_id, &p);
	ret2 = Conf_proc_by_id(GlobalStatus.leader_id, &leader_p);
	if (ret1 < 0)
	{
		printf("Report_message: Skiping illegal status \n");
		printf("==================================\n");
		return TESTSP_ERROR;
	}

	if (ret2 < 0)
	{
		printf("%s state %d gstate %d, %d procs in %d segments, leader %d accelerated %d retrans %d\n", p.name,
			GlobalStatus.state, GlobalStatus.gstate,
			GlobalStatus.num_procs, GlobalStatus.num_segments,
			GlobalStatus.leader_id,
			GlobalStatus.accelerated_ring,
			GlobalStatus.retrans);	

		leader_p.name[0] = 0;
	}
	else
	{
		printf("%s state %d gstate %d, %d procs in %d segments, leader %s accelerated %d retrans %d\n", p.name,
			GlobalStatus.state, GlobalStatus.gstate,
			GlobalStatus.num_procs, GlobalStatus.num_segments,
			leader_p.name,
			GlobalStatus.accelerated_ring,
			GlobalStatus.retrans);
	}
	strncpy(TestSP.name, p.name, TestSP_MAX_NAME_SIZE - 1);
	TestSP.state = GlobalStatus.state;
	TestSP.gstate = GlobalStatus.gstate;
	TestSP.packet_sent = GlobalStatus.packet_sent;
	TestSP.packet_recv = GlobalStatus.packet_recv;
	TestSP.packet_delivered = GlobalStatus.packet_delivered;
	TestSP.retrans = GlobalStatus.retrans;

	int id = htonl(GlobalStatus.my_id);	
	inet_ntop(AF_INET, &(id), TestSP.my_ip, TestSP_MAX_IP_SIZE);

	id = htonl(GlobalStatus.leader_id);
	inet_ntop(AF_INET, &(id), TestSP.leader_ip, TestSP_MAX_IP_SIZE);
	
	strncpy(TestSP.leader_name, leader_p.name, TestSP_MAX_NAME_SIZE - 1);
	TestSP.membership_changes = GlobalStatus.membership_changes;
	TestSP.num_procs = GlobalStatus.num_procs;
	TestSP.num_segments = GlobalStatus.num_segments;
	TestSP.window = GlobalStatus.window;
	TestSP.personal_window = GlobalStatus.personal_window;
	TestSP.accelerated_ring = GlobalStatus.accelerated_ring;
	TestSP.accelerated_window = GlobalStatus.accelerated_window;
	TestSP.num_sessions = GlobalStatus.num_sessions;
	TestSP.num_groups = GlobalStatus.num_groups;
	TestSP.major_version = GlobalStatus.major_version;
	TestSP.minor_version = GlobalStatus.minor_version;
	TestSP.patch_version = GlobalStatus.patch_version;

	int ya_existe = 0;
	Mutex_lock(&Status_mutex);
	for (i = 0; i < TestSP_MAX_PROCS; i++)
	{
		if (strcmp(TestReportProc[i].name, TestSP.name) == 0)
		{
			ya_existe = 1;
			break;
		}
	}

	if (ya_existe == 0)
	{
		memcpy(&TestReportProc[numTestReportProc], &TestSP, sizeof(TestSP_report_proc));
		if (numTestReportProc < TestSP_MAX_PROCS) numTestReportProc++;
	}
	else
	{
		memcpy(&TestReportProc[i], &TestSP, sizeof(TestSP_report_proc));
	}
	Mutex_unlock(&Status_mutex);

	return 0;
}

static int read_configuration(char* error, int size_error)
{
	if (Conf_init(Config_file, My_name, error, size_error) < 0)
	{
		return -1;
	}
	Cn = Conf();
	My = Conf_my();

	Alarm_clear_types(ALL);
	Alarm_set_types(PRINT | EXIT);

	return 0;
}

#ifdef	_REENTRANT

#ifndef 	ARCH_PC_WIN95
static	void* Read_thread_routine()
#else		/* ARCH_PC_WIN95 */
static	DWORD WINAPI    Read_thread_routine(void* dummy)
#endif		/* ARCH_PC_WIN95 */
{
	for (;;)
	{
		if (Report_message(Report_socket, 0, NULL) == TESTSP_SELECT_TIMEOUT)
		{
			Mutex_lock(&Status_mutex);
			if (status_requested)
			{
				if (_report_Spread_Status_Cb)
				{
					_report_Spread_Status_Cb(numTestReportProc, TestReportProc);
				}
				status_requested = 0;
			}
			Mutex_unlock(&Status_mutex);
		}

		if (Read_thread_routine_exit) break;
	}
	return(0);
}

#endif	/* _REENTRANT */



