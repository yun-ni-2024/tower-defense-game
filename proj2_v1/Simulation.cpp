#include "Simulation.h"

int ENEMY_NUM;
int INIT_AFFIX;

Simulation::Simulation(QObject *parent) : QObject(parent)
{

}

Simulation::Simulation(QWidget* par, Map* m)
{
    endFlag = 0;
    map = m;
    money = 2000, heart = 3, leftEnemy = ENEMY_NUM;
//    qDebug() << leftEnemy;
    for (int i = 0; i < 10; ++i) affixes[i] = INIT_AFFIX;
    memset(death, 0, sizeof(death));
    parent = par;
}

Simulation::~Simulation()
{
    map = NULL;
    parent = NULL;

    for (Enemy* e: enemies)
    {
        delete e;
        e = NULL;
    }
    enemies.clear();

    for (Defense* d: defenses)
    {
        delete d;
        d = NULL;
    }
    defenses.clear();
}

void Simulation::Start()
{
    for (size_t i = 0; i < map->paths.size(); ++i)
    {
        std::thread path_inst(path_simulation, map->paths[i], i, this);
        path_inst.detach();
    }
}

void Simulation::path_simulation(Path p, int num, Simulation* sim)
{
    for (int i = 0; i < num; ++i)
    {
//        int noise = getRand() % 400 - 200;
        Sleep(1131);
        if (sim->endFlag) return;

//        qDebug() << "There's still a path thread";
    }
    int x = (size_t)num < ENEMY_NUM % sim->map->paths.size() ? 1 : 0;
    for (size_t i = 0; i < ENEMY_NUM / sim->map->paths.size() + x; ++i)
    {
        if (sim->endFlag) return;

//        qDebug() << 'e' << sim->enemies.size();

        Enemy* e = new Enemy(sim->parent, make_pair(p[0].first * LEN, p[0].second * LEN), sim->enemies.size());

        qDebug() << 'e' << e->num;

        sim->enemies.push_back(e);
        emit sim->createEnemy(sim->enemies.size() - 1, make_pair(p[0].first * LEN, p[0].second * LEN), e->affix);
        std::thread enemy_inst(enemy_simulation, e, p, sim);
        enemy_inst.detach();

        for (int i = 0; i < INR_OF_ENEMIES; ++i)
        {
//            int noise = getRand() % 400 - 200;
            Sleep(1000);
            if (sim->endFlag) return;

//            qDebug() << "There's still a path thread";
        }


    }
    return;
}

