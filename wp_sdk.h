#ifndef _WP_SDK_H_
#define _WP_SDK_H_
#include <stddef.h>

//OpenStream & CloseStream params of 'type'
#define CTL_STREAM_BS     0
#define CTL_STREAM_S      1
#define CTL_STREAM_B      2 

//DataCallBack params of 'type'
#define DATA_TYPE_AUDIO						2				//Audio data g.721
#define	DATA_TYPE_HD_IFRAME					3				//Encoded HD frame type is I
#define	DATA_TYPE_HD_PFRAME					4				//Encoded HD frame type is P
#define DATA_TYPE_CIF_IFRAME				5				//Encoded CIF frame type is I
#define	DATA_TYPE_CIF_PFRAME				6				//Encoded CIF frame type is P

#define MAX_CAMERA				128

#define CRUISEMAX			4
#define PRESETS_PER_PATH			128
#define PRESETMAX			PRESETS_PER_PATH*CRUISEMAX
//the data format in decoder
typedef enum{
	MPEG4 = 0,
	H264 = 264,
}STREAMTYPE;

typedef enum{
       OSD = 1,//whether to show osd or not    
       TIMETEXT,
       CHANNELNAME,
       TIMETYPE,
       DATETYPE,  
}OSDCONTROL;
//the state of camera
typedef enum{
	IPC_CONNECT = 0,										//connect to camera£¬if param1 equel to param2 and equel 264£¬that the camera is twenty Thousand pixel 
	IPC_DISCONNECT,											//disconnect camera
	IPC_MOTION,												//move sense
	IPC_NO_ALARM,
	IPC_LEFT_ALARM,
	IPC_RIGHT_ALARM,
	IPC_LR_ALARM,
}IPC_STATE;

//video format
typedef enum{
	PAL = 0,
	NTSC
}TVFORMAT;

typedef struct _EXTPTZCOMM{        
	int                protocol;        
	int                port;        
	int                baudrate;        
	int                baseAddr;        
	int                panSpeed;        
	int                titlSpeed;        
	int                reserve1;        
	int                reserve2;
}EXTPTZCOMM;

//for high speed dome basic control and set info
typedef enum{
	UP = 0,										//PTZ_NormalControl		up
	DOWN,										//PTZ_NormalControl		down
	LEFT,										//PTZ_NormalControl		left
	RIGHT,										//PTZ_NormalControl		right
	FOCUSFAR,									//PTZ_NormalControl		focus far
	FOCUSNEAR,									//PTZ_NormalControl		focus near
	ZOOMWIDE,									//PTZ_NormalControl		zoom wide
	ZOOMTELE,									//PTZ_NormalControl		zoom telescope
	IRISCLOSE,									//PTZ_NormalControl		iris
	IRISOPEN,									//PTZ_NormalControl		open iris
	PTZ_SETPRESET = 19,								//PTZ_PresetControl		add preset point
	PTZ_CLEARPRESET,								//PTZ_PresetControl		delete preset point
	PANSTOP=23,									//PTZ_NormalControl		stop move left or right
	TILTSTOP,									//PTZ_NormalControl		stop move up or down 
	ZOOMSTOP,									//PTZ_NormalControl		stop zoom 
	FOCUSSTOP,									//PTZ_NormalControl		stop focus
	IRISSTOP,									//PTZ_NormalControl		stop iris
	AUTOFOCUS = 50,								//PTZ_NormalControl		auto focus
	MANUALFOCUS = 51,								//PTZ_NormalControl		manual focus
	PTZ_PAN_SPEED = 53,								//PTZ_NormalControl		set the move speed of horizontal
	PTZ_TILT_SPEED,								//PTZ_NormalControl		set the move speed of vertical
	PTZ_SETZEROPOS=57,								//PTZ_NormalControl             set initial position
	PTZ_CRUISE_ON = 80,								//PTZ_PresetControl		start cruising
	PTZ_CRUISE_OFF,								//PTZ_PresetControl		stop cruising
	ADD_SCAN_PRESET = 89,
	DEL_SCAN_PRESET,
	START_SCAN,
	STOP_SCAN,
}NETOPERATE;


typedef struct _CRUISEINDEX{
	unsigned short		id;
	unsigned short		interval;
}CRUISEINDEX;
typedef struct _CRUISESET{
	int			number:14;
	int			type:2;
	int			reserve:16;
	CRUISEINDEX	index[PRESETS_PER_PATH];
}CRUISESET;

typedef struct _PRESET{
	unsigned short  ID;
	char			name[26];
}PRESET;

