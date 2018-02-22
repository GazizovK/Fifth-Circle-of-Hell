#include "label.h"
#include <QPainter>
#include <QDebug>
#include <QVector2D>

Label::Label(QWidget *parent) :
    QLabel(parent)
{
    pix = QPixmap(":/pictures/001.png");

    pixDown[0] = QPixmap(":/pictures/001.png");
    pixUp[0] = QPixmap(":/pictures/005.png");
    pixRight[0] = QPixmap(":/pictures/003.png");
    pixLeft[0] = QPixmap(":/pictures/007.png");
    pixUpLeft[0] = QPixmap(":/pictures/006.png");
    pixDownLeft[0] = QPixmap(":/pictures/008.png");
    pixUpRight[0] = QPixmap(":/pictures/004.png");
    pixDownRight[0] = QPixmap(":/pictures/002.png");

    pixDown[1] = QPixmap(":/pictures/mech1.png");
    pixUp[1] = QPixmap(":/pictures/mech5.png");
    pixRight[1] = QPixmap(":/pictures/mech3.png");
    pixLeft[1] = QPixmap(":/pictures/mech7.png");
    pixUpLeft[1] = QPixmap(":/pictures/mech6.png");
    pixDownLeft[1] = QPixmap(":/pictures/mech8.png");
    pixUpRight[1] = QPixmap(":/pictures/mech4.png");
    pixDownRight[1] = QPixmap(":/pictures/mech2.png");

    shiftNumber = 1;
    shift[0][0] = QPoint(-14, -16);
    shift[0][1] = QPoint(20, -13);
    shift[0][2] = QPoint(20, -13);
    shift[0][3] = QPoint(15, -15);
    shift[0][4] = QPoint(0, -14);
    shift[0][7] = QPoint(18, -20);
    shift[0][5] = QPoint(24, -20);
    shift[0][6] = QPoint(1, -12);

    shift[1][0] = QPoint(13, -16);
    shift[1][1] = QPoint(20, -13);
    shift[1][2] = QPoint(20, -13);
    shift[1][3] = QPoint(15, -15);
    shift[1][4] = QPoint(0, -14);
    shift[1][7] = QPoint(27, -20);
    shift[1][5] = QPoint(24, -20);
    shift[1][6] = QPoint(5, -12);
}

void Label::setSize(const Game &a)
{
    pixmap = new QPixmap(a.size);
}

void Label::drawLand(Game &a)
{

    igra = a;
    pixmap->fill(Qt::darkBlue);
    QPainter painter(pixmap);
    painter.drawPixmap(0, 0, QPixmap(":/pictures/water.png"));
    for (int i = 0; i < a.ny; i++)
        for (int j = 0; j < a.nx; j++)
        {
            double flag = 0;
            if (i%2)
                flag = 1;

            if (a.land[i][j] == 0 || a.land[i][j] == -4 || a.land[i][j] == -5 || a.land[i][j] == -6)
            {
                QPixmap pix(":/pictures/grass.png");
                painter.drawPixmap((j-0.5+flag/2)*a.diag*2-a.adam[0].location.x()+a.size.width()/2,
                                   (i/2+flag/2)*a.diag-a.adam[0].location.y()+a.size.height()/2, pix);
            }
            else if (abs(a.land[i][j]) == 2 || a.land[i][j] == -3 || a.land[i][j] == 1 || a.land[i][j] == -10)
            {
                QPixmap pix(":/pictures/sand.png");
                painter.drawPixmap((j-0.5+flag/2)*a.diag*2-a.adam[0].location.x()+a.size.width()/2,
                                   (i/2+flag/2)*a.diag-a.adam[0].location.y()+a.size.height()/2, pix);
            }
            else if (a.land[i][j] >= -9 && a.land[i][j] <= -7)
            {
                QPixmap pix(":/pictures/sand.png");
                painter.drawPixmap((j-0.5+flag/2)*a.diag*2-a.adam[0].location.x()+a.size.width()/2,
                                   (i/2+flag/2)*a.diag-a.adam[0].location.y()+a.size.height()/2, pix);
            }
            else if (a.land[i][j] == -1)
            {
                QPixmap pix(":/pictures/water~01.png");
                painter.drawPixmap((j-0.5+flag/2)*a.diag*2-a.adam[0].location.x()+a.size.width()/2,
                                   (i/2+flag/2)*a.diag-a.adam[0].location.y()+a.size.height()/2, pix);
            }
        }
    if(a.fireStone.inAir)
    {
        int tau = a.size.height() / a.fireStone.velocity / 2 + 1;
        double delta = double(a.fireStone.timeInAir) / (double)tau;
        painter.setBrush(Qt::black);
//        qDebug() << tau << "time" << delta;
        painter.drawEllipse(a.fireStone.aim-a.adam[0].location+QPointF(igra.size.width()/2, igra.size.height()/2),
                25 * delta, 10 * delta);
    }
    drawItems(a, painter);
    for (int i = 0; i < a.sauron.size(); i++)
        drawGrenade(a.sauron[i], painter, a);
    setPixmap(*pixmap);
}

