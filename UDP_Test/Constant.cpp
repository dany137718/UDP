#include "Constant.h"

CConstantParam * constantParamIns;

CConstantParam::CConstantParam()
{
	shareMemorySize = 0;
	ALL_CPU_ID = CAR_FINDER_CPU_ID = PLATE_FINDER_CPU_ID = SOCKET_CPU_ID = ETC_CPU_ID = 0;

	loadParameter();

	int ret = 0;

	ret = _mkdir(BASE_FOLDER_FOR_SAVE);
	if(ret == -1 && errno == ENOENT)
	{
		cout << "somthing wrong in creation of " << BASE_FOLDER_FOR_SAVE << "parameter ->" << endl << "Exit after 5 sec ..." << endl;
		Sleep(5000);
		exit(EXIT_FAILURE);
	}

	ret = _mkdir(BASE_RAM_DISK_FOLDER);
	if (ret == -1 && errno == ENOENT)
	{
		cout << "somthing wrong in creation of " << BASE_RAM_DISK_FOLDER << "parameter ->" << endl << "Exit after 5 sec ..." << endl;
		Sleep(5000);
		exit(EXIT_FAILURE);
	}

	sprintf_s(ALL_SYSTEM_LOG_FOLDER_PATH , _ArraySizeMacro_(ALL_SYSTEM_LOG_FOLDER_PATH) , "%s/System Log" , BASE_FOLDER_FOR_SAVE);

	/////////////////////////////////////////
	sprintf_s(mySelfProcessName , _ArraySizeMacro_(mySelfProcessName) , "AI");

	sprintf_s(generalLogFolderPath , _ArraySizeMacro_(generalLogFolderPath) , "%s/%s" , ALL_SYSTEM_LOG_FOLDER_PATH , mySelfProcessName);

	ret = _mkdir(ALL_SYSTEM_LOG_FOLDER_PATH);
	if (ret == -1 && errno == ENOENT)
	{
		cout << "somthing wrong in creation of " << ALL_SYSTEM_LOG_FOLDER_PATH << "parameter ->" << endl << "Exit after 5 sec ..." << endl;
		Sleep(5000);
		exit(EXIT_FAILURE);
	}

	ret = _mkdir(generalLogFolderPath);
	if (ret == -1 && errno == ENOENT)
	{
		cout << "somthing wrong in creation of " << generalLogFolderPath << "parameter ->" << endl << "Exit after 5 sec ..." << endl;
		Sleep(5000);
		exit(EXIT_FAILURE);
	}
}

CConstantParam::~CConstantParam()
{
}

void CConstantParam::loadParameter()
{
	try
	{
		FileStorage fs_read("AI_Params.XML" , FileStorage::READ);

		if (fs_read.isOpened())
		{
			FileNode AI = fs_read["AI"];
			//////////////////////////
			string BASE_FOLDER_FOR_SAVE_string = (string)(fs_read["BASE_FOLDER_FOR_SAVE"]);
			if (BASE_FOLDER_FOR_SAVE_string.size() > 1)
				strcpy_s(BASE_FOLDER_FOR_SAVE , _ArraySizeMacro_(BASE_FOLDER_FOR_SAVE) , BASE_FOLDER_FOR_SAVE_string.c_str());
			else
				strcpy_s(BASE_FOLDER_FOR_SAVE , _ArraySizeMacro_(BASE_FOLDER_FOR_SAVE) , "F:");

			string BASE_RAM_DISK_FOLDER_string = (string)(fs_read["BASE_RAM_DISK_FOLDER"]);
			if (BASE_RAM_DISK_FOLDER_string.size() > 1)
				strcpy_s(BASE_RAM_DISK_FOLDER , _ArraySizeMacro_(BASE_RAM_DISK_FOLDER) , BASE_RAM_DISK_FOLDER_string.c_str());
			else
				strcpy_s(BASE_RAM_DISK_FOLDER , _ArraySizeMacro_(BASE_RAM_DISK_FOLDER) , "H:");

			FileNode CpuIdNode = fs_read["Cpu_ID"];

			CAR_FINDER_CPU_ID = (int)(CpuIdNode["CAR_FINDER_CPU_ID"]);
			PLATE_FINDER_CPU_ID = (int)(CpuIdNode["PLATE_FINDER_CPU_ID"]);
			SOCKET_CPU_ID = (int)(CpuIdNode["SOCKET_CPU_ID"]);
			ETC_CPU_ID = (int)(CpuIdNode["ETC_CPU_ID"]);
			
			fs_read.release();
		}
		else
		{
			cout << "Error ..." << endl << "AI_Params.XML Damaged or not Found !" << endl;
			fs_read.release();
			Sleep(4000);
			exit(EXIT_FAILURE);
		}
	}
	catch (const cv::Exception& e)
	{
		cout << "Error ..." << endl << "Something Wrong : " << endl << "\t" << e.msg;
		Sleep(4000);
		exit(EXIT_FAILURE);
	}
}