void Simulation::enemy_simulation(Enemy* e, Path p, Simulation* sim)
{
    int trail = 0;
    while (e->hp && e->pos != make_pair(p[p.size() - 1].first * LEN, p[p.size() - 1].second * LEN))
    {
        if (sim->endFlag) return;

        if (e->hp && e->frozen)
        {
            int noise = getRand() % 400 - 200;
            if (e->hp) Sleep(1000 + noise);

//            qDebug() << "There's still an enemy thread";

            if (e->hp && e->bleeding)
            {
                e->hp = max(0, e->hp - 1);
                emit sim->updateEnemyHp(e->num, ENEMY_LEN * e->hp / e->maxHp);
                if (e->hp == 0) sim->kill(e);
                e->bleeding--;
                if (e->bleeding == 0) emit sim->unbleed(e->num);
            }

            noise = getRand() % 400 - 200;
            if (e->hp) Sleep(1000 + noise);

//            qDebug() << "There's still an enemy thread";

            if (e->hp && e->bleeding)
            {
                e->hp = max(0, e->hp - 1);
                emit sim->updateEnemyHp(e->num, ENEMY_LEN * e->hp / e->maxHp);
                if (e->hp == 0) sim->kill(e);
                e->bleeding--;
                if (e->bleeding == 0) emit sim->unbleed(e->num);
            }

            e->frozen = 0;
            emit sim->unfreeze(e->num, e->affix);
        }

        if (sim->endFlag) return;

//        qDebug() << "There's still an enemy thread";

        bool flash = 0;
        if (e->hp && e->affix % 2 && time(NULL) - e->lastTime >= e->abilityCd)
        {
            for (Defense* d: sim->defenses)
            {
                if (d->hp && d->pos == make_pair(p[trail + 1].first * LEN, p[trail + 1].second * LEN))
                {
                    flash = 1;
                    for (Defense* d1: sim->defenses)
                    {
                        if (d->hp && d1->pos == make_pair(p[trail + 2].first * LEN, p[trail + 2].second * LEN))
                        {
                            flash = 0;
                            break;
                        }
                    }
                    if (flash) break;
                }
            }
        }

//        qDebug() << "There's still an enemy thread";

        if (flash)
        {
            trail += 2;
            e->pos = make_pair(p[trail].first * LEN, p[trail].second * LEN);
            e->lastTime = time(NULL);
            emit sim->enemyMoveTo(e->num, e->pos);
            continue;
        }

        if (sim->endFlag) return;

        bool battle = 0;
        int target;
        float dist = 100000000;
        for (Defense* d: sim->defenses)
        {
            if (d->hp && !d->saint && e->in_range(d->pos) && d->pos != make_pair(p[trail - 1].first * LEN, p[trail - 1].second * LEN))
            {
                if (e->affix / 4 % 2 == 0)
                {
                    battle = 1;
                    target = d->num;
                    break;
                }
                else
                {
                    float this_dist = e->get_range(d->pos);
                    if (this_dist < dist)
                    {
                        battle = 1;
                        target = d->num;
                        dist = this_dist;
                    }
                }

            }
        }

        if (battle)
        {
            int noise = getRand() % 400 - 200;
            if (e->hp) Sleep(1000 * e->cd + noise);

            if (e->frozen) continue;

//            qDebug() << "There's still an enemy thread";

            if (e->hp && e->bleeding)
            {
                e->hp = max(0, e->hp - 1);
                emit sim->updateEnemyHp(e->num, ENEMY_LEN * e->hp / e->maxHp);
                if (e->hp == 0) sim->kill(e);
                e->bleeding--;
                if (e->bleeding == 0) emit sim->unbleed(e->num);
            }

            if (sim->defenses[target]->hp && e->hp)
            {
                sim->defenses[target]->hp = max(0, sim->defenses[target]->hp - e->atk);
                emit sim->updateDefenseHp(target, DEFENSE_LEN * sim->defenses[target]->hp / sim->defenses[target]->maxHp);
                sim->attack(sim->defenses[target], e, e->attackType);
            }
            if (sim->defenses[target]->hp == 0) emit sim->eliminateDefense(sim->defenses[target]->num);
        }
        else
        {
            Coor cur = p[trail], nxt = p[++trail];
            while (e->hp && e->pos != make_pair(nxt.first * LEN, nxt.second * LEN))
            {
//                qDebug() << "There's still an enemy thread";

                if (e->affix / 2 % 2)
                {
                    Sleep(1000 / LEN);
                    e->pos.first += (nxt.first - cur.first) * 2;
                    e->pos.second += (nxt.second - cur.second) * 2;
                    emit sim->enemyMoveTo(e->num, make_pair(e->pos.first, e->pos.second));
                }
                else if (e->slowing)
                {
                    Sleep(1000 * 2 / LEN);
                    e->pos.first += (nxt.first - cur.first);
                    e->pos.second += (nxt.second - cur.second);
                    emit sim->enemyMoveTo(e->num, make_pair(e->pos.first, e->pos.second));
                    e->slowing--;
                    if (e->slowing == 0) emit sim->unslow(e->num);
                }
                else
                {
                    Sleep(1000 / LEN);
                    e->pos.first += nxt.first - cur.first;
                    e->pos.second += nxt.second - cur.second;
                    emit sim->enemyMoveTo(e->num, make_pair(e->pos.first, e->pos.second));
                }
            }

            if (sim->endFlag) return;

            if (e->hp && e->bleeding && (e->slowing || (e->affix / 2 % 2 == 0 && trail % 2) || (e->affix / 2 % 2 && trail % 4)))
            {
                e->hp = max(0, e->hp - 1);
                emit sim->updateEnemyHp(e->num, ENEMY_LEN * e->hp / e->maxHp);
                if (e->hp == 0) sim->kill(e);
                e->bleeding--;
                if (e->bleeding == 0) emit sim->unbleed(e->num);
            }

//            qDebug() << "There's still an enemy thread";
        }
    }

    if (sim->endFlag) return;

//    qDebug() << "There's still an enemy thread";

    if (e->hp && e->pos == make_pair(p[p.size() - 1].first * LEN, p[p.size() - 1].second * LEN))
    {
        e->hp = 0;
        emit sim->eliminateEnemy(e->num);
        sim->heart = max(0, sim->heart - 1);
        emit sim->updateHeart(sim->heart);

        if (!sim->endFlag && sim->heart == 0)
        {
            sim->endFlag = 1;
            for (Enemy* e : sim->enemies) e->hp = 0;
            for (Defense* d : sim->defenses) d->hp = 0;
            emit sim->lose();

            qDebug() << "lose";

            std::thread remake_inst(remake_simulation, sim);
            remake_inst.detach();
        }

        sim->leftEnemy--;

//        qDebug() << sim->leftEnemy;
        sim->death[e->num] = 1;
        bool flag = 1;
        for (int i = 0; i < ENEMY_NUM; ++i)
            if (!sim->death[i])
            {
                flag = 0;
                break;
            }
        if (!sim->endFlag && flag) sim->end();
    }
    return;
}

