#ifndef LABEL_H
#define LABEL_H

#include <QLabel>
#include "game.h"
#include <QPointF>
#include <QMouseEvent>

class Label : public QLabel
{
    Q_OBJECT

    QPixmap *pixmap;
    Game igra;

    QPixmap pix;
    QPixmap pixDown[2];
    QPixmap pixUp[2];
    QPixmap pixRight[2];
    QPixmap pixLeft[2];
    QPixmap pixUpLeft[2];
    QPixmap pixDownLeft[2];
    QPixmap pixUpRight[2];
    QPixmap pixDownRight[2];

    QPoint shift[2][8];

    int shiftNumber;
public:
    explicit Label(QWidget *parent = 0);
    void drawLand(Game&);
    void drawLoot(QPainter&);
    void drawHero(QPainter&, const int &, int);
    void drawItems(Game&, QPainter&);
    void drawPath(Game&);
    void drawHP(Game&);
    void drawEXP(Game&);
    void drawBonusTime(Game&);
    void drawMeteor(Game&);
    void setSize(const Game &);
    void mousePressEvent(QMouseEvent *ev);
    void drawGrenade(Tower&, QPainter&, Game&);
};

#endif // LABEL_H
