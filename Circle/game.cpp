#include "game.h"
#include <QDebug>
#include <QQueue>
#include <ctime>

Game::Game()
{
    size = QSize(600, 600);
    segment = 20;
    diag = 2.5*segment;
    ny = 25;
    nx = 7;

    bonus.isHear = false;
    bonus.hasBeenUp = false;
    bonus.time = 0;

    fireStone.velocity = 9;
    fireStone.inAir = false;
    fireStone.aim = QPoint(0, 0);
    fireStone.time = 0;
    fireStone.timeInAir = 0;
    fireStone.damage = 25;

    openMap();
}

void Game::openMap()
{
    QFile file(":Maps/circleTestMap.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        stream >> nx >> ny;
        land = new int*[ny];
        rombCenters = new QPointF*[ny];
        for (int i = 0; i < ny; i++)
        {
            land[i] = new int[nx];
            rombCenters[i] = new QPointF[nx];
        }
        for (int i = 0; i < ny; i++)
            for (int j = 0; j < nx; j++)
            {
                double flag = 0;
                if (i%2)
                    flag = 1;
                QPointF p[4] = {
                    QPointF((j+flag/2)*diag*2, (i/2+flag/2)*diag),
                    QPointF((j+0.5+flag/2)*diag*2, (i/2+0.5+flag/2)*diag),
                    QPointF((j+flag/2)*diag*2, (i/2+1+flag/2)*diag),
                    QPointF((j-0.5+flag/2)*diag*2, (i/2+0.5+flag/2)*diag),
                };
                rombCenters[i][j] = (p[2] + p[0]) / 2;
            }
        for (int i = 0; i < ny; i++)
            for(int j = 0; j < nx; j++)
            {
                double flag = 0;
                stream >> land[i][j];
                if (land[i][j] == -2)
                {
                    if (i%2)
                        flag = 1;
                    Tower t;
                    t.pos = QPointF((2*j + flag)*diag, (i/2+flag/2+0.5)*diag);
                    t.romb = QPoint(j, i);
                    t.radius = 200;
                    t.aim = 0;
                    t.velocity = QVector2D(0, 0);
                    t.tau = 9;
                    t.grenadePos = t.pos - QPointF(-5, 60);
                    t.grenadeInAir = false;
                    t.time = 0;
                    sauron.push_back(t);
                }
            }
        int i, j;
        stream >> i >> j;
        stream >> countBot;
//        qDebug() << countBot;
        adam = new Hero[countBot];
        adam[0].damage = 10;
        adam[0].healthMax = 150;
        adam[0].health = 150;
        adam[0].exp = 0;
        adam[0].expMax = 3;
        adam[0].pos = QPointF(size.width()/2, size.height()/2);
        adam[0].location = rombCenters[i][j];
        adam[0].spawn = adam[0].location;
        adam[0].velocity = 5;
        adam[0].isGoing = false;
        adam[0].number = 0;
        adam[0].attackSpeed = 40;
        adam[0].newWay = QPoint(-1, -1);
        adam[0].armor = 2;
        adam[0].pathSize = 0;
        adam[0].stop[0] = rombCenters[heroRomb(adam[0].number).x()][heroRomb(adam[0].number).y()];
        adam[0].stop[1] = QPoint(-1, -1);
        adam[0].inAttack = false;
        adam[0].isDead = false;
        adam[0].agility = 25;
        adam[0].level = 0;
        adam[0].level2 = 0;
        adam[0].bonusTime = 0;
        adam[0].changePix = 1;
        for (int t = 1; t < countBot; t++)
        {
//            qDebug() << "DIoA";
            int i, j;
            stream >> i >> j;
            adam[t].spawnTime = 500;
            adam[t].pos = rombCenters[i][j];
            adam[t].spawn = adam[t].pos;
            adam[t].healthMax = 60;
            adam[t].health = 60;
            adam[t].exp = 0;
            adam[t].expMax = 2;
            adam[t].damage = 7;
            adam[t].location = QPointF(size.width()/2, size.height()/2);
            adam[t].velocity = 5;
            adam[t].isGoing = false;
            adam[t].number = t;
            adam[t].newWay = QPoint(-1, -1);
            adam[t].armor = 1;
            adam[t].attackSpeed = 40;
            adam[t].pathSize = 0;
            adam[t].aim = -1;
            adam[t].inAttack = false;
            adam[t].distanceToAim = 100000;
            adam[t].stop[0] = rombCenters[i][j];
            adam[t].stop[1] = QPoint(-1, -1);
            adam[t].isDead = false;
            adam[t].agility = 25;
            adam[t].level = 0;
            adam[t].level2 = 0;
            adam[t].bonusTime = 0;
            adam[t].changePix = 1;
        }
    }
}

