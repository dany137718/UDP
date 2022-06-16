#ifndef _DEFINE_CONSTANT_VARIABLE_H_
#define _DEFINE_CONSTANT_VARIABLE_H_

//#define __LINUX
#ifdef __LINUX
#include "linuxUTIL.h"
#include "dirent.h"
//Linux share memory includes
#include <sys/ipc.h>
#include <sys/shm.h>

#define HANDLE pthread_t
#define ThreadReturn void *
#else
#include <windows.h>
#include <direct.h>
#include "dirent.h"
#pragma warning( disable : 4250 )
#pragma warning(disable:4996);

#include <process.h>
#define ThreadReturn unsigned __stdcall
#endif

#define NOMINMAX

#include <math.h>
#include <iterator>
#include <memory>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace std;
using namespace cv;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////// GENERAL /////////////////
#define CONSOL_WRITE_ACT

///////////////////////////////////////////////////////

#define EPSILON numeric_limits<double>::epsilon()
#define PI 3.1415926535897932384626433832795
#define LN_TWO  0.69314718055994530941723212145818
#define E_NUMBER  2.7182818284590452353602874713527

//////////////////////////////////////////////////////////////////////////

template <typename T , unsigned int N>
char(&COUNTOF_REQUIRES_ARRAY_ARGUMENT(T(&)[N]))[N];
#define _ArraySizeMacro_(x) sizeof(COUNTOF_REQUIRES_ARRAY_ARGUMENT(x))

#endif