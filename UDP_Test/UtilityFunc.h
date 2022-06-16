#ifndef _UTILITY_FUNC_H_
#define _UTILITY_FUNC_H_

#include "Constant.h"

//#ifndef __LINUX
//#include <windows.h>
//#include <direct.h>
//#include "dirent.h"
//#else
//#include <dirent.h>
//#endif
//
//#include <vector>
//#include <string>
//
//#include <fstream>
//using namespace std;

#define USE_OPENCV_IN_UTILITYFUNC_CLASS
#define SAVE_HEALTH_ISSUE_LOG

#ifdef USE_OPENCV_IN_UTILITYFUNC_CLASS
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
#endif

enum ACTIVE_HEALTH_ISSUE_LOG_ACTION_ENUM {ACTIVE_HEALTH_ISSUE_LOG_CREATE = 0 , ACTIVE_HEALTH_ISSUE_LOG_ERASE};
enum GENERAL_LOG_TYPE_ENUM {DEBUG_LOG_TYPE = 0 , INFO_LOG_TYPE , WARN_LOG_TYPE , ERROR_LOG_TYPE , FATAL_LOG_TYPE , GENERAL_LOG_TYPE_ENUM_SIZE};

class CUtilityFunc
{
	char healthIssueLog_subFolderPath[200] , GENERAL_LOG_TYPE_ENUM_STRING[GENERAL_LOG_TYPE_ENUM_SIZE][100];
	int tolo_khoy_table[13] , qoroob_zabol_table[13];

public:
	CUtilityFunc(void);
	~CUtilityFunc(void);

	long long getFakeEpochTime();
	SYSTEMTIME convertEpochTimeToSystemTime(long long in_epochTime);
	unsigned int getMiliSecondFromMidnight(SYSTEMTIME & in_crntSystemTime);

#ifndef __LINUX
	long long getFakeEpochTimeFromFileTime(FILETIME ft);
#endif

	template<typename T_getMaxFunc>
	inline T_getMaxFunc getMax(T_getMaxFunc valA , T_getMaxFunc valB){ return(valA > valB ? valA : valB); }

	template<typename T_getMinFunc>
	inline T_getMinFunc getMin(T_getMinFunc valA , T_getMinFunc valB){ return(valA < valB ? valA : valB); }

	inline int getRound(double val){ return (int)(val < 0.0 ? ceil(val - 0.5) : floor(val + 0.5)); }
	inline int getRound(float val){ return (int)(val < 0.0 ? ceil(val - 0.5) : floor(val + 0.5)); }

	bool deleteDirectory(char * in_directoryPath);
	void dirDirectory(const char * in_directoryPath , vector<string> & dirResultFullPath , vector<string> & dirResultName);

	bool isGeneralLogCreateBusy;
	void generalLogCreate(GENERAL_LOG_TYPE_ENUM in_generalLogTypeEnum , char * in_functionality , char * in_comment);
	void exitAndLog(GENERAL_LOG_TYPE_ENUM in_generalLogTypeEnum , char * in_logStr);
	void softResetAndLog(GENERAL_LOG_TYPE_ENUM in_generalLogTypeEnum, char * in_logStr);
	void terminateServerSocketThreadLog(unsigned short in_portNumber , int in_typeOfTerminate);
	void socketCreateLog(unsigned short in_portNumber , char * in_funcName);

	bool isFileHaveNullCharacter(char * in_fileName);

#ifdef SAVE_HEALTH_ISSUE_LOG
	void healthIssueLog(char * in_logStr);
	void activeHealthIssueLog(char * in_logStr , ACTIVE_HEALTH_ISSUE_LOG_ACTION_ENUM in_action);
#endif

	bool doSyncTimeFromIP(char * in_IP);
	bool doAdvanceShare(char * in_IP , char * in_user , char * in_pass);
	void specificSocketConnectionDailyLog(char * in_rootFolder , char * in_IP , unsigned short in_portNo , char * in_connectivityStatus);

#ifdef USE_OPENCV_IN_UTILITYFUNC_CLASS
	inline Point getRound(Point2d inPoint2d){ return( Point( getRound(inPoint2d.x) , getRound(inPoint2d.y) ) ); }
	inline Size getRound(Size2f inSize2f){ return( Size(getRound(inSize2f.width) , getRound(inSize2f.height)) ); }
	inline bool isPointInImage(Point inPoint , Mat & inImg){ return(inPoint.x >= 0 && inPoint.x < inImg.cols && inPoint.y >= 0 && inPoint.y < inImg.rows); }
	void pushPointInImage(Point & inPoint , int inImgCols , int inImgRows);
	void pushPointInImage(Point2d & inPoint , int inImgCols , int inImgRows);
	void pushRectInImage(Rect & inRect, int inImgCols, int inImgRows);
#endif

	bool isInIranNowJustFullDay();
	bool ethernetReset(string & in_ethernetName);

#ifdef __LINUX
	std::string exec(const char* cmd);
	std::string trim(const std::string &s);
#endif
};

extern CUtilityFunc * utilityFuncInstance;

#endif