void Game::restart()
{
    adam[0].damage = 10;
    adam[0].healthMax = 150;
    adam[0].health = 150;
    adam[0].exp = 0;
    adam[0].expMax = 3;
    adam[0].pos = QPointF(size.width()/2, size.height()/2);
    adam[0].location = adam[0].spawn;
    adam[0].velocity = 5;
    adam[0].isGoing = false;
    adam[0].number = 0;
    adam[0].attackSpeed = 40;
    adam[0].newWay = QPoint(-1, -1);
    adam[0].armor = 1;
    adam[0].pathSize = 0;
    adam[0].stop[0] = rombCenters[heroRomb(adam[0].number).x()][heroRomb(adam[0].number).y()];
    adam[0].stop[1] = QPoint(-1, -1);
    adam[0].inAttack = false;
    adam[0].isDead = false;
    adam[0].agility = 25;
    adam[0].level = 0;
    adam[0].level2 = 2;
    adam[0].bonusTime = 0;
    for (int t = 1; t < countBot; t++)
    {
//            qDebug() << "DIoA";
        adam[t].spawnTime = 500;
        adam[t].pos = adam[t].spawn;
        adam[t].healthMax = 60;
        adam[t].health = 60;
        adam[t].exp = 0;
        adam[t].expMax = 2;
        adam[t].damage = 7;
        adam[t].location = QPointF(size.width()/2, size.height()/2);
        adam[t].velocity = 5;
        adam[t].isGoing = false;
        adam[t].number = t;
        adam[t].newWay = QPoint(-1, -1);
        adam[t].armor = 0;
        adam[t].attackSpeed = 40;
        adam[t].pathSize = 0;
        adam[t].aim = -1;
        adam[t].inAttack = false;
        adam[t].distanceToAim = 100000;
        adam[t].stop[0] = rombCenters[heroRomb(t).x()][heroRomb(t).y()];
        adam[t].stop[1] = QPoint(-1, -1);
        adam[t].isDead = false;
        adam[t].agility = 25;
        adam[t].level = 0;
        adam[t].level2 = 2;
    }
    fireStone.time = 0;
    bonus.isHear = false;
    bonus.hasBeenUp = false;
    bonus.time = 0;
    for (int i = 0; i < sauron.size(); i++)
    {
        sauron[i].grenadePos = sauron[i].pos - QPointF(-5, 60);
        sauron[i].time = 0;
    }
}

void Game::bfs(const QPoint &start, const QPoint &end, const int &t)
{
    if(adam[t].isDead)
        return;

    path.clear();
    if(land[start.x()][start.y()] < 0 || land[end.x()][end.y()] < 0 || start == end)
        return;

    adam[t].stage = 0;
    int dist[ny][nx];
    int used[ny][nx];
    QPoint parent[ny][nx];

    int x = end.x();
    int y = end.y();

    int ni[8] = {-1, 1, -1, 1, 0, 2, -2,  0}; // если нечетная строка (0 - индексация)
    int nj[8] = { 0, 0,  1, 1, 1, 0,  0, -1};

    int di[8] = {-1, -1,  1,  1, -2, 2,  0,  0}; // если четная строка (0 - индексация)
    int dj[8] = {-1,  0,  0, -1,  0, 0,  1, -1};

    for(int i = 0; i < ny; i++)
        for(int j = 0; j < nx; j++)
        {
            used[i][j] = 0;
            dist[i][j] = 0;
        }

    QQueue<QPoint> q;
    q.push_back(end);
    parent[x][y].setX(-1);
    parent[x][y].setY(-1);
    dist[x][y] = 0;
    used[x][y] = 1;

    for (int s = 0; s < countBot; s++)
        if (t != s && !adam[s].isDead && QVector2D(adam[t].location + adam[t].pos - adam[s].location - adam[s].pos).length() <= 5*diag)
            used[heroRomb(s).x()][heroRomb(s).y()] = 1;

    while(!q.empty())
    {
        QPoint from = q.front();
        q.pop_front();
        for(int i = 0; i < 8; i++)
        {
            QPoint to;
            if(from.x() % 2 != 0)
            {
                to.setX(from.x() + ni[i]);
                to.setY(from.y() + nj[i]);
            }
            else
            {
                to.setX(from.x() + di[i]);
                to.setY(from.y() + dj[i]);
            }
            if(used[to.x()][to.y()] == 0 && to.y() < nx && to.y() >= 0 && to.x() < ny && to.x() >= 0 && land[to.x()][to.y()]>=0)
            {
                parent[to.x()][to.y()] = from;
                dist[to.x()][to.y()] = dist[from.x()][from.y()] + 1;
                used[to.x()][to.y()] = 1;
                q.push_back(to);
            }
        }
    }

    if(dist[start.x()][start.y()] == 0)
        return;

    int i = start.x(), j = start.y();
    while(i != -1 && j != -1)
    {
        path.push_back(rombCenters[i][j]);
        int x = i, y = j;
        i = parent[x][y].x();
        j = parent[x][y].y();
    }
    adam[t].pathSize = path.size();
    for(int i = 0; i < adam[t].pathSize; i++)
    {
        adam[t].pathPoints[i] = path[i];
    }
}