void Simulation::defense_simulation(Defense* d, Simulation* sim)
{
    while (d->hp)
    {
        if (sim->endFlag) return;

        int noise = getRand() % 400 - 200;
        Sleep(1000 * d->cd + noise);

        if (d->hp && d->heal)
        {
            d->hp = min(d->maxHp, d->hp + 1);
            emit sim->updateDefenseHp(d->num, DEFENSE_LEN * d->hp / d->maxHp);
        }

        int cnt = 0;
        for (Enemy* e: sim->enemies)
        {
            if (e->hp && d->in_range(e->pos))
            {
                if (d->hp && e->hp)
                {
                    sim->attack(d, e, d->attackType);
                    cnt++;
                }
                if (cnt == d->targetNum) break;
            }
        }
    }
    return;
}

void Simulation::defense_bullet_simulation(int defenseNum, int bulletNum, Defense* d, Enemy* e, bool is_cri, Simulation* sim)
{
//    if (sim->bullet_used[num]) return;
//    sim->bullet_used[num]++;

//    qDebug() << '(' << d->pos.first << ',' << d->pos.second << ")->(" << e->pos.first << ',' << e->pos.second << ')';

    const int x_dist = e->pos.first - d->pos.first, y_dist = e->pos.second - d->pos.second, dist = sqrt(x_dist * x_dist + y_dist * y_dist);
    const float delta = LEN / 7;
    Coor pos = d->pos;
    for (int i = 1; i <= dist / delta; ++i)
    {
        if (sim->endFlag) return;
//        if (sim->bullet_used[num] > 1) return;

        Sleep(10);
//        if (sim->bullet_used[num] > 1) return;
        pos.first = d->pos.first + i * delta * x_dist / dist;
        pos.second = d->pos.second + i * delta * y_dist / dist;
        emit sim->defenseBulletMoveTo(defenseNum, bulletNum, pos);
    }
    emit sim->eliminateDefenseBullet(defenseNum, bulletNum);

    if (e->hp)
    {
        if (is_cri)
        {
            e->hp = 0;
            sim->kill(e);
            return;
        }

        e->hp = max(0, e->hp - d->atk);
        emit sim->updateEnemyHp(e->num, ENEMY_LEN * e->hp / e->maxHp);
        if (e->hp == 0)
        {
            sim->kill(e);
            return;
        }

        if (d->bleed)
        {
            e->bleeding = 3;
            emit sim->bleed(e->num);
        }
        if (d->slow)
        {
            e->slowing = 1.5 * LEN;
            emit sim->slow(e->num);
        }
    }
    if (sim->endFlag) return;
}

