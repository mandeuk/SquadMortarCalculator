#pragma once
#include <Windows.h>
//#include <time.h>
#include <math.h>
#include <atlstr.h>
#include <atlimage.h>

#define W_WIDTH 300
#define W_HEIGHT 320
#define PI 3.141592f

#define IDC_BUTTON 100
#define IDC_EDIT 200

#define IDC_EDIT_MYPOS 201
#define IDC_EDIT_ENEMPOS 202

#define GRID_DISTANCE 300
#define KP_DISTANCE 100
#define SKP_DISTANCE 33

#define HELPTEXT_X 30
#define HELPTEXT_Y 100



#define VERSION "v0.2"

#define MIL_50M		1579
#define MIL_100M	1558
#define MIL_150M	1538
#define MIL_200M	1517
#define MIL_250M	1496
#define MIL_300M	1475
#define MIL_350M	1453
#define MIL_400M	1431
#define MIL_450M	1409
#define MIL_500M	1387
#define MIL_550M	1364
#define MIL_600M	1341
#define MIL_650M	1317
#define MIL_700M	1292
#define MIL_750M	1267
#define MIL_800M	1240
#define MIL_850M	1212
#define MIL_900M	1183
#define MIL_950M	1152
#define MIL_1000M	1118
#define MIL_1050M	1081
#define MIL_1100M	1039
#define MIL_1150M	988
#define MIL_1200M	918
#define MIL_1250M	800

int MilMeter[26]{ 1579,
1558,
1538,
1517,
1496,
1475,
1453,
1431,
1409,
1387,
1364,
1341,
1317,
1292,
1267,
1240,
1212,
1183,
1152,
1118,
1081,
1039,
988,
918,
800
};

int MilGap[26]{
	21,
	20,
	21,
	21,
	21,
	22,
	22,
	22,
	22,
	23,
	23,
	24,
	25,
	25,
	27,
	28,
	29,
	31,
	34,
	37,
	42,
	51,
	70,
	118,
	0
};