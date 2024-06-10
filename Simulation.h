#ifndef SIMULATION_H
#define SIMULATION_H


#pragma once
#include <QObject>
#include "Function.h"
#include "Map.h"
#include "Enemy.h"
#include "Defense.h"

class Simulation : public QObject
{
    Q_OBJECT

private:
    friend class Widget;
    bool endFlag;
    Map* map;
    vector <Enemy*> enemies;
    vector <Defense*> defenses;
    int money, heart, leftEnemy;
    int affixes[10];
    bool death[1000];
    static void print(Simulation* sim);
    static void path_simulation(Path p, int num, Simulation* sim);
    static void enemy_simulation(Enemy* e, Path p, Simulation* sim);
    static void player_simulation(Simulation*);
    static void defense_simulation(Defense* d, Simulation* sim);
    static void defense_bullet_simulation(int defenseNum, int bulletNum, Defense* d, Enemy* e, bool is_cri, Simulation* sim);
    static void defense_claw_simulation(int defenseNum, int clawNum, Simulation* sim);
    static void enemy_bullet_simulation(int enemyNum, int bulletNum, Enemy* e, Defense* d, Simulation* sim);
    static void enemy_claw_simulation(int enemyNum, int clawNum, Simulation* sim);
    static void moneyNotEnough_simulation(Simulation* sim);
    static void blocked_simulation(Simulation* sim);
    static void affixNotEnough_simulation(Simulation* sim);
    static void alreadyInstalled_simulation(Simulation* sim);
    static void remake_simulation(Simulation* sim);
    void attack(Defense* d, Enemy* e, int typ);
    void meleeClicked(int i, int j);
    void archerClicked(int i, int j);
    void affixClicked(int num, int affNum, int typ);
    void removeAffixClicked(int num, int affNum, int typ);
    void removeClicked(int num, int typ);
    void kill(Enemy* e);
    void end();

    QWidget* parent;

public:
    explicit Simulation(QObject *parent = nullptr);
    Simulation(QWidget* par, Map* m);
    ~Simulation();
    void Start();

signals:
    void createEnemy(int num, Coor pos, int affix);
    void enemyMoveTo(int num, Coor pos);
    void createMelee(int num, Coor pos);
    void createArcher(int num, Coor pos);
    void eliminateDefense(int num);
    void eliminateEnemy(int num);

    void createDefenseBullet(int defenseNum, int bulletNum, Coor pos, int typ);
    void defenseBulletMoveTo(int defenseNum, int bulletNum, Coor pos);
    void eliminateDefenseBullet(int defenseNum, int bulletNum);

    void createDefenseClaw(int defenseNum, int clawNum, Coor pos, int typ);
    void defenseClawResize(int defenseNum, int clawNum, int len);
    void eliminateDefenseClaw(int defenseNum, int clawNum);

    void createEnemyBullet(int enemyNum, int bulletNum, Coor pos, int typ);
    void enemyBulletMoveTo(int enemyNum, int bulletNum, Coor pos);
    void eliminateEnemyBullet(int enemyNum, int bulletNum);

    void createEnemyClaw(int enemyNum, int clawNum, Coor pos, int typ);
    void enemyClawResize(int enemyNum, int clawNum, int len);
    void eliminateEnemyClaw(int enemyNum, int clawNum);

    void installAffix(int num, int affNum, int typ);
    void removeAffix(int num, int affNum, int defenseType);
    void updateEnemyHp(int num, int hp);
    void updateDefenseHp(int num, int hp_len);
    void updateMoney(int money);
    void moneyNotEnough();
    void blocked();
    void affixNotEnough();
    void alreadyInstalled();
    void closeMessage();
    void updateHeart(int heart);
    void updateAffix(int typ, int num);
    void freeze(int num);
    void unfreeze(int num, int affix);
    void bleed(int num);
    void unbleed(int num);
    void slow(int num);
    void unslow(int num);
    void makeSound(int typ);
    void lose();
    void win();
    void remake();

};

#endif // SIMULATION_H
