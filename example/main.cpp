/**
 * CN:
 * Main program entry point
 * 1.Copy the SDK folder to the current directory of your project source
 * 2.Include the standard_interface.h header
 *
 * EN:
 * main program entry
 * 1. Copy the files of the specified platform and the common basic files (data.h, parse.cpp.user.cpp) under the SDK to the current directory of your own project source code
 * 2. Reference data.h,udp_linux.h header file
 */

#include <stdio.h>
#include <string.h>
#include "../sdk/standard_interface.h"

// Print through the supplied callback pointer
void CallBackMsg(int msgtype, void *param,int length)
{
	
	switch (msgtype)
	{
	// Real-time lidar point cloud data
	case 1:
	{
		UserData *pointdata = (UserData *)param;
		if (pointdata->type == FRAMEDATA)
		{
			 //printf("frame idx:%d  %s\t%d \t num:%d timestamp:%d.%d\n", pointdata->idx, pointdata->connectArg1, pointdata->connectArg2, pointdata->framedata.data.size(), pointdata->framedata.ts[0], pointdata->framedata.ts[1]);
			//  for (int i = 0; i <pointdata->framedata.data.size(); i++)
			//  {
			//  	printf("%s\t%d \t%.5f\t%.3f\t%d\n", pointdata->connectArg1, pointdata->connectArg2,  pointdata->framedata.data[i].angle, pointdata->framedata.data[i].distance, pointdata->framedata.data[i].confidence);
			//  }
		}
		else
		{
			 //printf("span idx:%d  %s\t%d \t num:%d timestamp:%d.%d\n", pointdata->idx, pointdata->connectArg1, pointdata->connectArg2, pointdata->spandata.data.N, pointdata->spandata.data.ts[0], pointdata->spandata.data.ts[1]);
			//  for (int i = 0; i <pointdata->spandata.data.N; i++)
			//  {
			//  	printf("%s\t%d \t%.5f\t%.3f\t%d\n", pointdata->connectArg1, pointdata->connectArg2,  pointdata->spandata.data.points[i].angle, pointdata->spandata.data.points[i].distance, pointdata->spandata.data.points[i].confidence);
			//  }
		}
		break;
	}
	// Real-time alarm data
	case 2:
	{
		LidarMsgHdr *zone = (LidarMsgHdr *)param;
		uint32_t event = zone->events;
		std::string text;
		if (zone->flags % 2 == 1)
		{
			// Hardware alarm information
			if (getbit(event, 0) == 1)
				text += "Insufficient power supply";
			if (getbit(event, 1) == 1)
				text += "Motor stalled";
			if (getbit(event, 2) == 1)
				text += "Ranging module overtemperature";
			if (getbit(event, 3) == 1)
				text += "Network error";
			if (getbit(event, 4) == 1)
				text += "No output from ranging module";
			//printf("alarm MSG:%s\n", text.c_str());
		}
		if (zone->flags >= 0x100)
		{
			// Protection zone alarm information
			if (getbit(event, 12) == 1)
				text += "Observation！！！";
			if (getbit(event, 13) == 1)
				text += "Warning！！！";
			if (getbit(event, 14) == 1)
				text += "Alarm！！！";
			if (getbit(event, 15) == 1)
				text += "Obstruction！";
			if (getbit(event, 16) == 1)
				text += "No data";
			if (getbit(event, 17) == 1)
				text += "No protection zone configured";
			if (getbit(event, 18) == 1)
				text += "Internal system error";
			if (getbit(event, 19) == 1)
				text += "Abnormal system operation";
			if (getbit(event, 20) == 1)
				// Duplicates the fourth item above; disabled here
				// text+='Network error\n'
				if (getbit(event, 21) == 1)
					text += "Device updating";
			if (getbit(event, 22) == 1)
				text += "Zero-position output";
			//printf("Active zone:%d\tMSG:%s\n", zone->zone_actived, text.c_str());
		}
		break;
	}
	// Get global parameters of the network lidar model
	case 3:
	{
		EEpromV101 *eepromv101 = (EEpromV101 *)param;
		// Type, ID, serial number
		printf("dev info: Device ID:%d\t Serial number:%s\t Type:%s\n", eepromv101->dev_id, eepromv101->dev_sn, eepromv101->dev_type);
		// IP address Subnet mask Gateway address Default destination IP  Default destination UDP port  Default external UDP service port
		char tmp_IPv4[16] = {0};
		char tmp_mask[16] = {0};
		char tmp_gateway[16] = {0};
		char tmp_srv_ip[16] = {0};

		sprintf(tmp_IPv4, "%d.%d.%d.%d", eepromv101->IPv4[0], eepromv101->IPv4[1], eepromv101->IPv4[2], eepromv101->IPv4[3]);
		sprintf(tmp_mask, "%d.%d.%d.%d", eepromv101->mask[0], eepromv101->mask[1], eepromv101->mask[2], eepromv101->mask[3]);
		sprintf(tmp_gateway, "%d.%d.%d.%d", eepromv101->gateway[0], eepromv101->gateway[1], eepromv101->gateway[2], eepromv101->gateway[3]);
		sprintf(tmp_srv_ip, "%d.%d.%d.%d", eepromv101->srv_ip[0], eepromv101->srv_ip[1], eepromv101->srv_ip[2], eepromv101->srv_ip[3]);

		printf("dev info: IP address:%s Subnet mask:%s Gateway address:%s Default destination IP:%s  Default destination UDP port:%d   Default external UDP service port:%d\n",
			   tmp_IPv4, tmp_mask, tmp_gateway, tmp_srv_ip, eepromv101->srv_port, eepromv101->local_port);

		/*char tmp_ranger_bias[8] = {0};
		memcpy(tmp_ranger_bias, eepromv101->ranger_bias, sizeof(eepromv101->ranger_bias) - 1);*/
		// Rotation speed ,Motor startup parameter,FIR filter order，Revolution count，Resolution，Upload automatically at startup，Fixed upload，Point smoothing，Trailing-point removal，Record correction coefficient，Network heartbeat，Record IO port polarity
		printf("dev info: Rotation speed:%d Motor startup parameter:%d FIR filter order:%d Revolution count:%d  Resolution:%d   Upload automatically at startup:%d Fixed upload:%d  Point smoothing:%d Trailing-point removal:%d   Network heartbeat:%d  Record IO port polarity:%d\n",
			   eepromv101->RPM, eepromv101->RPM_pulse, eepromv101->fir_filter, eepromv101->cir, eepromv101->with_resample, eepromv101->auto_start,
			   eepromv101->target_fixed, eepromv101->with_smooth, eepromv101->with_filter, eepromv101->net_watchdog, eepromv101->pnp_flags);

		printf("dev info:Smoothing coefficient：%d  Active protection zone：%d  Uploaded data type：%d\n", eepromv101->deshadow, eepromv101->zone_acted, eepromv101->should_post);
		break;
	}

	// Get lidar timestamps for printing (network models return lidar timestamps; serial models use the local receive timestamp)
	case 4:
	{
		//DevTimestamp *devtimestamp = (DevTimestamp *)param;
		//printf("timestamp:lidar_ip:%s lidar_port:%d time:%d delay:%d\n", devtimestamp->ip, devtimestamp->port, devtimestamp->timestamp, devtimestamp->delay);
		break;
	}
	// Print information (can also be written to a log)
	case 8:
	{
		char result[512];
		memcpy(result, param, length);
		printf("info: %s\n", result);
		break;
	}
	case 9:
	{
		char result[512];
		memcpy(result, param, length);
		printf("error: %s\n", result);
		break;
	}
	}
	fflush(stdout);
}
int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Incorrect number of parameters  %d\n usage : ./demo  ../../config/xxx.txt   At least one txt of Lidar\n", argc);
		return ARG_ERROR_NUM;
	}
	BlueSeaLidarSDK *lidarSDK =  BlueSeaLidarSDK::getInstance();
	int lidar_sum = argc - 1;
	for (int i = 0; i < lidar_sum; i++)
	{
		const char *cfg_file_name = argv[i + 1];
		//Add the corresponding lidar using the configuration file path
		int lidarID = lidarSDK->addLidarByPath(cfg_file_name);
		if (!lidarID)
		{
			printf("config file is not exist:%s\n", cfg_file_name);
			return -1;
		}

		//Set the data callback function
		lidarSDK->setCallBackPtr(lidarID, CallBackMsg);

		//Connect the specified lidar and its associated thread pool
		if (!lidarSDK->openDev(lidarID))
		{
			printf("open lidar failed:%d\n", lidarID);
			return -2;
		}
		printf("SDK version:%s\n", lidarSDK->getVersion());
		//sleep(2);

		//Read global lidar parameters
		// EEpromV101 eepromv101;
		// if(lidarSDK->GetDevInfo(lidarID,&eepromv101))
		// 	CallBackMsg(3,&eepromv101,sizeof(EEpromV101));
		
		//Get currently online lidar information (network models only)
		// std::vector<DevConnInfo> dev =lidarSDK->getLidarsList();
		// for(int i=0;i<dev.size();i++)
		// {
		// 	printf("%s\n",dev[i].conn_ip);
		// }
		// Switch protection zones (protection zone models only) 
		//char zone = '9';
		//if (!lidarSDK->ZoneSection(lidarID, zone))
		//{
		//	printf("switch the specified zone failed!\n");
		//}

		//Control lidar startup, stopping, and restarting rotation
		// lidarSDK->ControlDrv(lidarID,6,"LSTARH");
		// sleep(2);
		// lidarSDK->ControlDrv(lidarID,6,"LSTOPH");
		// sleep(2);
		// lidarSDK->ControlDrv(lidarID,6,"LRESTH");
		// sleep(3);

		//Set lidar network parameters (network/protection zone models only; retained after power loss; update the target lidar IP after changing)
		//lidarSDK->SetUDP(lidarID,"192.168.0.140","255.255.255.0","192.168.0.1",6543);

		//Set lidar upload network address parameters (network/protection zone models only; retained after power loss)
		//lidarSDK->SetDST(lidarID,"192.168.0.49",6543);

		 //bool ret,ret2,ret3;
		 ////Change rotation speed (not retained after power loss)
		 //ret=lidarSDK->SetRPM(lidarID,3000);
		 ////Filtering (not retained after power loss)
		 //ret2=lidarSDK->SetDSW(lidarID,true);
		 ////Trailing-point removal (not retained after power loss)
		 //ret3=lidarSDK->SetSMT(lidarID,true);
		 //printf("rpm:%d fitter:%d  smooth:%d\n",ret,ret2,ret3);


		// ////Fixed upload (network/protection zone models only; retained after power loss)
		// ret = lidarSDK->SetTFX(lidarID,false);
		// //Data upload type (network/protection zone models only; retained after power loss)
		// ret2=lidarSDK->SetPST(lidarID,3);
		// //Set lidar ID (network/protection zone models only; retained after power loss)
		// ret3=lidarSDK->SetDID(lidarID,999);
		// printf("tfx:%d post:%d num:%d\n",ret,ret2,ret3);

		//Set lidar NTP upload information
		// lidarSDK->SetNTP(lidarID,"192.168.0.111",133,1);

	}
	while(1)
	{
		//RunConfig* one = BlueSeaLidarSDK::BlueSeaLidarSDK::getInstance()->getLidar(1);
		//RunConfig* two = BlueSeaLidarSDK::BlueSeaLidarSDK::getInstance()->getLidar(2);
		//Keep the process running
		msleep(100);
	}
	return 0;
}