void Label::drawItems(Game &a, QPainter &painter)
{
    QPoint b[a.countBot];
    igra.bonus = a.bonus;
    for (int i = 0; i < a.countBot; i++)
        b[i] = a.heroRomb(i);
    for (int i = 0; i < a.ny; i++)
        for (int j = 0; j < a.nx; j++)
        {
            double flag = 0;
            if (i%2)
                flag = 1;
            if (a.land[i][j] == -2)
            {
                QPixmap pix(":/pictures/tower.png");
                painter.drawPixmap((j+flag/2)*a.diag*2-a.adam[0].location.x()+a.size.width()/2-32,
                                   (i/2+0.5+flag/2)*a.diag-a.adam[0].location.y()+a.size.height()/2-65, pix);
                painter.setBrush(Qt::yellow);
                painter.drawEllipse((j+flag/2)*a.diag*2-a.adam[0].location.x()+a.size.width()/2-2,
                                    (i/2+0.5+flag/2)*a.diag-a.adam[0].location.y()+a.size.height()/2-65, 10, 10);
            }
            else if (a.land[i][j] == 2)
            {
                QPixmap pix(":/pictures/bones.png");
                painter.drawPixmap((j+flag/2)*a.diag*2-a.adam[0].location.x()+a.size.width()/2-29,
                                   (i/2+0.5+flag/2)*a.diag-a.adam[0].location.y()+a.size.height()/2-20, pix);

            }
            else if (a.land[i][j] == -3)
            {
                QPixmap pix(":/pictures/tree.png");
                painter.drawPixmap((j+flag/2)*a.diag*2-a.adam[0].location.x()+a.size.width()/2-29,
                                   (i/2+0.5+flag/2)*a.diag-a.adam[0].location.y()+a.size.height()/2-69, pix);
            }
            else if (a.land[i][j] == -4)
            {
                QPixmap pix(":/pictures/tower1.png");
                painter.drawPixmap((j+flag/2)*a.diag*2-a.adam[0].location.x()+a.size.width()/2-42,
                                   (i/2+0.5+flag/2)*a.diag-a.adam[0].location.y()+a.size.height()/2-69, pix);
            }
            else if (a.land[i][j] == -5)
            {
                QPixmap pix(":/pictures/tower4.png");
                painter.drawPixmap((j+flag/2)*a.diag*2-a.adam[0].location.x()+a.size.width()/2-30,
                                   (i/2+0.5+flag/2)*a.diag-a.adam[0].location.y()+a.size.height()/2-58, pix);
            }
            else if (a.land[i][j] == -6)
            {
                QPixmap pix(":/pictures/tower3.png");
                painter.drawPixmap((j+flag/2)*a.diag*2-a.adam[0].location.x()+a.size.width()/2-40,
                                   (i/2+0.5+flag/2)*a.diag-a.adam[0].location.y()+a.size.height()/2-60, pix);
            }
            else if (a.land[i][j] == -7)
            {
                QPixmap pix(":/pictures/stone3.png");
                painter.drawPixmap((j+flag/2)*a.diag*2-a.adam[0].location.x()+a.size.width()/2-48,
                                   (i/2+0.5+flag/2)*a.diag-a.adam[0].location.y()+a.size.height()/2-50, pix);
            }
            else if (a.land[i][j] == -8)
            {
                QPixmap pix(":/pictures/stone2.png");
                painter.drawPixmap((j+flag/2)*a.diag*2-a.adam[0].location.x()+a.size.width()/2-43,
                                   (i/2+0.5+flag/2)*a.diag-a.adam[0].location.y()+a.size.height()/2-48, pix);
            }
            else if (a.land[i][j] == -9)
            {
                QPixmap pix(":/pictures/stone1.png");
                painter.drawPixmap((j+flag/2)*a.diag*2-a.adam[0].location.x()+a.size.width()/2-42,
                                   (i/2+0.5+flag/2)*a.diag-a.adam[0].location.y()+a.size.height()/2-53, pix);
            }

            if (a.bonus.pos == a.rombCenters[i][j] && a.bonus.isHear)
                drawLoot(painter);

            for (int t = 0; t < a.countBot; t++)
                if (b[t].x() == i && b[t].y() == j && !igra.adam[t].isDead)
                    drawHero(painter, t, a.adam[t].changePix);
        }
    for (int s = 0; s < a.countBot; s++)
        igra.moveHero(s);
    a.adam = igra.adam;
}

