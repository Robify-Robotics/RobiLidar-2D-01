#ifndef __LIDARDATAPROCESS_H__
#define  __LIDARDATAPROCESS_H__
#include"Global.h"
#include"service/LidarWebService.h"

#ifdef _WIN32

#elif __unix__ 
#include <sys/time.h>
#endif
typedef void (*printfMsg)(int, void*,int);

enum ACTION
{
	OFFLINE=0,
	ONLINE,
	RUN,
	CONTROL,
	GETALLPARAMS,
	SETPARAM,
	READZONE,
	WRITEZONE,
	FINISH
};
enum STATE
{
	INIT=0,
	WORK,
	WORK_AND_WEB,
	STOP_ALL
};

struct Responsive
{
	int mode;//0x0043  0x0053  0x4753
	int send_len;
	char send_cmd[1024];
	short rand;
	long timestamp;
};

struct RunScript
{
	//connect
	char type[16];//"uart"   or   "udp"  "vpc"
	char connectArg[16];//ip/com
	int connectArg2;//port/baud
	int local_port;
	//data
	int from_zero;
	int output_360;			//CN:Sector printing (0: partial sector printing, 1: complete)EN:Fan printing (0: Partial fan printing 1: Completed)

	int error_circle;		//CN:Detect the number of revolutions with zero length					EN:Detect the number of turns with a length of 0
	double error_scale;		//CN:Detect the proportion with zero length					EN:Detect the Scale of turns with a length of 0

	int service_port;		//CN:Port for enabling the local service					EN:Local service enable port
	int is_open_service;	//CN:Whether to enable the local service					EN:Enable local service
	//udp
	int is_group_listener;	//CN:0 normal mode, 1 listening mode, 2 sending mode	EN:0 Normal mode 1 Listening mode 2 Sending mode
	char group_ip[16];		//CN:Multicast IP	

	// E120 scan filter
	ShadowsFilterParam shadows_filter;
	MedianFilterParam median_filter;
	//E330 scan filter
	SeparationFilterParam separation_filter;
	//get
	int uuid;
	int model;
	int version;

	//set common
	int rpm;
	int resample_res;

	int with_smooth;
	int with_deshadow;

	int with_start;
	//set uart
	int with_confidence;
	int unit_is_mm;
	//set udp  vpc
	int alarm_msg;
	int direction;
	int ats;  //1udp  2vpc
	//ntp
	char ntp_ip[16];
	int ntp_port;
	int ntp_enable;
};

//Runtime configuration 
struct RunConfig
{	
	int ID;
	STATE state;	//-1 stop all 0 init   1 run work thread  2 run work and web thread 

	unsigned long thread_data;//DWORD == pthread_t
	unsigned long thread_web;
#endif 
	LidarWebService *webservice;
	int fd;//Handle
	printfMsg  callback;	//CN:Information printing callback function					EN:Information printing callback function
	UserData userdata;//Current frame
	LidarMsgHdr zonemsg;//Current zone information
	EEpromV101 eepromv101;//Global variable parameters
	char hardwareVersion[32];//Hardware version number
	
	ACTION action;//0 no operation, 1 start/stop control command, 2 get device global parameters, 3 set device parameters, 4 read zone, 5 set zone
	//Current input command
	int mode;
	int send_len;
	char send_cmd[1024];
	int recv_len;
	char recv_cmd[1024];

	RunScript  runscript;
	
};

 int setup_lidar_uart(int fd_uart, RunScript* arg, EEpromV101* eepromv101, char* version);
 int setup_lidar_vpc(int hCom, RunScript* arg);

 bool readConfig(const char* cfg_file_name, RunScript& cfg);
 bool checkPointsLengthZero(UserData *tmp, float scale);
 void* lidar_thread_proc_udp(void* param);
 void* lidar_thread_proc_uart(void* param);