void Simulation::defense_claw_simulation(int defenseNum, int clawNum, Simulation* sim)
{
//    if (sim->claw_used[num]) return;
//    sim->claw_used[num]++;

    for (int i = 1; i <= CLAW_LEN / 3; ++i)
    {
        if (sim->endFlag) return;
//        if (sim->claw_used[num] > 1) return;

        Sleep(1);
//        if (sim->claw_used[num] > 1) return;
        emit sim->defenseClawResize(defenseNum, clawNum, i * 3);
    }
    emit sim->eliminateDefenseClaw(defenseNum, clawNum);
    if (sim->endFlag) return;
}

void Simulation::enemy_bullet_simulation(int enemyNum, int bulletNum, Enemy* e, Defense* d, Simulation* sim)
{
    const int x_dist = d->pos.first - e->pos.first, y_dist = d->pos.second - e->pos.second, dist = sqrt(x_dist * x_dist + y_dist * y_dist);
    const float delta = LEN / 3;
    Coor pos = e->pos;
    for (int i = 1; i <= dist / delta; ++i)
    {
        if (sim->endFlag) return;

        Sleep(10);
        pos.first = e->pos.first + i * delta * x_dist / dist;
        pos.second = e->pos.second + i * delta * y_dist / dist;
        emit sim->enemyBulletMoveTo(enemyNum, bulletNum, pos);
    }
    emit sim->eliminateEnemyBullet(enemyNum, bulletNum);
    return;
}

void Simulation::enemy_claw_simulation(int enemyNum, int clawNum, Simulation* sim)
{
    for (int i = 1; i <= CLAW_LEN / 3; ++i)
    {
        if (sim->endFlag) return;

        Sleep(1);
        emit sim->enemyClawResize(enemyNum, clawNum, i * 3);
    }
    emit sim->eliminateEnemyClaw(enemyNum, clawNum);
    if (sim->endFlag) return;
}

