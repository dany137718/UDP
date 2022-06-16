#include "UtilityFunc.h"

#define DELTA_EPOCH_IN_MICROSECS 11644473600000000

CUtilityFunc * utilityFuncInstance;

CUtilityFunc::CUtilityFunc(void)
{
#ifdef SAVE_HEALTH_ISSUE_LOG
	sprintf_s(healthIssueLog_subFolderPath , _ArraySizeMacro_(healthIssueLog_subFolderPath) , "%s/HealthIssue" , constantParamIns->ALL_SYSTEM_LOG_FOLDER_PATH);
	_mkdir(healthIssueLog_subFolderPath);
#endif
	
	isGeneralLogCreateBusy = false;
	strcpy_s(GENERAL_LOG_TYPE_ENUM_STRING[DEBUG_LOG_TYPE] , _ArraySizeMacro_(GENERAL_LOG_TYPE_ENUM_STRING[DEBUG_LOG_TYPE]) , "DEBUG");
	strcpy_s(GENERAL_LOG_TYPE_ENUM_STRING[INFO_LOG_TYPE] , _ArraySizeMacro_(GENERAL_LOG_TYPE_ENUM_STRING[INFO_LOG_TYPE]) ,  "INFO");
	strcpy_s(GENERAL_LOG_TYPE_ENUM_STRING[WARN_LOG_TYPE] , _ArraySizeMacro_(GENERAL_LOG_TYPE_ENUM_STRING[WARN_LOG_TYPE]) , "WARN");
	strcpy_s(GENERAL_LOG_TYPE_ENUM_STRING[ERROR_LOG_TYPE] , _ArraySizeMacro_(GENERAL_LOG_TYPE_ENUM_STRING[ERROR_LOG_TYPE]) , "ERROR");
	strcpy_s(GENERAL_LOG_TYPE_ENUM_STRING[FATAL_LOG_TYPE] , _ArraySizeMacro_(GENERAL_LOG_TYPE_ENUM_STRING[FATAL_LOG_TYPE]) , "FATAL");

	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	int tolo_khoy_table_copy[13] = { 0 , 8 , 8 , 7 , 7 , 7 , 6 , 7 , 7 , 7 , 7 , 7 , 8 };
	int qoroob_zabol_table_copy[13] = { 0 , 16 , 17 , 17 , 18 , 19 , 19 , 19 , 19 , 18 , 17 , 16 , 16 };

	for (int i = 0; i < 13; i++)
	{
		tolo_khoy_table[i] = tolo_khoy_table_copy[i];
		qoroob_zabol_table[i] = qoroob_zabol_table_copy[i];
	}
}

CUtilityFunc::~CUtilityFunc(void)
{

}

#ifdef SAVE_HEALTH_ISSUE_LOG
void CUtilityFunc::activeHealthIssueLog(char * in_logStr , ACTIVE_HEALTH_ISSUE_LOG_ACTION_ENUM in_action)
{
	char th_filePath[1000];
	sprintf_s(th_filePath , _ArraySizeMacro_(th_filePath) , "%s/System Log/HealthIssue/%s_%s.txt" ,
		constantParamIns->BASE_RAM_DISK_FOLDER , constantParamIns->mySelfProcessName , in_logStr);

	if(in_action == ACTIVE_HEALTH_ISSUE_LOG_ERASE)
	{
		remove(th_filePath);
		return;
	}
	else
		generalLogCreate(FATAL_LOG_TYPE , (char *)"HealthIssueLog" , in_logStr);

	char th_subFolderPath[100];
	sprintf_s(th_subFolderPath , _ArraySizeMacro_(th_subFolderPath) , "%s/System Log" , constantParamIns->BASE_RAM_DISK_FOLDER);
	_mkdir(th_subFolderPath);

	sprintf_s(th_subFolderPath , _ArraySizeMacro_(th_subFolderPath) , "%s/System Log/HealthIssue" , constantParamIns->BASE_RAM_DISK_FOLDER);
	_mkdir(th_subFolderPath);

	if(in_action == ACTIVE_HEALTH_ISSUE_LOG_CREATE)
	{
		if( isFileHaveNullCharacter(th_filePath) == true )
		{
			Sleep(100);
			remove(th_filePath);
			Sleep(100);
		}

		ofstream output_Log;
		output_Log.open(th_filePath , ios_base::out | ios_base::app);
		if(output_Log)
		{
			SYSTEMTIME crntTime;
			GetLocalTime(& crntTime);

			char outPutErrLogStr[2000];

			sprintf_s(outPutErrLogStr , _ArraySizeMacro_(outPutErrLogStr) , "%02d:%02d:%02d,%s_%s" ,
				crntTime.wHour , crntTime.wMinute , crntTime.wSecond , constantParamIns->mySelfProcessName , in_logStr);

			output_Log<<outPutErrLogStr<<endl;

			output_Log.flush();
			output_Log.close();
		}

		return;
	}
}

