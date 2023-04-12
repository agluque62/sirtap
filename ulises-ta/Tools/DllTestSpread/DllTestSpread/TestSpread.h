#ifndef TESTSPREAD_H
#define TESTSPREAD_H

#define TestSP_MAX_PROCS	128
#define TestSP_MAX_NAME_SIZE	256
#define TestSP_MAX_IP_SIZE	22

typedef struct TestSP_report_proc
{
	char name[TestSP_MAX_NAME_SIZE];
	int	state;
	int	gstate;
	int	packet_sent;
	int	packet_recv;
	int	packet_delivered;
	int	retrans;
	char my_ip[TestSP_MAX_IP_SIZE];
	char leader_ip[TestSP_MAX_IP_SIZE];
	char leader_name[TestSP_MAX_NAME_SIZE];
	int	membership_changes;
	int	num_procs;
	int	num_segments;
	int	window;
	int	personal_window;
	int	accelerated_ring;
	int	accelerated_window;
	int	num_sessions;
	int	num_groups;
	int	major_version;
	int	minor_version;
	int	patch_version;

} TestSP_report_proc;

typedef void(__stdcall* Report_Spread_Status_Cb)(int numprocs, TestSP_report_proc*);

__declspec(dllexport) int Init_Spread_Status(char* sp_config_file, char* proc_name, Report_Spread_Status_Cb report_Spread_Status_Cb, char* error, int size_error);
__declspec(dllexport) int End_Spread_Status(void);
__declspec(dllexport) int Get_Spread_Status(char* error, int size_error);

#endif