void Game::moveHero(int &t)
{
    if(adam[t].isDead)
    {
        return;
    }
    if(adam[t].pathSize == 0 || adam[t].location + adam[t].pos - QPointF(size.width()/2, size.height()/2) ==
            adam[t].pathPoints[adam[t].pathSize-1])
    {
        adam[t].isGoing = false;
        adam[t].pathSize = 0;
        adam[t].stop[0] = rombCenters[heroRomb(t).x()][heroRomb(t).y()];
        adam[t].stop[1] = QPointF(-1, -1);
        return;
    }

    adam[t].stop[0] = adam[t].pathPoints[adam[t].stage];
    adam[t].stop[1] = adam[t].pathPoints[adam[t].stage+1];

    for (int i = 0; i < countBot; i++)
        if (t != i && (adam[i].stop[0] == adam[t].pathPoints[adam[t].stage+1]
                    || adam[i].stop[1] == adam[t].pathPoints[adam[t].stage+1]))
        {
            adam[t].isGoing = false;
            adam[t].pathSize = 0;
            if (t != 0)
                adam[t].pos = adam[t].pathPoints[adam[t].stage];
            else
            {
                adam[t].location = adam[t].pathPoints[adam[t].stage];
            }
            adam[t].stop[1] = QPoint(-1, -1);
            return;
        }

    if(adam[t].location + adam[t].pos-QPointF(size.width()/2, size.height()/2) == adam[t].pathPoints[adam[t].stage+1])
         adam[t].stage++;
    QPointF l(adam[t].pathPoints[adam[t].stage+1] - adam[t].pathPoints[adam[t].stage]);
    l = l / sqrt((l.x()*l.x() + l.y()*l.y()));
    int tau = 1;
    if (t == 0)
        adam[t].location = adam[t].location + tau * l * adam[t].velocity;
    else
        adam[t].pos = adam[t].pos + tau * l * adam[t].velocity;
    if (QPointF::dotProduct(adam[t].location-adam[t].pathPoints[adam[t].stage+1]+adam[t].pos-QPointF(size.width()/2, size.height()/2),
                           adam[t].location-adam[t].pathPoints[adam[t].stage+1]+adam[t].pos-QPointF(size.width()/2, size.height()/2)) <= 4.5)
    {
        if (t == 0)
            adam[0].location = adam[0].pathPoints[adam[0].stage+1];
        else
            adam[t].pos = adam[t].pathPoints[adam[t].stage+1];
    }
    if (adam[t].newWay.x() > 0 && (adam[t].location+adam[t].pos-QPointF(size.width()/2, size.height()/2) ==
                                   adam[t].pathPoints[adam[t].stage+1]))
    {
//        if (t == 0)
//            qDebug() << "Hey:" << adam[t].newWay << land[adam[t].newWay.x()][adam[t].newWay.y()]
//                    << rombCenters[heroRomb(t).x()][heroRomb(t).y()];
        bfs(heroRomb(t), adam[t].newWay, t);
        adam[t].newWay = QPoint(-1, -1);
    }
//    if (t == 0)
//    {
//        qDebug() << adam[0].stage;
//        qDebug() << adam[0].pathPoints[adam[0].stage] << adam[0].location << adam[0].pathPoints[adam[0].stage+1];
//    }
}

