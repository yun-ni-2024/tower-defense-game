#ifndef ENEMY_H
#define ENEMY_H


#pragma once
#include <QObject>
#include "Function.h"

//STANDARD CONSTANTS//STANDARD CONSTANTS//STANDARD CONSTANTS//
const int ENEMY_HP = 5;
const int ENEMY_ATK = 1;
const float ENEMY_CD = 1;
const float ENEMY_SPEED = 1;
const int FLASH_CD = 5;
const float ENEMY_RANGE = 3;
//STANDARD CONSTANTS//STANDARD CONSTANTS//STANDARD CONSTANTS//

class Enemy : public QObject
{
    Q_OBJECT

private:
    friend class Simulation;
    int maxHp;
    int hp; //生命值
    int atk; //攻击力
    float cd; //攻击冷却
    float speed; //行走速度
    float range;
    Coor pos; //当前位置
    int affix;
    int lastTime;
    int abilityCd;
    bool frozen;
    int bleeding, slowing;

    QWidget* parent;
    int num, attackType;
    int bulletNum, clawNum;

public:
    explicit Enemy(QObject *parent = nullptr);
    Enemy(QWidget* par, Coor p, int n);
    ~Enemy();
    bool in_range(Coor other_pos);
    float get_range(Coor other_pos);

signals:

};

#endif // ENEMY_H