typedef struct _PTZINFO{
	char			bExtPTZEnable;
	char			reserve3;
	char			bAutoFocus;
	char			bEnable;
	int				presetnum;
	int				interval;	
	int				protocol;     
	int				port;        
	int				baudrate;
	int				baseAddr;	
    char			bCruise;
	char			CruisePath;    
	unsigned short			reserve0;
	int				reserve1;
	int 			panSpeed;
	int 			tiltSpeed;
	PRESET  		PreSet[PRESETMAX];
	CRUISESET		Cruise[CRUISEMAX];
}PTZINFO;
typedef enum{
	EXT_GET_INFO = 0,
	EXT_SET_TIME = 3,
	EXT_START_CRUISE = 4,
	EXT_STOP_CRUISE = 5,
	EXT_GOTO_PRESET = 21,
	EXT_ADD_PRESET = 83,
	EXT_DEL_PRESET = 84,
}EXTOPT;

#define DVR_IP_LEN 64
#define DVR_USR_LEN 16
#define DVR_PASS_LEN 16
#define DVR_SERIALNO_LEN 16
typedef struct
{
    long lLoginMode;                             
    char pszDvrIP[DVR_IP_LEN];                     
    unsigned short usDvrPort;                      
    char pszDvrUserName[DVR_USR_LEN];               
    char pszDvrPassword[DVR_PASS_LEN];              
    char pszSSIP[DVR_IP_LEN];                       
    unsigned short ulSSPort;                       
} DVR_LOGININFO, *PDVR_LOGININFO;

typedef struct
{
    unsigned char szSerialNumber[DVR_SERIALNO_LEN]; 
    unsigned char ucAlarmInPortNum;                
    unsigned char ucAlarmOutPortNum;               
    unsigned char ucDiskNum;                       
    unsigned char ucDVRType;                      
    unsigned char ucChanNum;                        
    unsigned char ucStartChan;                      
}DVR_DEVICEINFO,*PDVR_DEVICEINFO;
//for callback
typedef bool (*DataCallBack)(int index,int type, unsigned char *pData, int len , int param1, int param2,STREAMTYPE streamtype,unsigned long starttime);	
//Data callback function. index:camera index. type:frame type. pData:receive data. len:the length of pData. param1:width param2:height
typedef bool (*EventCallBack)(int index, int type); 
//Event callback function. index :camera index. type : event type defined in enum IPC_STATE
typedef void (*SearchCamCallBack)(char *ethname, char *ip, char *mac);
//Search camera callback function. ethname: adapter's name. ip: camera's ip. mac: camera's mac.

int WP_IPC_GetData(DataCallBack fData);                  //Set the data callback function
int WP_IPC_GetEventData(EventCallBack fEvent);           //Set the event callback function

bool WP_IPC_Init();
bool WP_IPC_Cleanup();
//for connect
int WP_IPC_Login(PDVR_LOGININFO pLoginInfo, PDVR_DEVICEINFO lpDeviceInfo);  
//return -1 if error occur,return the camera index (0-127) after succeed		
bool WP_IPC_Logout(int index);                                              //Disconnect camera. index:index of camera

bool WP_IPC_PTZControl(long lUserHandle, long lChannel , unsigned long ulPTZCommand , unsigned long ulSpeed ,unsigned long ulPro ,unsigned long ulAddr);
//lUserHandle : index of camera     ulPTZCommand :the value within the UNIT NETOPERATE    ulSpeed:used when ulPTZComand is PTZ_PAN_SPEED  or PTZ_TILT_SPEED
//other param isn't used;recommend to use WP_IPC_PTZ_NormalControl or WP_IPC_PTZ_SetInfo instead;
bool WP_IPC_OpenStream(long lUserHandle, int type = CTL_STREAM_BS);  
//lUserHandle : index of camera ,type:Big or small stream defined in line 6-8
bool WP_IPC_CloseStream(long lUserHandle, int type = CTL_STREAM_BS);  
//lUserHandle : index of camera
//for HSD 
int WP_IPC_PTZ_NormalControl(int index, int oper, int param = 0);//Control HSD. index:index of HSD. oper:the value within the UNIT NETOPERATE
int WP_IPC_PTZ_PresetControl(int index, int operate,int presetID = 0, int cruiseID = 0,int szSec = 0, char* prename = 0);	
//Setup the index HSD(High Speed Dome) camera's presets and cruise information. 
//presetID is the preset ID, (0-127)
//cruiseID is the cruiseID, (0-3)
//szSec is the interval, 
//prename is the preset name.
int WP_IPC_GetPTZInfo(int index, PTZINFO *PTZInfo);
////Get the preset info of HSD.
//for control camera
int WP_IPC_SET_VideoFormat(int index, TVFORMAT tvf);
int WP_IPC_SET_RGB(int index, int r, int b);                               //Set RGB of video. r and b:the range from 120 to 200.
int WP_IPC_SET_Contrast(int index, int contrast);                          //Set contrast of video. contrast:range from -128 to 127.
int WP_IPC_SET_EdgeGain(int index, int gain);                              //Set edge gain.  gain:0 is OF,1 is ON.
int WP_IPC_SET_ManualReduce(int index, int reduce);						//Set manual reduce. reduce:range from 1 to 4, 1:FULL; 2:1/2 rate; 3:1/3 rate; 4:1/4 rate.
int WP_IPC_SET_AutoReduce(int index, int reduce);							//Set auto reduce frame. reduce: 0 is OFF,1 is ON.
int WP_IPC_SET_Exposure(int index, int oper);								//Set the mode of exposure.  oper: 0:Brightest part priority; 1:Darker part priority; 2:Brighter part priority; 3:Darkest part priority.
int WP_IPC_SET_Resolution(int index, int resolution);						//Set resolution. resolution: 2: 4:3; 4: 16:9.(Just 720P camera has this function).
int WP_IPC_SET_WhiteBalance(int index, int mod);                           //Set white balance. mod:range from 1 to 6, 1:auto white balance; 2:outdoor; 3:flourescence; 4:manual white balance; 5:indoor; 6:tungsten.
int WP_IPC_SET_IRCard(int index, int oper);								//Set IR clip switch time. oper:value range from 0 to 2, 0:just use hardware for IR card; 1:software and hardware to IR card; 2:just software for IR card.
int WP_IPC_SET_ShutterSpeed(int index, int value);                         //Set the speed for shutter. value:is 3-8, 3:1 times; 4:1/2 times 5:1/4 times; 6:1/8 times; 7:1/16 times; 8:1/20 times.
int WP_IPC_SET_InvertImage(int index);										//Invert the image of the camera.
int WP_IPC_SET_BitRate(int index, int value);								//Set the bit rate of the camera.
int WP_IPC_RebootCamera(int index);										//Reboot camera.
int WP_IPC_SET_ConnectMode(int index, int value);							
int WP_IPC_SET_EncodeMode(int index, int value);