void Simulation::attack(Defense* d, Enemy* e, int typ)
{
//    if (is_attack) return;
//    is_attack++;

    switch(typ)
    {
    case 0: //melee
    {
//        if (is_attack > 1) return;
        emit this->createDefenseClaw(d->num, d->clawNum, e->pos, d->freeze);
        qDebug() << "defense claw: " << d->num << ' ' << d->clawNum;
//        if (is_attack > 1) return;
        std::thread defense_claw_inst(defense_claw_simulation, d->num, d->clawNum, this);
//        if (is_attack > 1) return;
        defense_claw_inst.detach();
        d->clawNum++;
//        if (is_attack > 1) return;
//        if (is_attack > 1) return;
        e->hp = max(0, e->hp - d->atk);
//        if (is_attack > 1) return;
        emit this->updateEnemyHp(e->num, ENEMY_LEN * e->hp / e->maxHp);

        if (d->freeze)
        {
//            if (is_attack > 1) return;
            e->frozen = 1;
//            if (is_attack > 1) return;
            emit this->freeze(e->num);
        }

        if (e->hp == 0) kill(e);

        emit this->makeSound(0);

        break;
    }
    case 1: //archer
    {
//        if (is_attack > 1) return;
        bool is_cri = 0;
        if (d->critical)
        {
            int rd = getRand() % 100;
            if (rd < 30) is_cri = 1;
        }
        if (is_cri)
        {
//            if (is_attack > 1) return;
            emit this->createDefenseBullet(d->num, d->bulletNum, d->pos, 4);
            qDebug() << "defense bullet: " << d->num << ' ' << d->bulletNum;
//            if (is_attack > 1) return;
            std::thread defense_bullet_inst(defense_bullet_simulation, d->num, d->bulletNum, d, e, 1, this);
//            if (is_attack > 1) return;
            defense_bullet_inst.detach();
            d->bulletNum++;
//            if (is_attack > 1) return;

            emit this->makeSound(1);

            break;
        }

//        if (is_attack > 1) return;
        emit this->createDefenseBullet(d->num, d->bulletNum, d->pos, (d->slow << 1) + d->bleed);
        qDebug() << "defense bullet: " << d->num << ' ' << d->bulletNum;
//        if (is_attack > 1) return;
        std::thread defense_bullet_inst(defense_bullet_simulation, d->num, d->bulletNum, d, e, 0, this);
//        if (is_attack > 1) return;
        defense_bullet_inst.detach();
        d->bulletNum++;
//        if (is_attack > 1) return;

        emit this->makeSound(1);

        break;
    }
    case 2: //enemy
    {
//        if (is_attack > 1) return;
        emit this->createEnemyClaw(e->num, e->clawNum, d->pos, 0);
        qDebug() << "enemy claw: " << e->num << ' ' << e->clawNum;
//        if (is_attack > 1) return;
        std::thread enemy_claw_inst(enemy_claw_simulation, e->num, e->clawNum, this);
//        if (is_attack > 1) return;
        enemy_claw_inst.detach();
        e->clawNum++;
//        if (is_attack > 1) return;

        emit this->makeSound(0);

        break;
    }
    case 3: //enemy archer
    {
//        if (is_attack > 1) return;
        emit this->createEnemyBullet(e->num, e->bulletNum, e->pos, 5);
        qDebug() << "enemy bullet: " << e->num << ' ' << e->bulletNum;
//        if (is_attack > 1) return;
        std::thread enemy_bullet_inst(enemy_bullet_simulation, e->num, e->bulletNum, e, d, this);
//        if (is_attack > 1) return;
        enemy_bullet_inst.detach();
        e->bulletNum++;
//        if (is_attack > 1) return;

        emit this->makeSound(1);

        break;
    }
    default:
        break;
    }

//    is_attack = 0;
}

void Simulation::kill(Enemy* e)
{
    money += getRand() % 41 + 200;

//    qDebug() << leftEnemy;
    death[e->num] = 1;
    bool flag = 1;
    for (int i = 0; i < ENEMY_NUM; ++i)
        if (!death[i])
        {
            flag = 0;
            break;
        }

    if (!endFlag && flag) end();
    emit this->updateMoney(money);
    emit this->eliminateEnemy(e->num);

    int rd = getRand() % 100;
    const int tot = 8;
    if (rd < BONUS_PROB / tot)
    {
//        qDebug() << "狂暴的";
        affixes[1]++;
        emit this->updateAffix(1, affixes[1]);
    }
    else if (rd < BONUS_PROB * 2 / tot)
    {
//        qDebug() << "冰系的";
        affixes[2]++;
        emit this->updateAffix(2, affixes[2]);
    }
    else if (rd < BONUS_PROB * 3 / tot)
    {
//        qDebug() << "群伤的";
        affixes[3]++;
        emit this->updateAffix(3, affixes[3]);
    }
    else if (rd < BONUS_PROB * 4 / tot)
    {
//        qDebug() << "放血的";
        affixes[4]++;
        emit this->updateAffix(4, affixes[4]);
    }
    else if (rd < BONUS_PROB * 5 / tot)
    {
//        qDebug() << "隐匿的";
        affixes[5]++;
        emit this->updateAffix(5, affixes[5]);
    }
    else if (rd < BONUS_PROB * 6 / tot)
    {
//        qDebug() << "迟缓的";
        affixes[6]++;
        emit this->updateAffix(6, affixes[6]);
    }
    else if (rd < BONUS_PROB * 7 / tot)
    {
//        qDebug() << "自愈的";
        affixes[7]++;
        emit this->updateAffix(7, affixes[7]);
    }
    else if (rd < BONUS_PROB * 8 / tot)
    {
//        qDebug() << "必杀的";
        affixes[8]++;
        emit this->updateAffix(8, affixes[8]);
    }
}