void Game::checkBonus()
{
    if (bonus.time > 0)
        bonus.time--;
    else
    {
        bonus.isHear = false;
        bonus.pos = QPoint(-10, -10);
    }
    if (adam[0].bonusTime > 0)
    {
        adam[0].bonusTime--;
        return;
    }
    else if (!bonus.isHear && bonus.hasBeenUp)
    {
        bonus.num = -bonus.num;
        bonus.hasBeenUp = false;
    }

//    if (adam[0].bonusTime <= 0)
//        checkBonus();

//    qDebug() << QVector2D(bonus.pos - adam[0].location).length();
    if (bonus.pos == adam[0].location)
    {
        adam[0].bonusTime = 250;
        bonus.pos = QPointF(-10, -10);
        bonus.isHear = false;
        bonus.hasBeenUp = true;
    }
    else if (bonus.num < 0)
    {
        if (bonus.num == -1)
            adam[0].velocity -= 2;
        else if (bonus.num == -2)
            adam[0].damage /= 2;
        else if (bonus.num == -3)
            adam[0].armor /= 2;
        bonus.num = 0;
        return;
    }
    else
        return;
    if (bonus.num == 0)
    {
        adam[0].health += 30;
        if (adam[0].health > adam[0].healthMax)
            adam[0].health = adam[0].healthMax;
    }
    else if (bonus.num == 1)
        adam[0].velocity += 2;
    else if (bonus.num == 2)
        adam[0].damage *= 2;
    else if (bonus.num == 3)
        adam[0].armor *= 2;
}


void Game::smartBots()
{
    dealDamage(0, adam[0].aim);

    for (int i = 1; i < countBot; i++)
    {
        if (!adam[i].isDead)
        {
            adam[i].aim = -1;
            adam[i].distanceToAim = 10000;
            for (int j = 0; j < countBot; j++)
                if (!adam[j].isDead)
                {
                    if (i != j && QVector2D(adam[i].pos+adam[i].location - adam[j].pos-adam[j].location).length() < 6*diag)
                    {
                        if (QVector2D(adam[i].pos+adam[i].location - adam[j].pos-adam[j].location).length() <
                                adam[i].distanceToAim)
                        {
                            adam[i].aim = j;
                            adam[i].distanceToAim = QVector2D(adam[i].pos+adam[i].location - adam[j].pos-adam[j].location).length();
                        }
                    }
                }
            dealDamage(i, adam[i].aim);
            if (adam[i].aim >= 0)
            {
                if (!adam[i].isGoing)
                {
                    bfs(heroRomb(i), heroRomb(adam[i].aim), i);
                    adam[i].isGoing = true;
                }
                else if (!adam[i].inAttack)
                    adam[i].newWay = heroRomb(adam[i].aim);
            }
        }
        else
            resurrection(i);
    }
}

void Game::moveFireball()
{

}

QPoint Game::heroRomb(const int &t)
{
    int i = 0, j = 0;
    i = (adam[t].location.y()+adam[t].pos.y()-size.height()/2)/diag;
    i *= 2;
    j = (adam[t].location.x()+adam[t].pos.x()-size.width()/2+diag)/(diag*2);
    double y1 = 0, y2 = 0, x1 = 0, x2 = 0;
    x1 = (j+0.5)*diag*2, y1 = (i/2+0.5)*diag;
    x2 = j*diag*2, y2 = (i/2+1)*diag;
    if (adam[t].location.y()+adam[t].pos.y()-size.height()/2 >
            (y1-y2)/(x1-x2)*(adam[t].location.x()+adam[t].pos.x()-size.width()/2)-(y1-y2)/(x1-x2)*x1+y1)
        i++;
    else if (adam[t].location.y()+adam[t].pos.y()-size.height()/2 <
             (y1-y2)/(x1-x2)*(adam[t].location.x()+adam[t].pos.x()-size.width()/2)-(y1-y2)/(x1-x2)*(x1-diag)+y1-diag/2
             && i > 0 && j > 0)
        i--, j--;
    else
    {
        x1 = j*diag*2, y1 = (i/2+1)*diag;
        x2 = (j-0.5)*diag*2, y2 = (i/2+0.5)*diag;
        if (adam[t].location.y()+adam[t].pos.y()-size.height()/2 >
                (y1-y2)/(x1-x2)*(adam[t].location.x()+adam[t].pos.x()-size.width()/2)-(y1-y2)/(x1-x2)*x1+y1 && j > 0)
            i++, j--;
        else if (adam[t].location.y()+adam[t].pos.y()-size.height()/2 <
                 (y1-y2)/(x1-x2)*(adam[t].location.x()+adam[t].pos.x()-size.width()/2)-(y1-y2)/(x1-x2)*(x1+diag)+y1-diag/2
                 && i > 0)
            i--;
    }
    //qDebug() << t << j;
    return QPoint(i, j);
}

