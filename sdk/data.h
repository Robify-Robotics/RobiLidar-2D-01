#ifndef _LIDAR_DATA
#define  _LIDAR_DATA
/**

 * Copyright (C),  Pacecat:(C) <LanHai>,All right reserved

 * File name:      data.h

 * Author:  	   *
      
 * Version:        1.0  
    
 * Date:		   2022.3.25

 * Description:    Data storage structure and data parsing capabilities for all devices

 */

#include <stdint.h>
#include <string>
#include<stdio.h>
#include <stdlib.h>
#include<vector>
extern "C"
{
	#include"third_party/mongoose/mongoose.h"
}

#define PI 3.1415926535898

#define MAX_LIDARS 8
#define MAX_FANS  100
#define MAX_POINTS 550
#define MAX_FRAMEPOINTS 10500
#define MAX_FRAMEIDX  10000000
#define HDR_SIZE 6
#define HDR2_SIZE 8
#define HDR3_SIZE 16 
#define HDR7_SIZE 28 
#define HDR99_SIZE 32 
#define HDRAA_SIZE 48

#define BUF_SIZE 8*1024
#define ZONE_SIZE 1024
#define SLEEP_SIZE 5
#define USER_SIZE  10

#define ZS_PACK 0x5A53
#define ZG_PACK 0x5A47

#define GO_PACK 0x474F
#define GT_PACK 0x4754
#define G0_PACK 0x4730
#define G1_PACK 0x4731
#define GF_PACK 0x4746

#define FH_PACK 0x4648
#define F_PACK 0x0046
#define PO_PACK 0x504F
#define P0_PACK 0x5030
#define PT_PACK 0x5054
#define P1_PACK 0x5031
#define GS_PACK 0x4753
#define S_PACK 0x0053
#define T_PACK 0x0054
#define C_PACK 0x0043

#define CHECKSERVICE  6789  //Port number for detecting the local lidar list
#define getbit(x,y)   ((x) >> (y)&1)
#define setbit(x,y) x|=(1<<y)         //Set bit Y of X to 1
#define clrbit(x,y) x&=~(1<<y)            //Clear bit Y of X to 0
#define UNUSED(x) (void)(x)

//CN：Heartbeat detection packet EN：Heartbeat detection package
struct KeepAlive {
	uint32_t world_clock;//Timestamp
	uint32_t mcu_hz;//MCU frequency (internal use)
	uint32_t arrive;//Packet travel time from host to lidar (internal use)
	uint32_t delay;//Delay (internal use)
	uint32_t reserved[4];//Reserved bits
};
struct DataPoint
{
	float angle;				//CN:Radians		EN:radian

	float distance;				//CN:Distance (meters)	EN:distance(Meter)

	unsigned char confidence;	//CN:Intensity		EN:strength
};
struct RawData
{
	unsigned short code;		//CN:Frame header										EN:data frame header
	unsigned short N;			//CN:Number of ranging points in the sector								EN:The number of ranging points in the sector
	unsigned short angle;		//CN:Starting angle of the current sector							EN:The starting angle of the current sector *10
	unsigned short span;		//CN:Total sector angle (sector end angle - sector start angle)	EN:The total angle of the sector (sector end angle - sector start angle)
	unsigned short fbase;		//CN:Sector start offset (NULL)						EN:Sector start offset（NULL）
	unsigned short first;		//CN:First point angle (NULL)						EN:first point angle（NULL）
	unsigned short last;		//CN:Last point angle (NULL)						EN:last point angle（NULL）
	unsigned short fend;		//CN:Sector end offset (NULL)							EN:Sector end offset（NULL）
	uint32_t flags;	//Message type
	// short ros_angle;	// 0.1 degree
	DataPoint points[MAX_POINTS];//CN:Detailed scan point information (the number initialized is determined by N)	EN:The specific information of the scanning point (the specific initialization number is determined by N)
	uint32_t ts[2];				//CN:Timestamp (seconds and milliseconds)							EN:timestamps(Second and millisecond )

};

