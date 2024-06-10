#ifndef FUNCTION_H
#define FUNCTION_H


#pragma once

#include <iostream>
#include <fstream>
#include <thread>
#include <cstring>
#include <Windows.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <utility>

#include <QPushButton>
#include <QMessageBox>
#include <QTextEdit>
#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QString>
#include <QPicture>
#include <QMediaPlayer>
#include <QSoundEffect>

using namespace std;

typedef pair <int, int> Coor;
typedef vector <Coor> Path;

//STANDARD CONSTANTS//STANDARD CONSTANTS//STANDARD CONSTANTS//
const int MAX_MAP_NUM = 20;
const int N = 13, M = 10;
const int LEN = 70;
const int OFFSET = 30;
const int DISPLAY_WID = LEN * N + OFFSET * 2;
const int DISPLAY_HEI = LEN * M + OFFSET * 2;
const int STATUS_WID = 115;
const int ENEMY_LEN = LEN * 0.8;
const int ENEMY_OFFSET = (LEN - ENEMY_LEN) / 2 + OFFSET;
const int INR_OF_ENEMIES = 5;
const int MAX_ENEMY_NUM = 510;
const int CHOOSE_BUTTON_LEN = LEN * 1.2;
const int MAX_DEFENSE_NUM = 100;
const int DEFENSE_LEN = LEN;
const int DEFENSE_OFFSET = (LEN - DEFENSE_LEN) / 2 + OFFSET;
const int BULLET_LEN = 14;
const int BULLET_OFFSET = (LEN - BULLET_LEN) / 2 + OFFSET;
const int MAX_BULLET_NUM = 1000;
const int CLAW_LEN = 50;
const int CLAW_WID = 4;
const int CLAW_OFFSET_X = (LEN - CLAW_LEN) / 2 + OFFSET;
const int CLAW_OFFSET_Y = (LEN - CLAW_WID) / 2 + OFFSET;
const int MAX_CLAW_NUM = 1000;
const int BONUS_PROB = 30;
#define MAP_BLANK_GRAPH QIcon(":/resource/blank.ico")
#define MAP_PATH_GRAPH QIcon(":/resource/path.ico")
#define MAP_SIDE_GRAPH  QIcon(":/resource/side.ico")
#define ENEMY_GRAPH QIcon(":/resource/enemy.ico")
#define ENEMY_GRAPH1 QIcon(":/resource/enemy1.ico")
#define ENEMY_GRAPH2 QIcon(":/resource/enemy2.ico")
#define ENEMY_GRAPH3 QIcon(":/resource/enemy3.ico")
#define MELEE_GRAPH QIcon(":/resource/melee.ico")
#define ARCHER_GRAPH QIcon(":/resource/archer.ico")
#define MONEY_GRAPH QIcon(":/resource/coin2.ico")
#define HEART_GRAPH QIcon(":/resource/heart.ico")
#define FROZEN_GRAPH QIcon(":/resource/frozen.ico")
#define BLOOD_GRAPH QIcon(":/resource/blood.ico")
#define SLOW_GRAPH QIcon(":/resource/slow.ico")
//STANDARD CONSTANTS//STANDARD CONSTANTS//STANDARD CONSTANTS//

extern int ENEMY_NUM;
extern int AFFIX1_PROB;
extern int AFFIX2_PROB;
extern int AFFIX3_PROB;
extern int INIT_AFFIX;

inline int getRand()
{
    srand((unsigned)time(NULL));
    int rd = rand();
    for (int i = 0; i < rand() % 10; ++i) rd = rand();
    return rd % 500;
}

#endif // FUNCTION_H
