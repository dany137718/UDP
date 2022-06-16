#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#include "DefineConstantVariable.h"

class CConstantParam
{
public:

	CConstantParam();
	~CConstantParam();

	void loadParameter();

	string shareMemoryKey , shareMemoryType;
	unsigned int shareMemorySize;

///////////////////////////////////////////////////////////////////////////////////////////////////////
	//TODO: Clean Below !

	char generalLogFolderPath[100] , BASE_FOLDER_FOR_SAVE[100] , BASE_RAM_DISK_FOLDER[100];
	char ALL_SYSTEM_LOG_FOLDER_PATH[100];

	//TODO: set right Value and name
	unsigned long long ALL_CPU_ID , CAR_FINDER_CPU_ID , PLATE_FINDER_CPU_ID , SOCKET_CPU_ID , ETC_CPU_ID;

	char mySelfProcessName[100];
};

extern CConstantParam * constantParamIns;

/////////////////////////////////////////////////////
/////////// ##end of Constant.h file## //////////////
#endif