void Label::drawGrenade(Tower& t, QPainter &painter, Game &a)
{
    if (t.time > 0)
    {
        t.time--;
        return;
    }
    QPixmap pix(":/pictures/fireball");
    if (t.velocity == QVector2D(0, 0) && a.checkTower(t))
    {
        t.grenadePos = t.pos-QPointF(-5, 70);
        QPointF b(igra.adam[t.aim].pos + igra.adam[t.aim].location - QPoint(igra.size.width()/2, 30+igra.size.height()/2));
        t.velocity = QVector2D(b-t.grenadePos).normalized();

//        qDebug() << b << t.velocity << endl;
    }
//    qDebug() << t.grenadePos;
    if (t.velocity != QVector2D(0, 0))
    {
        painter.drawPixmap(t.grenadePos-igra.adam[0].location+QPointF(igra.size.width()/2, igra.size.height()/2), pix);
        t.grenadePos = t.grenadePos + t.velocity.toPointF()*t.tau;
    }
    if (QVector2D(t.grenadePos-t.pos+QPointF(-5, 70)).length() > t.radius + 40)
        t.velocity = QVector2D(0,0);
//    for (int i = 0; i < ; i++)
    if (QVector2D(t.grenadePos-(igra.adam[t.aim].pos + igra.adam[t.aim].location -
                                QPoint(igra.size.width()/2, 30+igra.size.height()/2))).length() < 20 && t.velocity != QVector2D(0, 0))
    {
        if(a.adam[t.aim].health - 5 <= 0)
            a.adam[t.aim].health = 0;
        else
            a.adam[t.aim].health -= 5;
        t.velocity = QVector2D(0, 0);
        t.time = 15;
    }
    setPixmap(*pixmap);
}