//for Alarm
int WP_IPC_SET_Alarm(int index, bool bAlarm, int alarmType, int alarmPos);	//Set the alarm. 
//bAlarm:true is open alarm; false is close alarm.
//alarmType:0 is short alarm; 1 is cut alarm.  
//alarmPos:1 is left; 2 is right; 3 is right and left. 	

//for osd&time
int WP_IPC_SET_ChannelName(int index,char *Osdtext,int len);
//Set channel name len:length of Osdtext
int WP_IPC_SET_Time(int index,char *timetext,int len);
//Set camera time timetext has two type?20130326084630 yyyy-MM-dd-HH-mm-ss or 032608462013.30 MM-dd-HH-mm-yyyy-ss len :length of timetext
int WP_IPC_OSDControl(int index,OSDCONTROL osdctl,int data);
//Control the Osd text how to show it 
//typedef enum{
//       OSD = 1,//whether to show osd or not ;data:1 show,  0 no
//       TIMETEXT,//whether to show time or not;data:1 show,0 no
//       CHANNELNAME,//whether to show channel name or not;data:1 show,0 no
//       TIMETYPE,  //timetype,data:1 24-hour system    0  12-hour system 
//       DATETYPE,  //datetype,data:2  dd-MM-yyyy 1 MM-dd-yyyy  0 yyyy-MM-dd
//}OSDCONTROL;
int WP_IPC_GET_CamAngle(int index, int *horizontal, int *vertical);
//Get camera angle,return WAPA_IPC_NOERROR after succeed,return WAPA_IPC_NETWORK_SEND_ERROR or WAPA_IPC_NETWORK_CONNECT_FAIL after failed.
//horizontal:camera Horizontal angle.
//vertical:Vertical angle.
int WP_IPC_InitPTZ(int index);
//Init PTZ position
int WP_IPC_InitAngle(int index);
//Init ptz angle

//set ip via control port. index: camera's index. ip: destination IP that need to be set, such as "192.168.1.200".
int WP_IPC_SET_IP_Ctrl(int index, char *ip);

//set mask via control port. index: camera's index. mask: destination mask that need to be set, such as "225.255.255.0"
int WP_IPC_SET_Mask_Ctrl(int index, char *mask);

// set gateway via control port. index: camera's index. gateway: destination gateway that need to be set, 
// such as "192.168.1.1".
int WP_IPC_SET_Gateway_Ctrl(int index, char *gateway);

void WP_IPC_Search_Camera(SearchCamCallBack search);

// set IP via broadcast. devname: adapter's name, such as "eth0". The same below.
// destmac: destination camera's mac, it's length should be 6. The same below.
// ip: IP to be set, it's length should be 4.
void WP_IPC_SET_IP_ARP(char *devname, unsigned char *destmac, unsigned char *ip);

// set mask via broadcast. mask: mask to be set, it's length should be 4.
void WP_IPC_SET_Mask_ARP(char *devname, unsigned char *destmac, unsigned char *mask);

// set gateway via broadcast. gateway: gateway to be set, it's length should be 4
void WP_IPC_SET_Gateway_ARP(char *devname, unsigned char *destmac, unsigned char *gateway);
#endif			  																	