enum DataType
{
	SPANDATA=0,
	FRAMEDATA
};
struct SpanData
{
	//unsigned short N;
	RawData data;
};
struct FrameData
{
	//unsigned short N;			//CN:Number of ranging points in the sector								EN:The number of ranging points in the sector
	uint32_t ts[2];				//CN:Timestamp (seconds and microseconds)							EN:timestamps(Second and microseconds )
	std::vector<DataPoint>data;
	//DataPoint data[MAX_FRAMEPOINTS];//CN:Detailed scan point information (the number initialized is determined by N)	EN:The specific information of the scanning point (the specific initialization number is determined by N)
};
//Real-time lidar data ultimately returned for customer use
struct UserData
{
	DataType type;
	int idx;//0 indicates sector index; 1 indicates frame index; wraps after 10000000 (ten million) frames
	char connectArg1[16];     //ip/com
	int connectArg2;		//port /baud
	//Data is available only in the corresponding mode
	SpanData spandata;
	FrameData framedata;	
};
//Alarm packet
struct LidarMsgHdr
{
	char sign[4];  	// must be "LMSG"
	uint32_t proto_version;	//Protocol version, currently 0x101
	char dev_sn[20];	//Device number
	uint32_t dev_id;  	//Device serial number
	uint32_t timestamp; 	//Timestamp
	uint32_t flags;	//Message type
	uint32_t events;	//Bit combination of message contents
	uint16_t id;	//Message sequence number
	uint16_t extra;	//Length of (currently active zone + device function states + reserved)
	uint32_t zone_actived;	//Currently active zone (range 0~F)
	uint8_t all_states[32];//	Device function states
	uint32_t reserved[11];	//Reserved
};
//E100 series point cloud data filtering
struct ShadowsFilterParam
{
	int enable;
	double max_range;
	double min_angle, max_angle;
	int window;
};
//E100 series point cloud data filtering
struct MedianFilterParam
{
	int enable;
	int window;
};
//E330 series outlier filtering algorithm
struct SeparationFilterParam
{
	int filter_open;
	float max_range;
	float min_range;
	float max_range_difference;
	int filter_window;
};

/*
* RawDataHdr
* CN:Represents different data storage formats; each device uses only one format, but different devices may use the same format
* EN:Represents different data storage formats, each device uses only one format, but different devices may use the same format
*/

struct RawDataHdr
{
	unsigned short code; //CN:Frame header				EN:data frame header
	unsigned short N;    //CN:Number of ranging points in the sector	EN:The number of ranging points in the sector
	unsigned short angle;//CN:Sector start angle		EN:Corresponding ranging angle
};

struct RawDataHdr2
{
	unsigned short code;	//CN:Frame header				EN:data frame header
	unsigned short N;		//CN:Number of ranging points in the sector		EN:The number of ranging points in the sector
	unsigned short angle;	//CN:Sector start angle 		EN:Sector starting angle
	unsigned short span;	//CN:Sector size 			EN:sector size
};

struct RawDataHdr3
{
	unsigned short code;	//CN:Frame header				EN:data frame header
	unsigned short N;		//CN:Number of ranging points in the sector		EN:The number of ranging points in the sector
	unsigned short angle;	//CN:Sector start angle		EN:Corresponding ranging angle
	unsigned short span;	//CN:Sector size			EN:sectors size
	unsigned short fbase;	//CN:Sector start offset		EN:Sector start offset
	unsigned short first;	//CN:First point angle		EN:first point angle
	unsigned short last;	//CN:Last point angle		EN:last point angle
	unsigned short fend;	//CN:Sector end offset		EN:Sector end offset
};


struct RawDataHdr7 {
	uint16_t code;		//CN:Frame header						EN:data frame header
	uint16_t N;			//CN:Number of ranging points in this subpacket within the sector	EN:The number of ranging points for this subpacket in the buffer
	uint16_t whole_fan;	//CN:Total number of ranging points in the buffer			EN:The total number of ranging points in the sector
	uint16_t ofset;		//CN:Sector offset					EN:sector offset
	uint32_t beg_ang;	//CN:Sector start angle				EN:Sector start angle
	uint32_t end_ang;	//CN:Sector end angle				EN:Sector end angle
	uint32_t flags;		//CN:Status switch flag				EN:status switch flag
	uint32_t timestamp;	//CN:Timestamp						EN:timestamp		(other:The data timestamps of different sectors in a frame are consistent)
	uint32_t dev_id;	//CN:Device ID						EN:device ID
};


struct FanSegment_C7
{
	RawDataHdr7 hdr;			//CN：Hdr7 structure		EN：Hdr7structure 

	uint16_t dist[MAX_POINTS];	//CN:Distance				EN:distance
	uint16_t angle[MAX_POINTS]; //CN:Angle				EN:angle
	uint8_t energy[MAX_POINTS]; //CN:Energy intensity			EN:energy intensity

	struct FanSegment_C7* next;	// CN:Pointer to the next sector		EN:next sector pointer 
};
struct RawDataHdrAA {
	uint16_t code; // 0xFAAA
	uint16_t N;
	uint16_t whole_fan;
	uint16_t ofset;
	uint32_t beg_ang;
	uint32_t end_ang;
	uint32_t flags;
	uint32_t second;
	uint32_t nano_sec;
	uint32_t dev_id;
	uint32_t reserved[4];
};
struct FanSegment_AA
{
	RawDataHdrAA hdr;			//CN：HdrAA structure		EN：Hdr7structure

