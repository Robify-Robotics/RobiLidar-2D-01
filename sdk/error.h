#ifndef __ERROR_H_
#define __ERROR_H_

#include <stdarg.h>

#define  SUCCESS   0   //Success
#define  FAILTORUN   -1   //Lidar is not running normally
//Input parameter related
#define  ARG_ERROR_NUM  -10    //Incorrect number of input parameters
#define  ARG_ERROR_FILE_EXIST  -11 //Incorrect configuration file path supplied as an input parameter
#define  ARG_ERROR_TYPE     -12  //Input parameter type is outside the available range
#define  SIGNAL_BIND_FAILED  -13//Signal binding function failed
#define  FUNCTION_NOT_SUPPORT  -14//This device does not support this function
#define  UNDEFINED_ERROR     -15//Unknown error




#define UPDATE_ATTRS_FAILED  -23//Failed to update terminal commands
#define UPDATE_BANDRATE_FIILED -24//Failed to change the port number
#define BIND_SOCKET_PORT_FAILED -26//Failed to bind the port
#define THREAD_CREATE_FAILED -27 //Failed to create the thread
#define READ_UART_FAILED  -28//Failed to read the serial port file in a loop
#define  MUTICAST_GROUP_FIALED  -29//Failed to create multicast mode
#define MSG_POST_FAILED    -30//Failed to send the signal

//open
#define OPEN_UART_FD_FAILED -31//Failed to open the UART handle
#define OPEN_UDP_FD_FAILED -32//Failed to open the UDP handle
#define  OPEN_SERIAL_PORT_FAILED  -33//Failed to open the serial port
#define OPEN_ATTRS_FAILED  -34//Get the terminal handle
#define OPEN_SOCKET_PORT_FAILED -35//Failed to open the socket port


//get
#define GET_DEVINFO_FAILED -41//Failed to get device information
#define GET_DEVPONIT_FAILED -42 //Failed to get device point data
#define GET_DEVTIMESTAMP_FAILED -43 //Failed to get device timestamp
#define GET_ONEPOINT_FAILED	    -44//Failed to get a group of lidar point information
#define Get_ZONE_FAILED  -45 //Failed to read the zone

//set
#define SET_DEVINFO_FAILED -51//Failed to set device information
#define SET_DEV_FAILED  -52//Failed to set device start/stop/restart/test
#define SET_EVENT_FAILED  -53//Failed to set event synchronization
#define Set_ZONE_FAILED  -54 //Failed to set the zone
#define SET_ZONESECTION_FAILED -55 //Failed to set the active zone
//net
#define URL_ADDRESS_FAILED  -60//URL address does not exist

 
#define INFO_OUTPUT      0
#define WARNING_OUTPUT   1
#define DEBUG_OUTPUT     2
#define ERROR_OUTPUT     3
 
#define DEBUG_LEVEL      DEBUG_OUTPUT

#define INFO_PRINT(info,...)  \
do{ \
    if(DEBUG_LEVEL>=INFO_OUTPUT){\
        printf("Info %s,%s,%d:"info"",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);}\
}while(0)
 
#define WARNING_PRINT(info,...)  \
do{ \
    if(DEBUG_LEVEL>=WARNING_OUTPUT){\
        printf("Warning %s,%s,%d:"info"",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);}\
}while(0)
 
#define DEBUG_PRINT(info,...)  \
do{ \
    if(DEBUG_LEVEL>=DEBUG_OUTPUT){\
        printf("Debug %s,%s,%d:"info"",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);}\
}while(0)
 
#define ERROR_PRINT(info,...)  \
do{ \
    if(DEBUG_LEVEL>=ERROR_OUTPUT){\
        printf("Error %s,%s,%d:"info"",__FILE__,__FUNCTION__,__LINE__,##__VA_ARGS__);}\
}while(0)

#endif