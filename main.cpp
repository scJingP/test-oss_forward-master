#include "stdafx.h"

#include "

#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern "C"	// c����ͷ�ļ�
{
	#include "oss_sdk.h"
}

#include <iostream>
#include <list>
using namespace std;

#include "wp_sdk.h"
#include "ConfigFile.h"	//���������ļ�

//�����
#define SendBufferLength	1024 * 1024 * 1
#define BufferLength		1024 * 1024 * 2

// ȫ�ֱ���
//FILE* recvFile = 0;
int Totallen = 0 ;
int myRecvDataLen = 0;
FILE *file;
list<string> lstDateSection;
unsigned char buffer[BufferLength];

//��ʼ��lstDateSection���γ�ʱ����ϣ�
short InitDataSection(){
	CConfigFile config("config.txt");
	short i = config.GetValueForSession("date", lstDateSection);
	if(i == 0){
		return 0;
	}
	return -1;
}

// ���� 0 ������ʱ���	-1 ������ʱ���		dateSectionName ����ÿ��ʱ���ȷ��һ����ǩ����ǰʱ��ε���ʼʱ�䣩��Ϊ�ļ�����һ����
// �ϴ��������Ƶ��ļ����ƶ��壺ѧУ���_���ұ��_20160928_ʱ���
short CheckDate(list<string> lstDate, string date, string& dateSectionName){
	list<string>::iterator itor;
	for (itor = lstDate.begin(); itor != lstDate.end(); itor++) {
		string tmpDate = string(*itor);
		string dateFront = tmpDate.substr(0, 5);
		string dateBack = tmpDate.substr(6, 5);

		if(date.compare(dateFront) >= 0 && date.compare(dateBack) <= 0){
			dateSectionName = dateFront;
		}
    }
	//�ж��ļ����Ƿ�Ϊ�գ��Ƿ���ʱ��Σ�
	if( dateSectionName.compare("") != 0){
		dateSectionName.replace(2,1,"");
		return 0;
	}
	return -1;
}

void WriteDataToFile(FILE *writeFile, unsigned char *buffer, int lenR){        
	//��ʼ������
	int lenW = 0;   
	if(writeFile == NULL){  
		printf("file is null\n");
		exit(0);        
	}
	printf("----------------\n");
	printf("len %d\n", lenR);
	if((lenW = fwrite(buffer,sizeof(char), lenR, writeFile)) != lenR){            
		perror("fwrite");                       
		fclose(writeFile);            
		exit(0);        
	}
}

bool Callbackdata(int index,int type, unsigned char * data, int len, int param1, int param2,STREAMTYPE streamtype,unsigned long starttime)
{   
	if (!data || index >= MAX_CAMERA)
		return false;
	Totallen+= len;
	if (DATA_TYPE_HD_IFRAME == type){
		//printf("DATA_TYPE_HD_IFRAME11111 streamtype =%d starttime =%lu\n",streamtype,starttime);
		//my method
		//WriteDataToFile(file, data, len);
		printf("--------------new--------------");
		printf("accepted data curr pthread %u\n",pthread_self());
		printf("len : %d   myRecvDataLen : %d	%fMB\n",len, myRecvDataLen,myRecvDataLen/1024.0/1024.0);
		//��ȡϵͳʱ��
		time_t t; 
		struct tm * a; 
		time(&t); 
		a=localtime(&t); 
		printf("%4d-%02d-%02d %02d:%02d:%02d\n",1900+a->tm_year,1+a->tm_mon,a->tm_mday,a->tm_hour,a->tm_min,a->tm_sec); 
		char *tmpCurrDate = (char *)malloc(5);
		sprintf(tmpCurrDate, "%02d:%02d", a->tm_hour,a->tm_min);
		string currDate = string(tmpCurrDate);
		//�ж��Ƿ���ʱ����ڣ�������򿽱����ݵ�������
		char _fileNameHead[9];
		sprintf(_fileNameHead, "%4d%02d%02d_", 1900+a->tm_year,1+a->tm_mon,a->tm_mday);
		string fileName(_fileNameHead);
		string fileNamePart;
		if(CheckDate(lstDateSection, currDate, fileNamePart) == 0){
			memcpy(buffer+myRecvDataLen, data, len*sizeof(unsigned char));
			myRecvDataLen = myRecvDataLen + len;
			printf("copy data, myRecvDataLen : %d\n", myRecvDataLen);
		}
		//��⻺�����ݳ����Ƿ�����ϴ���������
		if(myRecvDataLen >= SendBufferLength){
			fileName.append(fileNamePart);
			fileName.append("_mytest.mp4");
			printf("fileName: %s \n", fileName.c_str());
			oss_sdk_append_object_from_buffer(fileName.c_str(), buffer, myRecvDataLen);
			memset(buffer, 0, myRecvDataLen);
			myRecvDataLen = 0;
		}
		
	}else if(DATA_TYPE_HD_PFRAME == type){
		//	    printf("DATA_TYPE_HD_PFRAME22222\n");
	}else if(DATA_TYPE_CIF_IFRAME == type){
		//	    printf("DATA_TYPE_CIF_IFRAME3333\n");
	}else if(DATA_TYPE_CIF_PFRAME == type){
		//	    printf("DATA_TYPE_CIF_PFRAME4444\n");
	}
	return true;
}