	uint16_t dist[MAX_POINTS];	//CN:Distance				EN:distance
	uint16_t angle[MAX_POINTS]; //CN:Angle				EN:angle
	uint8_t energy[MAX_POINTS]; //CN:Energy intensity			EN:energy intensity

	struct FanSegment_AA* next;	// CN:Pointer to the next sector		EN:next sector pointer
};
struct RawDataHdr99 {
	uint16_t code;			//CN:Frame header, fixed at 0x99FA			EN:Frame header, fixed at 0x99FA
	uint16_t N;				//CN:Number of ranging points in this subpacket within the sector	EN:The number of ranging points for this subpacket in the sector
	uint16_t from;			//CN:Sector start angle			    EN:Sector start angle
	uint16_t total;			//CN:Number of data points in a full revolution			EN:The number of data points in a full circle
	uint32_t flags;			//CN:Status switch flag				EN:status switch flag
	uint32_t timestamp;		//CN:Timestamp						EN:timestamp 
	uint32_t dev_no;		//CN:Device number					EN:device ID
	uint32_t reserved[3];	//CN:Reserved bits						EN:reserved
};


//Lidar device configuration set by the customer
struct DevData
{
	int RPM;//Rotation speed, range 0450-1200, for example: LSRPM:0450H
	int ERR;//Offset, range -99-+99, for example: LSERR:-23H  LSERR:+23H
	char UDP[64];//Combined UDP information: set lidar IP address, subnet mask, gateway, and service port, for example LSUDP:192.168.158.091 255.255.255.000 192.168.158.001 05000H
	char DST[64];//IP address and port receiving lidar information  LSDST:192.168.158.043 12300H
	char NSP[32];//Device type    LSNSP:LDS-50C-S-UH
	char UID[32];//Device serial number, for example LSUID:201812030001H
	int FIR;//Number of filtering revolutions (range 01~99), for example: LSFIR:03H
	int PUL;//Set motor startup pulse count (range 0500~4000), for example: LSPUL:2000H
	int VER;//Set version number, for example: LSPUL:2000H
	int PNP;//Set IO type; for example, LSNPN:1H sets the output IO type to PNP
	int SMT;//Data smoothing  LSSMT:1H enable  LSSMT:0H disable
	int DSW;//Trailing point removal  LSDSW:1H enable  LSDSW:0H disable
	int DID;//Device ID   LSDID:xxxH
	int ATS;//Automatic upload at startup  LSATS:xH    1/0 enable/disable   
	int TFX;//Fixed upload address  LSTFX:xH    1/0 enable/disable   
	//Additionally
	int PST;//Data/alarm upload type  LSPST:xH    0: none 1: data 2: alarm 3: data+alarm  
	int AF;//Trailing point removal coefficient
	char set[18];//Parameters to set: 1 set, 0 do not set	
	char result[35];//Whether setting succeeded: 1 success, 0 failure
};
struct  DevTimestamp
{
	char ip[256];
	int port;
	uint32_t timestamp;
	uint32_t delay;
};

struct EEpromV101
{
	char label[4];			// "EPRM"
    uint16_t pp_ver; // Protocol version
    uint16_t size;			// Structure size

	//uint32_t version;		// firmware version

	// device
    uint8_t dev_sn[20];//SN number
    uint8_t dev_type[16];//Model
    uint32_t dev_id;		// Number

	// network
	uint8_t IPv4[4];
	uint8_t mask[4];
	uint8_t gateway[4];
    uint8_t srv_ip[4];//Upload IP
    uint16_t srv_port;//Upload port
    uint16_t local_port;//Local port

	uint16_t RPM;
    uint16_t RPM_pulse;//Motor startup parameters
    uint8_t fir_filter;//Zone alarm point count threshold
    uint8_t cir;//Zone alarm revolution count filtering threshold
	uint16_t with_resample;

    uint8_t auto_start;//Automatic rotation at startup
    uint8_t target_fixed;//Fixed upload
    uint8_t with_smooth;//Data smoothing
    uint8_t with_filter;//Trailing point removal

    //
    uint8_t ranger_bias[8];//Zero-offset angle error correction
    uint32_t net_watchdog;//Watchdog

    uint32_t pnp_flags;//PNP/NPN
    uint16_t deshadow;//Smoothing coefficient
    uint8_t zone_acted;//Active zone
    uint8_t should_post;//Upload mode: 0 no data, 1 data only, 2 alarm, 3 alarm+data