void CUtilityFunc::healthIssueLog(char * in_logStr)
{
	SYSTEMTIME crntTime;
	GetLocalTime(& crntTime);

	char th_folderPath[500] , th_filePath[500];
	sprintf_s(th_folderPath , _ArraySizeMacro_(th_folderPath) , "%s/%i-%i-%i" , healthIssueLog_subFolderPath , 
					crntTime.wYear , crntTime.wMonth , crntTime.wDay);

	_mkdir(th_folderPath);

	sprintf_s(th_filePath , _ArraySizeMacro_(th_filePath) , "%s/%s.txt" , th_folderPath , constantParamIns->mySelfProcessName);

	if( isFileHaveNullCharacter(th_filePath) == true )
	{
		Sleep(100);
		remove(th_filePath);
		Sleep(100);
	}

	ofstream output_Log;
	output_Log.open(th_filePath , ios_base::out | ios_base::app);
	if(output_Log)
	{
		char outPutErrLogStr[2000];

		sprintf_s(outPutErrLogStr , _ArraySizeMacro_(outPutErrLogStr) , "%02d:%02d:%02d,%s_%s" ,
			crntTime.wHour , crntTime.wMinute , crntTime.wSecond , constantParamIns->mySelfProcessName , in_logStr);

		output_Log<<outPutErrLogStr<<endl;

		output_Log.flush();
		output_Log.close();
	}
}
#endif

void CUtilityFunc::socketCreateLog(unsigned short in_portNumber , char * in_funcName)
{
	char th_commentStr[1000];
	sprintf_s(th_commentStr , _ArraySizeMacro_(th_commentStr) , "Port = %i , funcName = %s" , (int)(in_portNumber) , in_funcName);
	generalLogCreate(DEBUG_LOG_TYPE , (char *) "socketFunctionDebugLog" , th_commentStr);
}

void CUtilityFunc::terminateServerSocketThreadLog(unsigned short in_portNumber , int in_typeOfTerminate)
{
	char th_commentStr[1000];
	sprintf_s(th_commentStr , _ArraySizeMacro_(th_commentStr) , "Port = %i , TerminateType = %i" , (int)(in_portNumber) , in_typeOfTerminate);
	generalLogCreate(DEBUG_LOG_TYPE , (char *)"terminateServerSocketThread" , th_commentStr);
}

void CUtilityFunc::generalLogCreate(GENERAL_LOG_TYPE_ENUM in_generalLogTypeEnum , char * in_functionality , char * in_comment)
{
	int th_whileCounter = 0;
	while(isGeneralLogCreateBusy == true && (th_whileCounter++) < 20)
		Sleep(1);

	isGeneralLogCreateBusy = true;
	//////////////////////////////////////////////////

	SYSTEMTIME crntTime;
	GetLocalTime(& crntTime);

	char th_logFilePath[200];
	sprintf_s(th_logFilePath , _ArraySizeMacro_(th_logFilePath) , "%s/generalLog-%04i-%02i-%02i.txt" , constantParamIns->generalLogFolderPath ,
					crntTime.wYear , crntTime.wMonth , crntTime.wDay);

	if( isFileHaveNullCharacter(th_logFilePath) == true )
	{
		Sleep(100);
		remove(th_logFilePath);
		Sleep(100);
	}

	//////////////////////////////////////////////////

	char outPutLogStr[1000];
	sprintf_s(outPutLogStr , _ArraySizeMacro_(outPutLogStr) , "%04i-%02i-%02i %02i:%02i:%02i|%s|%s|%s" , 
		crntTime.wYear , crntTime.wMonth , crntTime.wDay , crntTime.wHour , crntTime.wMinute , crntTime.wSecond , 
		GENERAL_LOG_TYPE_ENUM_STRING[in_generalLogTypeEnum] , in_functionality , in_comment);

	ofstream output_LogFile;
	output_LogFile.open(th_logFilePath , ios_base::out | ios_base::app);
	if(output_LogFile)
	{
		output_LogFile<<outPutLogStr<<endl;

		output_LogFile.flush();
		output_LogFile.close();
	}

	//////////////////////////////////////////////////
	isGeneralLogCreateBusy = false;
}