bool Game::checkTower(Tower &t)
{
    if (t.aim >= 0 && !adam[t.aim].isDead &&
            QVector2D(t.pos - adam[t.aim].pos - adam[t.aim].location +
                      QPointF(size.width()/2, size.height()/2)).length() <= t.radius)
        return true;

    for (int i = 0; i < countBot; i++)
    {
        if (!adam[i].isDead && QVector2D(t.pos-adam[i].pos-adam[i].location + QPointF(size.width()/2, size.height()/2)).length()
                <= t.radius)
        {
            t.aim = i;
            return true;
        }
    }
    return false;
}

double Game::getDiag()
{
    return diag;
}

int Game::getHP(const int &i)
{
    return adam[i].health;
}

void Game::meteorDamage()
{
//    QVector2D(t.grenadePos-(igra.adam[0].pos + igra.adam[0].location - QPoint(igra.size.width()/2, 30+igra.size.height()/2))).length() < 20
    for (int i = 0; i < countBot; i++)
    {
        QVector2D l(fireStone.pos - adam[i].pos-adam[i].location + QPointF(size.width()/2, size.height()/2));
        if(l.length() < diag)
        {
            if(adam[i].health - fireStone.damage <= 0)
                adam[i].health = 0;
            else
                adam[i].health-= (fireStone.damage + 5 * adam[0].level);
        }
        //checkPulse();
    }
}

void Game::dealDamage(int from, int to)
{
    if (to < 0)
        return;
    int ni[8] = {-1, 1, -1, 1, 0, 2, -2,  0}; // если нечетная строка (0 - индексация)
    int nj[8] = { 0, 0,  1, 1, 1, 0,  0, -1};

    int di[8] = {-1, -1,  1,  1, -2, 2,  0,  0}; // если четная строка (0 - индексация)
    int dj[8] = {-1,  0,  0, -1,  0, 0,  1, -1};

//    qDebug() << "START";
//    qDebug() << heroRomb(from) << heroRomb(to);
//    for(int i = 0; i < 8; i++)
//    {
//        if(heroRomb(to).y() % 2 != 0)
//            qDebug() << heroRomb(from) + QPoint(ni[i], nj[i]);
//        if(heroRomb(to).y() % 2 == 0)
//            qDebug() << heroRomb(from) + QPoint(di[i], dj[i]);
//    }
//    qDebug() << "END";


    adam[from].time++;
    if(adam[from].isGoing || adam[to].isDead)
    {
      //  qDebug() << "hi";
        adam[from].inAttack = false;
        adam[from].time = 0;
        adam[from].changePix = 1;
    }

    if(adam[from].time <= adam[from].attackSpeed / 2 && adam[from].inAttack)
    {
        adam[from].changePix = 0;
//        qDebug() << "DOWN";
    }
    else if(adam[from].time > adam[from].attackSpeed / 2 && adam[from].inAttack)
    {
        adam[from].changePix = 1;
//        qDebug() << "UP";
    }

    if(adam[from].time >= adam[from].attackSpeed)
    {
        for(int i = 0; i < 8; i++)
        {
            if(heroRomb(from).x() % 2 != 0 && heroRomb(to) == heroRomb(from) + QPoint(ni[i], nj[i]))
            {
                adam[from].changePix = 0;
                adam[from].inAttack = true;
                if(adam[to].health - (adam[from].damage - adam[from].armor) <= 0)
                    adam[to].health = 0;
                else
                {
                    if(adam[from].damage - adam[to].armor >= 1)
                        adam[to].health -= (adam[from].damage - adam[to].armor);
                    else
                        adam[to].health -= 1;
                }
            }
            else if(heroRomb(to) == heroRomb(from) + QPoint(di[i], dj[i]))
            {
                adam[from].changePix = 0;
                adam[from].inAttack = true;
                if(adam[to].health - (adam[from].damage - adam[from].armor) <= 0)
                    adam[to].health = 0;
                else
                {
                    if(adam[from].damage - adam[to].armor >= 1)
                        adam[to].health -= (adam[from].damage - adam[to].armor);
                    else
                        adam[to].health -= 1;
                }
            }
        }
        adam[from].time = 0;
      //  qDebug << adam[from].time;
      //  adam[from].inAttack = false;
        //checkPulse();
    }
    //qDebug() << adam[from].changePix;

}