bool CallbackEvent(int index, int type)
{
	if (IPC_MOTION == type) {
		printf("IPC_MOTION\n");

	} else if (IPC_NO_ALARM == type) {
		//	printf("L&R_ALARM\n");

	} else if (IPC_LEFT_ALARM == type) {
		printf("LEFT_ALARM\n");

	} else if (IPC_RIGHT_ALARM == type) {
		printf("RIGHT_ALARM\n");
	} else if (IPC_DISCONNECT == type) {
		printf("IPC_DISCONNECT\n");
	}

	return true;
}

void CallBackSearchCam(char *ethname, char *ip, char *mac){
	printf("asdf");
	printf("ip :%s/n",ip);
}


//-----------------------------------------------------------------------------------------------		
int main()
{
	/*
	printf("Hello\n");
	file = fopen("/home/server/ftp/tmp.mp4","w");
	if(file == NULL){
	perror("file fopen error");
	}
	*/
	
	InitDataSection();
	//��ʼ�� ����oss_sdk
	oss_sdk_init();

	DVR_LOGININFO LoginInfo;
	memset(&LoginInfo,0,sizeof(PDVR_LOGININFO));
	DVR_DEVICEINFO DeviceInfo;
	memset(&DeviceInfo,0,sizeof(PDVR_DEVICEINFO));
	strcpy(LoginInfo.pszDvrIP,"192.168.1.202");
	WP_IPC_Init();
	int index = WP_IPC_Login(&LoginInfo,&DeviceInfo);
	if(index < 0){
		printf("connect error index=%d\n",index);
		return -1;
	}

	WP_IPC_OpenStream(index);

	//WP_IPC_Search_Camera(CallBackSearchCam);
	WP_IPC_GetData(Callbackdata);
	WP_IPC_GetEventData(CallbackEvent);
	//  WP_IPC_PTZControl(0,0,LEFT,0,0,0);
	//  sleep(10);
	//  WP_IPC_PTZControl(0,0,PANSTOP,0,0,0);
	//  sleep(10);
	//  WP_IPC_PTZControl(0,0,RIGHT,0,0,0);
	//   sleep(10);
	//  WP_IPC_PTZControl(0,0,PANSTOP,0,0,0);
	//  sleep(10);
	//  WP_IPC_PTZControl(0,0,PTZ_PAN_SPEED,40,0,0);//(1-200)
	//  sleep(10);
	//  WP_IPC_PTZControl(0,0,PTZ_TILT_SPEED,25,0,0);//(1-48)
	char Osd[64];
	strcpy(Osd,"wapa125");
	WP_IPC_SET_ChannelName(0,Osd,sizeof(Osd));
	char Time[64];
	strcpy(Time,"20160909101920");
	WP_IPC_SET_Time(0,Time,sizeof(Time));
	WP_IPC_OSDControl(0,OSD,1);
	WP_IPC_OSDControl(0,TIMETEXT,0);
	// my test
	WP_IPC_SET_AutoReduce(index, 0);
	WP_IPC_SET_ShutterSpeed(index, 8);
	// -----------------------------
	//  WP_IPC_SetAlarm(index,true,0,3);
	//  WP_IPC_SET_InvertImage(index);
	//  WP_IPC_SET_RGB(index,160,162);
	//  WP_IPC_RebootCamera(index);

	//  sleep(10);//���е�������ʱ��
	while(1){
		//sleep(1);
	}
	WP_IPC_CloseStream(index);
	// sleep(5);
	//  IPC_OpenStream(index,CTL_STREAM_S);
	//  sleep(10);//���е�������ʱ��
	//  IPC_CloseStream(index,CTL_STREAM_S);
	WP_IPC_Logout(index);
	WP_IPC_Cleanup();
	printf("Totallen =%d\n",Totallen);

	//���� ����oss_sdk
	oss_sdk_destroy();
	return 0;
}
