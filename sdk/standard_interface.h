#ifndef __STANDARD_INTERFACE_H__
#define __STANDARD_INTERFACE_H__

#include "LidarDataProcess.h"
#include"LidarCheckService.h"
#include "error.h"
#define SDKVERSION "3.2.2" // SDK version number


class BlueSeaLidarSDK
{
public:
	static BlueSeaLidarSDK *getInstance();
	static void deleteInstance();
    
	/************************************************
	* @functionName:  getLidar
	* @date:          2022-08-21
	* @description:   CN:Get current lidar information  EN:get current lidar info
	* @Parameter:
	*				  1.ID[int,IN]			CN:Lidar ID			 EN:lidar ID
	*
	* @return:        RunConfig  struct
	* @others:        Null
	*************************************************/
	RunConfig * getLidar(int ID);
	/************************************************
	* @functionName:  addLidarByPath
	* @date:          2022-07-14
	* @description:   CN:Add an ID using the configuration file path  EN:add lidar by path
	* @Parameter:
	*				  1.cfg_file_name[const char*,IN]			CN:Configuration file path			 EN:config file path
	*
	* @return:        LIDAR ID
	* @others:        Null
	*************************************************/
	int addLidarByPath(const char *cfg_file_name);
	/************************************************
	* @functionName:  delLidarByID
	* @date:          2022-07-14
	* @description:   CN:Delete the specified lidar using the ID returned by addLidarByPath  EN:Delete the specified lidar by the ID returned by addLidarByPath
	* @Parameter:
	*				  1.ID[int,IN]			CN:Lidar ID			 EN:Lidar ID
	*
	* @return:        true/false
	* @others:        Null
	*************************************************/
	bool delLidarByID(int ID);
	/************************************************
	* @functionName:  setCallBackPtr
	* @date:          2022-07-14
	* @description:   CN:Set the lidar callback printing function by ID  EN:Setting up a callback print function for the radar based on an ID
	* @Parameter:
	*				  1.ID[int,IN]			CN:Lidar ID			 EN:Lidar ID
	*				  1.ptr[printfMsg,IN]	CN:Callback printing function		EN:Callback print function
	*
	* @return:        NUll
	* @others:        Null
	*************************************************/
	void setCallBackPtr(int ID,printfMsg ptr);
	/************************************************
	* @functionName:  openDev
	* @date:          2022-03-28
	* @description:   CN:Start the lidar  EN:start lidar
	* @Parameter:
	*				  1.ID[int,IN]			CN:Lidar ID			 EN:Lidar ID
	*
	* @return:        true/false
	* @others:        Null
	*************************************************/
	bool openDev(int ID);
	/************************************************
	* @functionName:  StopDrv
	* @date:          2022-03-28
	* @description:   CN:Stop the lidar  EN:stop lidar
	* @Parameter:
	*				  1.ID[int,IN]			CN:Lidar ID			 EN:Lidar ID
	*
	* @return:        Null
	* @others:        Null
	*************************************************/
	void StopDev(int ID);
	/************************************************
	* @functionName:  GetDevInfo
	* @date:          2023-08-21
	* @description:   CN:Get lidar global parameters  EN:get lidar config arg
	* @Parameter:
	*				  1.ID[int,IN]			CN:Lidar ID			 EN:Lidar ID
	*				  2.eepromv101[EEpromV101,OUT]			CN:Lidar parameter information			 EN:Lidar arg info
	*
	* @return:        true/false
	* @others:        Null
	*************************************************/
	bool GetDevInfo(int ID,EEpromV101 *eepromv101);

	/************************************************
	 * @functionName:  ControlDrv
	 * @date:          2023-08-21
	 * @description:   CN:Set the lidar to start or pause  EN:Set the lidar on or off
	 * @Parameter:	  
	 *					1.ID[int,IN]			CN:Lidar ID			 EN:Lidar ID
	 *				    2.num	[int,IN]   CN:Command length	EN:cmd length
	 *					3.cmd	[int,IN]   CN:Command contents	EN:cmd  data
	 * @return:			true/false
	 * @others:        Null
	 *************************************************/
	bool ControlDrv(int ID, int num, char *cmd);

