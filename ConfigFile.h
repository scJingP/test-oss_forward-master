//----------------------------------------------------------------------------
// 程序名称：   ConfigFile.h
// 程序说明：   类 ConfigFile 的定义
// 程序作者：   
// 程序版本：   1.0
// 开始日期：   1999-01-10
//----------------------------------------------------------------------------

#ifndef __CONFIGFILE_H__
#define __CONFIGFILE_H__

#include <fstream>
#include <string>
#include <list>
#include <iostream>
using namespace std;

#include <string.h>
#include <stdio.h>

#define MAX_LINE_LENGTH 300

//----------------------------------------------------------------------------
// 类说明：用于对配置文件进行操作，对于给定的节名和变量名，获得变量的值。
//----------------------------------------------------------------------------
class CConfigFile
{
//构造函数和析构函数
private:    //私有
   
protected:  //保护
   
public:     //公有
    CConfigFile();
    CConfigFile(char *pConfigFileName);
    ~CConfigFile();
   

//属性
private:    //私有
    fstream m_CfgFile;
    short     m_IsOpen;  //文件打开状态
    enum
    {
      IS_COMMENT ,    //是注释
      IS_SESSION ,    //是节名
      IS_VALUE,       //是变量的值
      NOT_FOUND       //没有找到指定的变量名
    };
   
protected:  //保护
   
public:     //公有
   

//服务
private:    //私有
    //查找一个指定的节名
    short  GetSession(char *pStr,char *SessionName);
    //获得变量的值
    short GetContent(char *pStr,char *ValueName,char *RetStr);
   
protected:  //保护
   
public:     //公有
    //打开一个指定的配置文件
    short  Open(char *ConfigFileName);
    //关闭配置文件
    void   Close();
    //根据给定的节名和变量名，从文件中读出变量的值
    short  GetValue(char *Session,char *ValueName,char *RetStr);
    //返回文件打开状态
    short  IsOpen() { return m_IsOpen; };
	//获取节的全部值
	short  GetValueForSession(char *Session, list<string> &lst);
};

#endif //__CONFIGFILE_H__