void Simulation::end()
{
    endFlag = 1;
    for (Enemy* e: enemies) e->hp = 0;
    for (Defense* d: defenses) d->hp = 0;
    emit this->win();

    qDebug() << "win";

    std::thread remake_inst(remake_simulation, this);
    remake_inst.detach();
}

void Simulation::meleeClicked(int i, int j)
{
    for (Enemy* e: enemies)
        if (e->hp && e->pos.first > LEN * (i - 1) && e->pos.first < LEN * (i + 1) && e->pos.second > LEN * (j - 1) && e->pos.second < LEN * (j + 1))
        {
            std::thread blocked_inst(blocked_simulation, this);
            blocked_inst.detach();
            return;
        }

    if (money < 500)
    {
        std::thread moneyNotEnough_inst(moneyNotEnough_simulation, this);
        moneyNotEnough_inst.detach();
        return;
    }

    money -= 500;
    emit this->updateMoney(money);

    Melee* d = new Melee(parent, make_pair(i * LEN, j * LEN), defenses.size());
    defenses.push_back(d);
    emit this->createMelee(defenses.size() - 1, make_pair(i * LEN, j * LEN));
    std::thread defense_inst(defense_simulation, d, this);
    defense_inst.detach();
}

void Simulation::archerClicked(int i, int j)
{
    if (money < 1000)
    {
        std::thread moneyNotEnough_inst(moneyNotEnough_simulation, this);
        moneyNotEnough_inst.detach();
        return;
    }

    money -= 1000;
    emit this->updateMoney(money);

    qDebug() << 'd' << defenses.size();

    Archer* d = new Archer(parent, make_pair(i * LEN, j * LEN), defenses.size());
    defenses.push_back(d);
    emit this->createArcher(defenses.size() - 1, make_pair(i * LEN, j * LEN));
    std::thread defense_inst(defense_simulation, d, this);
    defense_inst.detach();
}

