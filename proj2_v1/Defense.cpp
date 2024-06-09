#include "Defense.h"

Defense::Defense(QObject *parent) : QObject(parent)
{

}

Melee::Melee(QWidget* par, Coor p, int n)
{
    maxHp = hp = MELEE_HP, atk = MELEE_ATK, cd = MELEE_CD;
    pos = make_pair(p.first, p.second);
    parent = par;
    num = n;
    attackType = 0;
    targetNum = 1;
    mad = freeze = triple = heal = bleed = saint = slow = critical = 0;
    bulletNum = clawNum = 0;
}

Melee::~Melee()
{
    parent = NULL;
}

bool Melee::in_range(Coor other_pos)
{
    int x = pos.first, y = pos.second, x1 = other_pos.first, y1 = other_pos.second;
    return (abs(x - x1) <= LEN && y == y1) || (abs(y - y1) <= LEN && x == x1);
}

Archer::Archer(QWidget* par, Coor p, int n)
{
    maxHp = hp = ARCHER_HP, atk = ARCHER_ATK, cd = ARCHER_CD;
    range = ARCHER_RANGE;
    pos = make_pair(p.first, p.second);
    parent = par;
    num = n;
    attackType = 1;
    targetNum = 1;
    mad = freeze = triple = heal = bleed = saint = slow = critical = 0;
    bulletNum = clawNum = 0;
}

Archer::~Archer()
{
    parent = NULL;
}

bool Archer::in_range(Coor other_pos)
{
    int x = pos.first, y = pos.second, x1 = other_pos.first, y1 = other_pos.second;
    return (x - x1) * (x - x1) + (y - y1) * (y - y1) <= range * LEN * range * LEN;
}