    uint8_t functions_map[16];//I/O input and output ports
    uint8_t reserved[36];

};
//CN:Packet header used for receiving when setting lidar parameters over UDP	EN:UDP uses the header when setting lidar parameters
struct CmdHeader
{
	unsigned short sign; //CN:Flag agreed upon with the hardware						EN:Flags consistent with hardware
	unsigned short cmd;	 //CN:Command										EN：command
	unsigned short sn;	//CN:Random number checked for consistency between sent and received packets	EN:Random numbers, verify consistency when sending and receiving messages
	unsigned short len;	//CN:Command length									EN:command length
};

//Middle four bytes of the serial lidar status packet (STXXXXED)
struct UartState
{
    //byte1
    bool unit_mm;//0 cm 1 mm
    bool with_conf;//0 close 1 open
    bool with_smooth;
    bool with_fitter;
    bool span_9;
    bool span_18;
    bool span_other;
    bool resampele;//Resampling
    //byte2

    bool moter_turn;//0 forward, 1 reverse
    bool span_8;
    bool span_16;
    //byte3
    bool byte3_error0;
    bool byte3_error1;
    bool byte3_error2;
    bool byte3_error3;
    //byte4
    bool byte4_error0;
    bool byte4_error1;
    bool byte4_error2;
    bool byte4_error3;
    bool byte4_error4;
    bool byte4_error5;
    bool byte4_error6;
    bool byte4_error7;
};
struct DevInfo
{
	//Rotation speed, 2 bytes
	unsigned short rpm;
	//Startup pulse count, 2 bytes
	unsigned short pulse;
	//Reserved, 4 bytes
	char sign[4];
	//Version number, 2 bytes
	unsigned short version;
	//IP address, 4 bytes
	unsigned char ip[4];
	//Subnet mask, 4 bytes
	unsigned char mask[4];
	//Gateway address, 4 bytes
	unsigned char gateway[4];
	//Default destination IP, 4 bytes
	unsigned char remote_ip[4];
	//Default destination UDP port, 2 bytes
	unsigned short remote_udp;
	//Default external UDP service port, 2 bytes
	unsigned short port;
	//Object resolution, 1 byte
	unsigned char fir;
	//Bias, 6 bytes
	char zero_offset[6];
	//Device serial number, 20 bytes
	char dev_sn[20];
	//Device type, 11 bytes
	char dev_type[11];
	//IO type selection, 1 byte
	char io_type;
	//Response revolution count, 1 byte 
	unsigned char cir;
	//IO function pin configuration, 10 bytes
	unsigned char io_mux[10];
};
struct DevInfo2
{
	// Label, 4 bytes
	char sign[4];
	// Device serial number, 20 bytes
	char dev_sn[20];
	// Device type, 11 bytes
	char dev_type[12];
	//Version number, 2 bytes
	unsigned short version;
	// IP address, 4 bytes
	unsigned char ip[4];
	// Subnet mask, 4 bytes
	unsigned char mask[4];
	// Gateway address, 4 bytes
	unsigned char gateway[4];
	// Default destination IP, 4 bytes
	unsigned char remote_ip[4];
	//Default destination UDP port, 2 bytes
	unsigned short remote_udp;
	// Default external UDP service port, 2 bytes
	unsigned short port;
	//Reserved, 2 bytes
	char reserver[2];
};


struct DevInfoV101
{
	char sign[4];  // must be "LiDA"
	uint32_t proto_version; // Ð­Òé°æ±¾ V101
	uint32_t timestamp[2];// Ê±¼ä´Á
	char dev_sn[20];
	char dev_type[16];
	uint32_t version;
	uint32_t dev_id; //Éè±¸±àºÅ
	uint8_t ip[4]; //Éè±¸µØÖ·
	uint8_t mask[4]; //
	uint8_t gateway[4];
	uint8_t remote_ip[4]; // ·þÎñÆ÷µØÖ·

	uint16_t remote_udp; //·þÎñÆ÷¶Ë¿Ú
	uint16_t port; // lidar service udp port
	uint16_t status;// Éè±¸×´Ì¬
	uint16_t rpm; // µ±Ç°×ªËÙ

	uint16_t freq; // µ±Ç°×ªËÙ
	uint8_t ranger_version[2];
	uint16_t CpuTemp;
	uint16_t InputVolt;
	uint8_t alarm[16]; // ±¨¾¯ÐÅÏ¢
	uint32_t crc;
};

//#pragma pack(pop)

enum ConnType
{
	TYPE_COM,
	TYPE_UDP_V1,
	TYPE_UDP_V2,
	TYPE_UDP_V101
};

struct DevConnInfo
{
	ConnType type;
	char com_port[128];
	int com_speed;
	char conn_ip[32];
	int conn_port;
	char timeStr[16];  //HH-MM-SS
	union {
		DevInfo v1;// info;
		DevInfoV101 v101;// info101;
		DevInfo2 v2;
	} info;
};
#endif