void Label::drawHero(QPainter &painter, const int &t, int i)
{
    painter.setBrush(Qt::red);
    QPointF l(igra.adam[t].pathPoints[igra.adam[t].stage+1] - igra.adam[t].pathPoints[igra.adam[t].stage]);
    if(l.x() == 0 && l.y() > 0)
    {
        pix = pixDown[i];
        shiftNumber = 0;
    }
    if(l.x() == 0 && l.y() < 0)
    {
        pix = pixUp[i];
        shiftNumber = 1;
    }
    if(l.x() > 0 && l.y() == 0)
    {
        pix = pixRight[i];
        shiftNumber = 2;
    }
    if(l.x() < 0 && l.y() == 0)
    {
        pix = pixLeft[i];
        shiftNumber = 3;
    }
    if(l.x() > 0 && l.y() > 0)
    {
        pix = pixDownRight[i];
        shiftNumber = 4;
    }
    if(l.x() > 0 && l.y() < 0)
    {
        pix = pixUpRight[i];
        shiftNumber = 5;
    }
    if(l.x() < 0 && l.y() > 0)
    {
        pix = pixDownLeft[i];
        shiftNumber = 6;
    }
    if(l.x() < 0 && l.y() < 0)
    {
        pix = pixUpLeft[i];
        shiftNumber = 7;
    }

    if (t == 0)
    {
        painter.drawPixmap(igra.adam[0].pos - QPoint(35, 50) + shift[i][shiftNumber], pix);
//        painter.drawEllipse(igra.adam[0].pos - QPoint(0, 30), 10, 10);
    }
    else
    {
        painter.drawPixmap(igra.adam[t].pos.x()-igra.adam[0].location.x()+igra.size.width()/2-35+shift[i][shiftNumber].x(),
                igra.adam[t].pos.y()-igra.adam[0].location.y()+igra.size.height()/2-50+shift[i][shiftNumber].y(), pix);
        painter.drawRect(igra.adam[t].pos.x()-igra.adam[0].location.x()+igra.size.width()/2-30 + shift[i][shiftNumber].x(),
                         igra.adam[t].pos.y()-igra.adam[0].location.y()+igra.size.height()/2-65 + shift[i][shiftNumber].y(), 60, 10);
        painter.setBrush(Qt::green);
        painter.drawRect(igra.adam[t].pos.x()-igra.adam[0].location.x()+igra.size.width()/2-30 + shift[i][shiftNumber].x(),
                         igra.adam[t].pos.y()-igra.adam[0].location.y()+igra.size.height()/2-65 + shift[i][shiftNumber].y(),
                igra.adam[t].health*1.0/ igra.adam[t].healthMax * 60, 10);
    }
    //qDebug() << igra.adam[t].health*1.0/ igra.adam[t].healthMax * 60 << t;
}

void Label::drawLoot(QPainter &painter)
{
//    qDebug() << igra.bonus.num;
    if (igra.bonus.num == 0)
    {
        QPixmap pix(":/pictures/apteka.png");
        painter.drawPixmap(igra.bonus.pos - igra.adam[0].location + QPoint(igra.size.width()/2-20, igra.size.height()/2-12), pix);
    }
    else if (igra.bonus.num == 1)
    {
        QPixmap pix(":/pictures/agility.png");
        painter.drawPixmap(igra.bonus.pos - igra.adam[0].location + QPoint(igra.size.width()/2-17, igra.size.height()/2-15), pix);
    }
    else if (igra.bonus.num == 2)
    {
        QPixmap pix(":/pictures/damage.png");
        painter.drawPixmap(igra.bonus.pos - igra.adam[0].location + QPoint(igra.size.width()/2-17, igra.size.height()/2-15), pix);
    }
    else if (igra.bonus.num == 3)
    {
        QPixmap pix(":/pictures/armor.png");
        painter.drawPixmap(igra.bonus.pos - igra.adam[0].location + QPoint(igra.size.width()/2-17, igra.size.height()/2-15), pix);
    }
}

void Label::drawPath(Game &a)
{
    QPainter painter(pixmap);
    painter.setPen(Qt::red);
    for (int i = 0; i < igra.path.size(); i++)
        igra.adam[0].pathPoints[i] -= a.adam[0].location;
    painter.drawPolyline(igra.adam[0].pathPoints, igra.path.size());
    for (int i = 0; i < igra.path.size(); i++)
        igra.adam[0].pathPoints[i] += a.adam[0].location;
    setPixmap(*pixmap);
}