void CUtilityFunc::softResetAndLog(GENERAL_LOG_TYPE_ENUM in_generalLogTypeEnum, char * in_logStr)
{
	generalLogCreate(in_generalLogTypeEnum, (char *)"SOFT RESET", in_logStr);

	Sleep(1000);

#ifdef __LINUX
	system("shutdown -r now");
#else
	system("shutdown /r /f /t 1");
#endif

	Sleep(5000);
}

void CUtilityFunc::exitAndLog(GENERAL_LOG_TYPE_ENUM in_generalLogTypeEnum , char * in_logStr)
{
	generalLogCreate(in_generalLogTypeEnum , (char *) "EXIT PROGRAM" , in_logStr);

	Sleep(2000);

#ifndef __LINUX
	HANDLE HHH = GetCurrentProcess();
	TerminateProcess(HHH , 0);
#endif

	exit(0);
}

bool CUtilityFunc::isFileHaveNullCharacter(char * in_fileName)
{
	ifstream th_input_arguman;
	th_input_arguman.open(in_fileName , ios_base::in);
	if( th_input_arguman )
	{
		char th_ch;
		th_input_arguman.get(th_ch);
		th_input_arguman.close();

		if(th_ch > 7)
			return false;
		else
			return true;
	}
	else
	{
		return false;
	}

	return false;
}

void CUtilityFunc::dirDirectory(const char * in_directoryPath , vector<string> & dirResultFullPath , vector<string> & dirResultName)
{
	struct dirent * dp;

	DIR * dir = opendir(in_directoryPath);

	if (dir == NULL)
		return;

	while ( (dp = readdir(dir)) != NULL )
	{
		if(!strcmp(dp->d_name , ".") || !strcmp(dp->d_name , ".."))
		{
			continue;
		}

		string th_string = string(in_directoryPath) + "/" + string(dp->d_name);
		dirResultFullPath.push_back(th_string);

		dirResultName.push_back(dp->d_name);
	}

	closedir(dir);
}

bool CUtilityFunc::deleteDirectory(char * in_directoryPath)
{
	struct dirent * dp;

	DIR * dir = opendir(in_directoryPath);

	if (dir == NULL)
		return true;

	while( (dp = readdir(dir)) != NULL )
	{
		if(!strcmp(dp->d_name , ".") || !strcmp(dp->d_name , ".."))
		{
			continue;
		}

		int newObjectName_size = (int)(strlen(in_directoryPath) + strlen(dp->d_name) + 10);
		char * newObjectName = new char[newObjectName_size];
		sprintf_s(newObjectName , newObjectName_size , "%s/%s" , in_directoryPath , dp->d_name);

		if( remove(newObjectName) != 0)
		{
			if( deleteDirectory(newObjectName) == false)
			{
				delete [] newObjectName;
				closedir(dir);
				return false;
			}
		}

		delete [] newObjectName;
	}

	closedir(dir);

	if(_rmdir(in_directoryPath) != 0)
	{
		return false;
	}

	return true;
}

long long CUtilityFunc::getFakeEpochTime()
{
	long long tmpres = 0;

#ifdef __LINUX

	struct timeval now;
	gettimeofday(&now, NULL);
	long long l = 1000000;
	tmpres = now.tv_sec * l  + now.tv_usec;

 #else

	FILETIME ft;
	ZeroMemory(&ft,sizeof(ft));

	GetSystemTimeAsFileTime(&ft);

	tmpres = (long long)(ft.dwHighDateTime) << 32;
	tmpres |= ft.dwLowDateTime;

	tmpres /= 10;
	tmpres -= DELTA_EPOCH_IN_MICROSECS;

#endif

	return tmpres;
}

#ifndef __LINUX
long long CUtilityFunc::getFakeEpochTimeFromFileTime(FILETIME ft)
{
	long long tmpres = 0;

	tmpres = (long long)(ft.dwHighDateTime) << 32;
	tmpres |= ft.dwLowDateTime;

	tmpres /= 10;
	tmpres -= DELTA_EPOCH_IN_MICROSECS;

	return tmpres;
}
#endif

