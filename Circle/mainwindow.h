#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"
#include "label.h"
#include <QKeyEvent>
#include <QTimerEvent>
#include <QMouseEvent>
#include <QMediaPlayer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    int id;
    QMediaPlayer *player;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Game action;
    QString armr, dmg, agility, lvl, lvl2;
    bool t;
    void lost();

public slots:
    void timerEvent(QTimerEvent *);
    void on_pushButton_clicked();
private slots:
    void on_dmgButton_clicked();
    void on_armrButton_clicked();
    void on_agltyButton_clicked();
    void on_hpButton_clicked();
    void checkLVL();
    void on_exitButton_clicked();
};

#endif // MAINWINDOW_H
