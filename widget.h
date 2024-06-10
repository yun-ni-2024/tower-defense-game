#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include "Function.h"
#include "Map.h"
#include "Simulation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

private:
    Ui::Widget *ui;
    QSoundEffect* bgm, * bgm1, * bgm2;
    QSoundEffect* sound1, * sound2;
    QLabel* Catalogue;
    QLabel* EditWindow;
    QLabel* SetUpWindow;
    QLabel* ExtenWindow;
    vector <Map*> maps;
    vector <string> name;
    Simulation* S;
    QLabel* background;
    QPushButton*** graph;
    QPushButton** enemyGraph;
    QPushButton** defenseGraph;
    QPushButton** enemyHpGraph;
    QPushButton** defenseHpGraph;
    QPushButton** aff[2];
    QPushButton*** defenseBulletGraph, *** defenseClawGraph, *** enemyBulletGraph, *** enemyClawGraph;
    QPushButton** bloodGraph;
    QPushButton** slowGraph;
    QPushButton* moneyIcon;
    QPushButton* messageBoard;
    QPushButton* heartIcon;
    QPushButton* affixIcon[10];
    QPushButton* chooseBoard;

    Path tmpPath;
    vector <Path> tmpPaths;
    int** editData;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void fileRead();
    void fileWrite();
    void catalogueInit();
    void catalogueUpdate();
    void editWindowInit();
    void setUpInit();
    void extenInit();
    void load(Map* map);

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
    void updateEnemyHp(int num, int hp_len);
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
#endif // WIDGET_H
