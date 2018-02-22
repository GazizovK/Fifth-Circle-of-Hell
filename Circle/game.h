#ifndef GAME_H
#define GAME_H

#include <QSize>
#include <QPointF>
#include <QFile>
#include <QVector>
#include <QVector2D>

struct Hero
{
    int health;
    int healthMax;
    int damage;
    QPointF spawn;
    QPointF stop[2];
    int aim;
    double distanceToAim;
    int radius;
    int velocity;
    QPointF pos;
    QPointF location;
    int stage;
    bool isGoing;
    int number;
    int attackSpeed;
    int time;
    int spawnTime;
    int armor;
    QPoint newWay;
    QPointF pathPoints[150];
    int pathSize;
    bool inAttack;
    bool isDead;
    int exp;
    int expMax;
    int level;
    int level2;
    int agility;
    int bonusTime;

    int changePix;
};

struct Tower
{
    double radius;
    QPointF pos;
    QPoint romb;
    QPointF grenadePos;
    QVector2D velocity;
    int tau;
    int time;
    bool grenadeInAir;
    int aim;
};

struct Meteor
{
    QPointF pos;
    int damage;
    double radius;
    QPointF location;
    int velocity;
    bool inAir;
    QPointF aim;
    int time;
    int timeInAir;
};

struct Loot
{
    int num;
    QPointF pos;
    bool isHear;
    bool hasBeenUp;
    int time;
};

class Game
{
    int segment;
    int nx, ny;
    int **land;
    QVector<Tower> sauron;
    QVector<QPointF> path;
    double diag;
    QSize size;
    QPointF **rombCenters;
    Hero *adam;
    int countBot;
    Meteor fireStone;
    Loot bonus;
    void bfs(const QPoint&, const QPoint&, const int&);
    void moveHero(int &);
    void openMap();
    void printPath(QPoint);
    bool checkTower(Tower &);
    void moveFireball();
    void meteorDamage();
    void dealDamage(int,int);
public:
    void checkPulse();
    void checkBonus();
    void resurrection(const int &);

    void restart();

    void upgradeBots();
    void upHP(int);
    void upAttackSpeed(int);
    void upDamage(int);
    void upArmor(int);

    int getBonusTime();
    int getDmg();
    int getAgl();
    int getArmr();
    int getLVL();
    int getLVL2();

    void changeLVL2();


    void smartBots();
    QPoint heroRomb(const int &);
    Game();
    ~Game();

    double getDiag();
    int getHP(const int &);
    friend class Label;
};

#endif // GAME_H
