#pragma once

#include "resource.h"


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

#define WM_MOUSEWHEEL                   0x020A
#define MOUSE_WHEELDOWN	-120
#define MOUSE_WHEELUP	120
#define MAP_MAXZOOM 5
#define MAP_MINZOOM 1

#define MAP_JENSENSRANGE 1
#define MAPSIZE_JENSENSRANGE 1511

enum Language {KOR, ENG};

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