SYSTEMTIME CUtilityFunc::convertEpochTimeToSystemTime(long long in_epochTime)
{
	SYSTEMTIME localTimeForReturn;
	long long in_epochTime_copy = in_epochTime;

#ifdef __LINUX

	long long secends = in_epochTime_copy / 1000000;
	long long microsecends = in_epochTime_copy - secends * 1000000;

    struct timeval tv;
    tv.tv_sec = secends;
    tv.tv_usec = microsecends;
    struct tm* ptm;

    ptm = localtime (&tv.tv_sec);

    localTimeForReturn.wYear = 1900 + ptm->tm_year;
    localTimeForReturn.wMonth = ptm->tm_mon + 1;
    localTimeForReturn.wDay = ptm->tm_mday;
    localTimeForReturn.wHour = ptm->tm_hour;
    localTimeForReturn.wMinute = ptm->tm_min;
    localTimeForReturn.wSecond = ptm->tm_sec;

#else

	FILETIME ft;
	ZeroMemory(&ft , sizeof(ft));

	in_epochTime_copy += DELTA_EPOCH_IN_MICROSECS;
	in_epochTime_copy *=10;

	ft.dwHighDateTime = (DWORD)(in_epochTime_copy >> 32);
	ft.dwLowDateTime = (DWORD)(in_epochTime_copy);

	FILETIME ft_local;
	FileTimeToLocalFileTime(&ft , &ft_local);
	FileTimeToSystemTime(&ft_local , &localTimeForReturn);

#endif

	return localTimeForReturn;
}

unsigned int CUtilityFunc::getMiliSecondFromMidnight(SYSTEMTIME & in_crntSystemTime)
{
	unsigned int returnValue = (in_crntSystemTime.wHour * 60) + in_crntSystemTime.wMinute;
	returnValue = (returnValue * 60) + (unsigned int)(in_crntSystemTime.wSecond);
	returnValue = (returnValue * 1000) + (unsigned int)(in_crntSystemTime.wMilliseconds);

	return returnValue;
}

bool CUtilityFunc::doSyncTimeFromIP(char * in_IP)
{
	if( strcmp(in_IP , "127.0.0.1") == 0 )
		return true;

	printf("\n\n\n\n ######## START TIME SYNC ######## \n\n\n\n\n");

#ifdef __LINUX
	char th_setTime_string[100];
	sprintf_s(th_setTime_string, _ArraySizeMacro_(th_setTime_string), "ntpdate %s", in_IP);
	system(th_setTime_string);

    return true;
#else 
	int int_ret = system("Net stop w32time");
	Sleep(10);
	int_ret = system("Net start w32time");
	Sleep(10);

	char th_setTime_string[100];
	sprintf_s(th_setTime_string , _ArraySizeMacro_(th_setTime_string) , "net time \\\\%s /set /yes" , in_IP);
	int_ret = system(th_setTime_string);

	printf("\n\n\n\n ######## FINISH TIME SYNC ######## \n\n\n\n\n");

	if(int_ret == 0)
	{
		printf("\n\n Do Sync Time From IP (%s) is success\n\n" , in_IP);
		return true;
	}
#endif 

	printf("\n\n Do Sync Time From IP (%s) is FAIL\n\n" , in_IP);
	return false;
}

bool CUtilityFunc::doAdvanceShare(char * in_IP , char * in_user , char * in_pass)
{
	if( strcmp(in_IP , "127.0.0.1") == 0 )
		return true;
        
#ifdef __LINUX
        return true;
#else 

	char commandString[100];
	sprintf_s(commandString , _ArraySizeMacro_(commandString) , "cmdkey /add:%s /user:%s /pass:%s" , in_IP , in_user , in_pass);

	printf("\n\n\n\n ********* START ADVANCE SHARE ********** \n\n\n\n\n");
	int int_ret = system(commandString);
	printf("\n\n\n\n ********* FINISH ADVANCE SHARE ********** \n\n\n\n\n");

	if(int_ret == 0)
	{
		return true;
	}
#endif
        
	return false;
}