void Simulation::affixClicked(int num, int affNum, int typ)
{
    if (affixes[typ] == 0)
    {
        std::thread affixNotEnough_inst(affixNotEnough_simulation, this);
        affixNotEnough_inst.detach();
        return;
    }

    switch(typ)
    {
    case 1:
    {
        if (defenses[num]->mad)
        {
            std::thread alreadyInstalled_inst(alreadyInstalled_simulation, this);
            alreadyInstalled_inst.detach();
            break;
        }

        defenses[num]->mad = 1;
        defenses[num]->atk *= 2, defenses[num]->cd /= 2;
        emit this->installAffix(num, affNum, typ);
        affixes[typ]--;
        emit this->updateAffix(typ, affixes[typ]);
        break;
    }
    case 2:
    {
        if (defenses[num]->freeze)
        {
            std::thread alreadyInstalled_inst(alreadyInstalled_simulation, this);
            alreadyInstalled_inst.detach();
            break;
        }

        defenses[num]->freeze = 1;
        emit this->installAffix(num, affNum, typ);
        affixes[typ]--;
        emit this->updateAffix(typ, affixes[typ]);
        break;
    }
    case 3:
    {
        if (defenses[num]->triple)
        {
            std::thread alreadyInstalled_inst(alreadyInstalled_simulation, this);
            alreadyInstalled_inst.detach();
            break;
        }

        defenses[num]->triple = 1;
        defenses[num]->targetNum = 3;
        emit this->installAffix(num, affNum, typ);
        affixes[typ]--;
        emit this->updateAffix(typ, affixes[typ]);
        break;
    }
    case 4:
    {
        if (defenses[num]->bleed)
        {
            std::thread alreadyInstalled_inst(alreadyInstalled_simulation, this);
            alreadyInstalled_inst.detach();
            break;
        }

        defenses[num]->bleed = 1;
        emit this->installAffix(num, affNum, typ);
        affixes[typ]--;
        emit this->updateAffix(typ, affixes[typ]);
        break;
    }
    case 5:
    {
        if (defenses[num]->saint)
        {
            std::thread alreadyInstalled_inst(alreadyInstalled_simulation, this);
            alreadyInstalled_inst.detach();
            break;
        }

        defenses[num]->saint = 1;
        emit this->installAffix(num, affNum, typ);
        affixes[typ]--;
        emit this->updateAffix(typ, affixes[typ]);
        break;
    }
    case 6:
    {
        if (defenses[num]->slow)
        {
            std::thread alreadyInstalled_inst(alreadyInstalled_simulation, this);
            alreadyInstalled_inst.detach();
            break;
        }

        defenses[num]->slow = 1;
        emit this->installAffix(num, affNum, typ);
        affixes[typ]--;
        emit this->updateAffix(typ, affixes[typ]);
        break;
    }
    case 7:
    {
        if (defenses[num]->heal)
        {
            std::thread alreadyInstalled_inst(alreadyInstalled_simulation, this);
            alreadyInstalled_inst.detach();
            break;
        }

        defenses[num]->heal = 1;
        emit this->installAffix(num, affNum, typ);
        affixes[typ]--;
        emit this->updateAffix(typ, affixes[typ]);
        break;
    }
    case 8:
    {
        if (defenses[num]->critical)
        {
            std::thread alreadyInstalled_inst(alreadyInstalled_simulation, this);
            alreadyInstalled_inst.detach();
            break;
        }

        defenses[num]->critical = 1;
        emit this->installAffix(num, affNum, typ);
        affixes[typ]--;
        emit this->updateAffix(typ, affixes[typ]);
        break;
    }
    default:
        break;
    }
}

void Simulation::removeAffixClicked(int num, int affNum, int typ)
{
    qDebug() << "remove: " << num << ' ' << affNum << ' ' << typ;

    switch(typ)
    {
    case 1:
        defenses[num]->mad = 0;
        defenses[num]->atk /= 2, defenses[num]->cd *= 2;
        break;
    case 2:
        defenses[num]->freeze = 0;
        break;
    case 3:
        defenses[num]->triple = 0;
        defenses[num]->targetNum = 1;
        break;
    case 4:
        defenses[num]->bleed= 0;
        break;
    case 5:
        defenses[num]->saint = 0;
        break;
    case 6:
        defenses[num]->slow = 0;
        break;
    case 7:
        defenses[num]->heal = 0;
        break;
    case 8:
        defenses[num]->critical = 0;
        break;
    default:
        break;
    }

    emit this->removeAffix(num, affNum, defenses[num]->attackType);
    affixes[typ]++;
    emit this->updateAffix(typ, affixes[typ]);
}

void Simulation::removeClicked(int num, int typ)
{
    defenses[num]->hp = 0;
    emit this->eliminateDefense(num);

    money += typ == 0 ? 250 : 500;
    emit this->updateMoney(money);
}

void Simulation::moneyNotEnough_simulation(Simulation* sim)
{
    emit sim->moneyNotEnough();
    Sleep(1000);
    emit sim->closeMessage();
    return;
}

void Simulation::blocked_simulation(Simulation* sim)
{
    emit sim->blocked();
    Sleep(1000);
    emit sim->closeMessage();
    return;
}

void Simulation::affixNotEnough_simulation(Simulation* sim)
{
    emit sim->affixNotEnough();
    Sleep(1000);
    emit sim->closeMessage();
    return;
}

void Simulation::alreadyInstalled_simulation(Simulation* sim)
{
    emit sim->alreadyInstalled();
    Sleep(1000);
    emit sim->closeMessage();
    return;
}

void Simulation::remake_simulation(Simulation *sim)
{
    Sleep(2000);
    emit sim->remake();

    qDebug() << "remake";
}
