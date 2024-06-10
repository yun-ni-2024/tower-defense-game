#ifndef DEFENSE_H
#define DEFENSE_H


#pragma once
#include <QObject>
#include "Function.h"

//STANDARD CONSTANTS//STANDARD CONSTANTS//STANDARD CONSTANTS//
const int MELEE_HP = 10;
const int MELEE_ATK = 1;
const int MELEE_CD = 1;
const int ARCHER_HP = 3;
const int ARCHER_ATK = 1;
const int ARCHER_CD = 1;
const int ARCHER_RANGE = 3;
//STANDARD CONSTANTS//STANDARD CONSTANTS//STANDARD CONSTANTS//

class Defense : public QObject
{
    Q_OBJECT

protected:
    friend class Simulation;
    friend class Widget;
    int maxHp;
    int hp;
    int atk;
    float cd;
    int targetNum;
    bool mad, freeze, triple, bleed, saint, slow, heal, critical;
    pair <int, int> pos;

    QWidget* parent;
    int num, attackType;
    int bulletNum, clawNum;

public:
    explicit Defense(QObject *parent = nullptr);
    virtual ~Defense() {}
    virtual bool in_range(Coor other_pos) = 0;

signals:

};

class Melee : public Defense // 近战塔
{
public:
    Melee(QWidget* par, Coor p, int n);
    ~Melee();
    bool in_range(Coor other_pos);

};

class Archer : public Defense
{
    float range;
public:
    Archer(QWidget* par, Coor p, int n);
    ~Archer();
    bool in_range(Coor other_pos);

};

#endif // DEFENSE_H