void CUtilityFunc::specificSocketConnectionDailyLog(char * in_rootFolder , char * in_IP , unsigned short in_portNo , char * in_connectivityStatus)
{
	char th_LogFilePath[300];

	SYSTEMTIME th_crntTimePath;
	GetLocalTime(& th_crntTimePath);

	sprintf_s(th_LogFilePath , _ArraySizeMacro_(th_LogFilePath) , "%s/(%s-%i)-%i-%i-%i_socketConnectivityStatus.txt" , in_rootFolder , in_IP , in_portNo ,
				th_crntTimePath.wYear , th_crntTimePath.wMonth , th_crntTimePath.wDay );

	ofstream th_saveSocketConnectLogFile_connect;
	th_saveSocketConnectLogFile_connect.open(th_LogFilePath , ios_base::out | ios_base::app);
	if(th_saveSocketConnectLogFile_connect)
	{
		SYSTEMTIME th_crntTimeLog;
		GetLocalTime(& th_crntTimeLog);
		char th_saveSocketConnectLog[300];
		sprintf_s(th_saveSocketConnectLog , _ArraySizeMacro_(th_saveSocketConnectLog) , "%s-%i-%i-%i-%i-%i-%i-%i" , in_connectivityStatus ,
					th_crntTimeLog.wYear , th_crntTimeLog.wMonth , th_crntTimeLog.wDay ,
					th_crntTimeLog.wHour , th_crntTimeLog.wMinute , th_crntTimeLog.wSecond , th_crntTimeLog.wMilliseconds);

		th_saveSocketConnectLogFile_connect<<th_saveSocketConnectLog<<endl;
		th_saveSocketConnectLogFile_connect.flush();
		th_saveSocketConnectLogFile_connect.close();
	}
}

#ifdef USE_OPENCV_IN_UTILITYFUNC_CLASS
void CUtilityFunc::pushPointInImage(Point & inPoint , int inImgCols , int inImgRows)
{
	Point2d inPoint2d = Point2d( (double)(inPoint.x) , (double)(inPoint.y) );

	pushPointInImage(inPoint2d , inImgCols , inImgRows);

	inPoint.x = getRound(inPoint2d.x);
	inPoint.y = getRound(inPoint2d.y);
}

void CUtilityFunc::pushPointInImage(Point2d & inPoint , int inImgCols , int inImgRows)
{
	if(inPoint.x <= 0.0)
		inPoint.x = 0.0;

	if(inPoint.y <= 0.0)
		inPoint.y = 0.0;

	if(inPoint.x >= inImgCols - 1)
		inPoint.x = inImgCols - 1.0;

	if(inPoint.y >= inImgRows - 1)
		inPoint.y = inImgRows - 1.0;
}

void CUtilityFunc::pushRectInImage(Rect & inRect, int inImgCols, int inImgRows)
{
	if(inRect.x < 0)
		inRect.x = 0;

	if (inRect.y < 0)
		inRect.y = 0;

	if(inRect.x + inRect.width > inImgCols)
		inRect.width = inImgCols - inRect.x;

	if (inRect.y + inRect.height > inImgRows)
		inRect.height = inImgRows - inRect.y;
}
#endif

bool CUtilityFunc::isInIranNowJustFullDay()
{
	SYSTEMTIME th_nowTime;
	GetLocalTime(&th_nowTime);

	if(th_nowTime.wHour >= tolo_khoy_table[th_nowTime.wMonth] && th_nowTime.wHour < qoroob_zabol_table[th_nowTime.wMonth])
		return true;

	return false;
}

bool CUtilityFunc::ethernetReset(string & in_ethernetName)
{
	if (in_ethernetName.empty())
		return true;

	char strCmd_disable[200] , strCmd_enable[200];
	cout << "\n\n		Ethernet Reseting .....\n\n" << endl;

#ifndef __LINUX
	sprintf_s(strCmd_disable, _ArraySizeMacro_(strCmd_disable), "netsh interface set interface \"%s\" disable", in_ethernetName.c_str());


	sprintf_s(strCmd_enable, _ArraySizeMacro_(strCmd_enable), "netsh interface set interface \"%s\" enable", in_ethernetName.c_str());
#else	
	sprintf(strCmd_disable, "ifdown %s", in_ethernetName.c_str());

	sprintf(strCmd_enable, "ifup %s", in_ethernetName.c_str());
#endif

	system(strCmd_disable);
	Sleep(1000);

	system(strCmd_enable);

	Sleep(10000);
	cout << "\n\n		Ethernet Reset done !\n\n" << endl;

	return true;
}

#ifdef __LINUX
std::string CUtilityFunc::exec(const char* cmd)
{
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}
std::string CUtilityFunc::trim(const std::string &s)
{
    auto wsfront=std::find_if_not(s.begin(),s.end(),[](int c){return std::isspace(c);});
    auto wsback=std::find_if_not(s.rbegin(),s.rend(),[](int c){return std::isspace(c);}).base();
    return (wsback<=wsfront ? std::string() : std::string(wsfront,wsback));
}
#endif // __LINUX