void Label::drawHP(Game &a)
{
    QPainter painter(pixmap);
    QString s1, s2;
    s2.setNum(a.adam[0].healthMax);
    s1.setNum(a.adam[0].health);
    painter.setBrush(Qt::red);
    painter.drawRect(a.size.width()-205, 5, 200, 20);
    painter.setBrush(Qt::green);
    painter.drawRect(a.size.width()-205, 5, a.adam[0].health*1.0/ a.adam[0].healthMax * 200, 20);


    painter.drawText(QPointF(a.size.width() - 125, 20), s1+"/"+s2);
    setPixmap(*pixmap);
}

void Label::drawEXP(Game &a)
{
    QPainter painter(pixmap);
    QString s1, s2;
    s2.setNum(a.adam[0].expMax);
    s1.setNum(a.adam[0].exp);
    painter.setBrush(Qt::gray);
    painter.drawRect(a.size.width()-205, 30, 200, 20);
    painter.setBrush(Qt::blue);
    painter.drawRect(a.size.width()-205, 30, a.adam[0].exp*1.0/ a.adam[0].expMax * 200, 20);


    painter.drawText(QPointF(a.size.width() - 125, 45), s1+"/"+s2);
    setPixmap(*pixmap);
}

void Label::drawBonusTime(Game &a)
{
    if (a.bonus.num == 0)
        return;
    QString s;
    QPainter painter(pixmap);
    painter.setBrush(Qt::gray);
    painter.drawRect(100, 17, 250, 20);
    painter.setBrush(Qt::cyan);
    painter.drawRect(100, 17, a.adam[0].bonusTime *1.0/ 250 * 250, 20);

    if(a.bonus.num == 1)
        s = "SPEED";
    else if(a.bonus.num == 2)
        s = "DAMAGE";
    else if(a.bonus.num = 3)
        s = "ARMOR";
    painter.drawText(QPointF(a.size.width()/2-120, 32), "BONUS " + s);
    setPixmap(*pixmap);
}