	/************************************************
	 * @functionName:  getVersion
	 * @date:          2022-05-09
	 * @description:   CN:Get version number  EN:get version
	 * @Parameter:	  Null
	 * @return:        const char*
	 * @others:        Null
	 *************************************************/
	const char *getVersion();
	/************************************************
	 * @functionName:  ZoneSection
	 * @date:          2022-10-18
	 * @description:   CN:Switch the current zone  EN:change  section
	 * @Parameter:	  1.ID [long,IN]  CN: Lidar ID  EN:Lidar ID
	 *				  2.section	[char,IN]   CN:Active zone index	EN:Active zone serial number
	 * @return:        true/false
	 * @others:        Zone value range 0-9 a-f
	 *************************************************/
	bool ZoneSection(int ID, char section);
	/************************************************
	 * @functionName:  SetUDP
	 * @date:          2023-07-31
	 * @description:   CN:Set lidar IP  EN:set lidar ip
	 * @Parameter:		1.ID [long,IN]  CN: Lidar ID  EN:Lidar ID
	 *					2.ip	[char*,IN]   CN:IP address 	EN:ip
	 *					3.mask	[char*,IN]   CN:Subnet mask	EN:mask
	 *					4.gateway	[char*,IN]   CN:Gateway 	EN:gateway
	 *					5.port	[int,IN]   CN:Port 	EN:port
	 * @return:        true/false
	 * @others:        Null
	 *************************************************/
	bool SetUDP(int ID, char* ip, char* mask, char* gateway, int port);
	/************************************************
	 * @functionName:  SetDST
	 * @date:          2023-07-31
	 * @description:   CN:Set lidar upload IP  EN:set lidar post ip  address
	 * @Parameter:		1.ID [long,IN]  CN: Lidar ID  EN:Lidar ID
	 *					2.ip[char*,IN]   CN:IP address 	EN:ip
	 *					3.port[int,IN]   CN:Port 	EN:port
	 * @return:        true/false
	 * @others:        Null
	 *************************************************/
	bool SetDST(int ID, char* ip, int port);
	/************************************************
	 * @functionName:  SetRPM
	 * @date:          2023-08-21
	 * @description:   CN:Set lidar rotation speed  EN:set lidar rpm
	 * @Parameter:		1.ID [long,IN]  CN: Lidar ID  EN:Lidar ID
	 *					2.RPM[int,IN]   CN:Rotation speed 	EN:rpm
	 * @return:        true/false
	 * @others:        rpm   (300-3000)
	 *************************************************/
	bool SetRPM(int ID,int RPM);
	/************************************************
	 * @functionName:  SetTFX
	 * @date:          2023-08-21
	 * @description:   CN:Set lidar fixed upload  EN:set lidar fixed upload
	 * @Parameter:		1.ID [long,IN]  CN: Lidar ID  EN:Lidar ID
	 *					2.tfx[bool,IN]   true/false
	 * @return:        true/false
	 * @others:        Null
	 *************************************************/
	bool SetTFX(int ID,bool tfx);
	/************************************************
	 * @functionName:  SetDSW
	 * @date:          2023-08-21
	 * @description:   CN:Set lidar filtering and trailing point removal  EN:set lidar filtering
	 * @Parameter:		1.ID [long,IN]  CN: Lidar ID  EN:Lidar ID
	 *					2.tfx[bool,IN]   true/false
	 * @return:        true/false
	 * @others:        Null
	 *************************************************/
	bool SetDSW(int ID,bool dsw);
	/************************************************
	 * @functionName:  SetSMT
	 * @date:          2023-08-21
	 * @description:   CN:Set lidar data smoothing  EN:set lidar smooth
	 * @Parameter:		1.ID [long,IN]  CN: Lidar ID  EN:Lidar ID
	 *					2.smt[bool,IN]   true/false
	 * @return:        true/false
	 * @others:        Null
	 *************************************************/
	bool SetSMT(int ID,bool smt);
	/************************************************
	 * @functionName:  SetPST
	 * @date:          2023-08-21
	 * @description:   CN:Set lidar upload mode  EN:set lidar upload way
	 * @Parameter:		1.ID [long,IN]  CN: Lidar ID  EN:Lidar ID
	 *					2.mode[int,IN]  CN:0 no data, 1 data upload only, 2 alarm only, 3 data+alarm  EN:0 no data 1 only data 2 only alarm 3 data+alarm
	 * @return:        true/false
	 * @others:        Null
	 *************************************************/
	bool SetPST(int ID,int mode);
	/************************************************
	 * @functionName:  SetDID
	 * @date:          2023-08-21
	 * @description:   CN:Set lidar number  EN:set lidar number
	 * @Parameter:		1.ID [long,IN]  CN: Lidar ID  EN:Lidar ID
	 *					2.number[unsigned int,IN]   CN:Lidar number  EN:lidar number     
	 * @return:        true/false
	 * @others:        Repeatable, different from ID
	 *************************************************/
	bool SetDID(int ID,unsigned int number);

	/************************************************
	 * @functionName:  SetNTP
	 * @date:          2025-01-17
	 * @description:   CN:Set lidar NTP parameters  EN:set lidar ntp property
	 * @Parameter:		1.ID [long,IN]  CN: Lidar ID  EN:Lidar ID
	 *					2.ntp_ip[char*,IN]   CN:Lidar NTP IP  EN:lidar ntp upload ip   
	 *					3.ntp_port[unsigned int,IN]   CN:Lidar NTP upload port  EN:lidar ntp upload port     
	 *					4.enable[bool,IN]   CN:Enable  EN:enable    
	 * @return:        true/false
	 * @others:        Repeatable, different from ID
	 *************************************************/
	bool SetNTP(int ID,char*ntp_ip,unsigned int port,bool enable);


	/************************************************
	 * @functionName:  getLidarsList
	 * @date:          2023-12-13
	 * @description:   CN:Get heartbeat packet data for network lidars and currently available serial lidars  EN:Get heartbeat packet data for networked Lidars and currently available serial Lidars
	 * @return:        vector
	 * @others:        The network heartbeat packet contains the received timestamp, which needs to be compared with the current timestamp to see if it is offline.
	 *************************************************/
	std::vector<DevConnInfo> getLidarsList();
	bool OpenHeartService();
	bool CloseHeartService();
protected:
	//Set lidar parameters (stored across power loss)
	bool SetDevInfo(RunConfig *lidar,int length,char*cmd,int mode);
	// Open the local service (web test visualization page)
	bool OpenLocalService(int ID);
	// Close the local service
	bool CloseLocalService(int ID);

private:
	static BlueSeaLidarSDK *m_sdk;
	BlueSeaLidarSDK();
	~BlueSeaLidarSDK();

	int m_idx;
	std::vector<RunConfig*> m_lidars;

	LidarCheckService* m_checkservice;
	bool m_checkflag;//Control heartbeat thread shutdown
};
void* lidar_service(void* param);
//Default printing function
void CallBackMsg(int msgtype, void *param,int length);

#endif