void Game::resurrection(const int &t)
{
    if (adam[t].spawnTime > 0)
    {
        adam[t].spawnTime--;
        return;
    }
    for (int i = 0; i < countBot; i++)
        if (i != t && (adam[i].stop[0] == heroRomb(t) || adam[i].stop[1] == heroRomb(t)))
            return;
    adam[t].spawnTime = 500;
    adam[t].health = adam[t].healthMax;
    adam[t].isDead = false;
    adam[t].stop[0] = heroRomb(t);
}

void Game::checkPulse()
{
    for(int t = 0; t < countBot; t++)
    {
        if(adam[t].health <= 0 && !adam[t].isDead)
        {
            for(int j = 0; j < countBot; j++)
                if(QVector2D(adam[t].pos+adam[t].location - adam[j].pos-adam[j].location).length() < 5 * diag &&
                        adam[j].isDead == false)
                {
//                    qDebug() << QVector2D(adam[t].pos+adam[t].location - adam[j].pos-adam[t].location).length() <<
//                                t << ":" << heroRomb(t) << j << ":" << heroRomb(j) << 3*diag;
                    adam[j].exp++;
                    if(adam[j].exp == adam[j].expMax)
                    {
                        adam[j].level2++;
                        adam[j].level++;
                        adam[j].expMax++;
                        adam[j].exp = 0;
                    }
                }
            if (bonus.time <= 0 && adam[0].bonusTime <= 0)
            {
                bonus.isHear = true;
                bonus.pos = adam[t].pos;
                srand( time( 0 ) );
                bonus.num = rand()%4;
                bonus.time = 250;
            }
            adam[t].isDead = true;
            adam[t].newWay = QPoint(-1, -1);
            adam[t].stop[0] = QPointF(-1,-1);
            adam[t].stop[1] = QPointF(-1,-1);
            adam[t].pos = adam[t].spawn;
            adam[t].inAttack = false;
            adam[t].isGoing = false;
            adam[t].pathSize = 0;
//            adam[t].pos = QPointF(-3*size.width(), -3*size.height());
        }
    }
}

void Game::upHP(int t)
{
    if(t == 0)
    {
        adam[t].healthMax += 10 * adam[t].level;
        adam[t].health += 10 * adam[t].level / 2;
    }
    else
    {
        adam[t].healthMax += 5 * adam[t].level;
        adam[t].health += 5 * adam[t].level / 2;
    }

}

void Game::upAttackSpeed(int t)
{
    if(adam[t].agility <= 45)
    {
        adam[t].attackSpeed -= 1;
        adam[t].agility++;
    }
}

void Game::upDamage(int t)
{
    if(t == 0)
    {
        if (bonus.num == 2)
            adam[t].damage += 3;
        adam[t].damage += 3;
    }
    else
        adam[t].damage++;
}

void Game::upArmor(int t)
{
    if(t == 0)
    {
        if (bonus.num == 3)
            adam[t].armor += 2;
        adam[t].armor += 2;
    }
    adam[t].armor++;
}

int Game::getAgl()
{
    return adam[0].agility;
}

int Game::getDmg()
{
    return adam[0].damage;
}

int Game::getArmr()
{
    return adam[0].armor;
}

int Game::getLVL()
{
    return adam[0].level;
}

int Game::getLVL2()
{
    return adam[0].level2;
}

void Game::changeLVL2()
{
    if(adam[0].level2 > 0)
        adam[0].level2--;
}

void Game::upgradeBots()
{
    for(int i = 1; i < countBot; i++)
    {
        if(adam[i].level2 > 0)
        {
            upDamage(i);
            upArmor(i);
            upHP(i);
            upAttackSpeed(i);
            //qDebug() << i << adam[i].damage << adam[i].armor << adam[i].healthMax << adam[i].attackSpeed;
            adam[i].level2--;
        }
    }
}

int Game::getBonusTime()
{
    return adam[0].bonusTime;
}

Game::~Game()
{
    sauron.clear();
    path.clear();
//    for(int i = 0; i < ny; i++)
//        delete[] adam;
//    {
//        delete[] rombCenters[i];
//        delete[] land[i];
 //   }
//    delete[] rombCenters;
//    delete[] land;
}