void Label::mousePressEvent(QMouseEvent *ev)
{
//    igra.adam[0].stage = 0;
//      qDebug() << igra.adam[0].stage;
    int s = 0;
    QPoint testStart = igra.heroRomb(s);
    if (testStart.x() < 0 || testStart.y() < 0 || testStart.x() >= igra.ny || testStart.y() >= igra.nx)
    {
//        qDebug() << testStart.x() << " " << testStart.y();
        return;
    }

    QPoint testEnd;
    int i, j;
    double x1, x2, y1, y2;

    QPainter painter(pixmap);
    i = (ev->y()+igra.adam[0].location.y()-igra.size.height()/2)/igra.diag;
    i *= 2;
    j = (ev->x()+igra.diag+igra.adam[0].location.x()-igra.size.width()/2)/(igra.diag*2);
    x1 = (j+0.5)*igra.diag*2, y1 = (i/2+0.5)*igra.diag;
    x2 = j*igra.diag*2, y2 = (i/2+1)*igra.diag;
    if (ev->y()+igra.adam[0].location.y()-igra.size.height()/2 > (y1-y2)/(x1-x2)*(ev->x()+igra.adam[0].location.x()-igra.size.width()/2)-
            (y1-y2)/(x1-x2)*x1+y1)
        i++;
    else if (ev->y()+igra.adam[0].location.y()-igra.size.height()/2 < (y1-y2)/(x1-x2)*(ev->x()+igra.adam[0].location.x()-igra.size.width()/2)-
             (y1-y2)/(x1-x2)*(x1-igra.diag)+y1-igra.diag/2 && i > 0 && j > 0)
        i--, j--;
    else
    {
        x1 = j*igra.diag*2, y1 = (i/2+1)*igra.diag;
        x2 = (j-0.5)*igra.diag*2, y2 = (i/2+0.5)*igra.diag;
        if (ev->y()+igra.adam[0].location.y()-igra.size.height()/2 > (y1-y2)/(x1-x2)*(ev->x()+igra.adam[0].location.x()-igra.size.width()/2)
                -(y1-y2)/(x1-x2)*x1+y1 && j > 0)
            i++, j--;
        else if (ev->y()+igra.adam[0].location.y()-igra.size.height()/2 < (y1-y2)/(x1-x2)*(ev->x()+igra.adam[0].location.x()-igra.size.width()/2)
                 -(y1-y2)/(x1-x2)*(x1+igra.diag)+y1-igra.diag/2 && i > 0)
            i--;
    }
    //qDebug() << i << j;
    testEnd.setX(i); // Назначаю пункт назначения (лол)
    testEnd.setY(j);
//    qDebug() << i << " " << j << endl;
    if (i < 0 || j < 0 || i >= igra.ny || j >= igra.nx)
        return;

    double flag = 0;
    if (i%2)
        flag = 1;
    QPointF p[4] = {
        QPointF((j+flag/2)*igra.diag*2-igra.adam[0].location.x()+igra.size.width()/2,
        (i/2+flag/2)*igra.diag-igra.adam[0].location.y()+igra.size.height()/2),
        QPointF((j+0.5+flag/2)*igra.diag*2-igra.adam[0].location.x()+igra.size.width()/2,
        (i/2+0.5+flag/2)*igra.diag-igra.adam[0].location.y()+igra.size.height()/2),
        QPointF((j+flag/2)*igra.diag*2-igra.adam[0].location.x()+igra.size.width()/2,
        (i/2+1+flag/2)*igra.diag-igra.adam[0].location.y()+igra.size.height()/2),
        QPointF((j-0.5+flag/2)*igra.diag*2-igra.adam[0].location.x()+igra.size.width()/2,
        (i/2+0.5+flag/2)*igra.diag-igra.adam[0].location.y()+igra.size.height()/2),
    };

    for (int k = 1; k < igra.countBot; k++)
      //  qDebug() << k << igra.heroRomb(k) << testEnd;
        if (!igra.adam[k].isDead)
        {
            if (igra.heroRomb(k) == testEnd)
            {
                igra.adam[0].aim = k;
                break;
            }
            else
                igra.adam[0].aim = 0;
        }

    if (!igra.adam[0].isGoing)
    {
        int k = 0;
        igra.adam[0].isGoing = true;
        igra.bfs(testStart, testEnd, k);
    }
    else
        igra.adam[0].newWay = testEnd;

    painter.setPen(Qt::black);
    painter.setBrush(Qt::red);
    painter.drawPolygon(p, 4);

    setPixmap(*pixmap);
}

void Label::drawMeteor(Game &a)
{
    QPixmap pix(":/pictures/meteor.png");
    QPainter painter(pixmap);
    a.fireStone.time++;
    if (a.fireStone.time >= 500)
    {
        a.fireStone.pos = a.adam[0].location-QPointF(0, a.size.height()/2);
        a.fireStone.inAir = true;
        a.fireStone.aim = a.adam[0].location;
        a.fireStone.time = 0;
    }
    if (a.fireStone.inAir)
    {
        QVector2D direction(0, 1);
        direction.normalized();
        a.fireStone.timeInAir++;

//        painter.setBrush(Qt::yellow);
//        painter.drawEllipse(a.fireStone.pos - a.adam[0].location+QPointF(igra.size.width()/2, igra.size.height()/2), 20, 20);
        painter.drawPixmap(a.fireStone.pos - a.adam[0].location+QPointF(igra.size.width()/2-20, igra.size.height()/2-85), pix);
        a.fireStone.pos = a.fireStone.pos + direction.toPointF() * a.fireStone.velocity;
        //qDebug() << a.fireStone.timeInAir << "TimeInAir";
        setPixmap(*pixmap);
    }
    if (a.fireStone.pos.y() >= a.fireStone.aim.y())
    {
        a.meteorDamage();
        a.fireStone.pos = QPointF(-a.size.width(), -a.size.height());
        a.fireStone.inAir = false;
        a.fireStone.timeInAir = 0;
    }
}
