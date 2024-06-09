#include "Enemy.h"

int AFFIX1_PROB;
int AFFIX2_PROB;
int AFFIX3_PROB;

Enemy::Enemy(QObject *parent) : QObject(parent)
{

}

Enemy::Enemy(QWidget* par, Coor p, int n)
{
    maxHp = hp = ENEMY_HP, atk = ENEMY_ATK, cd = ENEMY_CD, speed = ENEMY_SPEED;
    pos = make_pair(p.first, p.second);
    parent = par;
    num = n;
    frozen = 0, bleeding = slowing = 0;
    attackType = 2;
    bulletNum = clawNum = 0;

    affix = 0;
    srand((unsigned)time(NULL));
    int rd1 = rand() % 100;
    int rd2 = rand() % 100;
    int rd3 = rand() % 100;
    if (rd1 < AFFIX1_PROB) //闪现的
    {
        affix += 1;
        abilityCd = FLASH_CD;
        lastTime = 0;
    }
    if (rd2 < AFFIX2_PROB) //神速的
    {
        affix += 2;
    }
    if (!(rd1 < AFFIX1_PROB && rd2 < AFFIX2_PROB) && rd3 < AFFIX3_PROB) //远程的
    {
        affix += 4;
        attackType = 3;
        range = ENEMY_RANGE;
    }
}

Enemy::~Enemy()
{
    parent = NULL;
}

bool Enemy::in_range(Coor other_pos)
{
    int x = pos.first, y = pos.second, x1 = other_pos.first, y1 = other_pos.second;
    if (attackType == 2)
        return (abs(x - x1) <= LEN && y == y1) || (abs(y - y1) <= LEN && x == x1);
    else
        return (x - x1) * (x - x1) + (y - y1) * (y - y1) <= range * LEN * range * LEN;
}

float Enemy::get_range(Coor other_pos)
{
    int x = pos.first, y = pos.second, x1 = other_pos.first, y1 = other_pos.second;
    return (x - x1) * (x - x1) + (y - y1) * (y - y1);
}
