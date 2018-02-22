#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "label.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setSize(action);
    ui->label->drawLand(action);
    ui->label->drawHP(action);
    ui->label->drawEXP(action);
    int s = 1;
//    qDebug() << action.heroRomb(s);
    s++;
    armr.setNum(action.getArmr());
    dmg.setNum(action.getDmg());
    agility.setNum(action.getAgl());
    lvl.setNum(action.getLVL());
    ui->labelHealth->setText("Damage: "+dmg);
    ui->labelArmor->setText("Armor: "+armr);
    ui->labelAgility->setText("Agility: "+agility);
    ui->labelLVL->setAlignment(Qt::AlignCenter);
    ui->labelLVL->setText("LEVEL "+lvl);

    ui->agltyButton->setEnabled(0);
    ui->armrButton->setEnabled(0);
    ui->hpButton->setEnabled(0);
    ui->dmgButton->setEnabled(0);

//    player = new QMediaPlayer;
//    player->setMedia(QUrl::fromLocalFile(":/audio/AC⁄DC - Highway to Hell.mp3"));
//    player->play();
}

void MainWindow::timerEvent(QTimerEvent *)
{
    ui->label->drawLand(action);
    ui->label->drawMeteor(action);

    action.checkBonus();
    action.smartBots();

//    action.checkPulse();
    ui->label->drawHP(action);
    ui->label->drawEXP(action);

    armr.setNum(action.getArmr());
    dmg.setNum(action.getDmg());
    agility.setNum(action.getAgl());
    lvl.setNum(action.getLVL());
    lvl2.setNum(action.getLVL2());

    ui->labelLVL->setAlignment(Qt::AlignCenter);
    ui->labelLVL->setText("LEVEL "+lvl+" (+"+lvl2+")");
    ui->labelHealth->setText("Damage: "+dmg);
    ui->labelArmor->setText("Armor: "+armr);
    ui->labelAgility->setText("Agility: "+agility);

    checkLVL();
    action.checkPulse();//Тут лучше
    action.upgradeBots();
    if (action.getBonusTime() > 0)
        ui->label->drawBonusTime(action);

    ui->agltyButton->setEnabled(t);
    if(action.getAgl() > 45)
        ui->agltyButton->setDisabled(1);
    ui->armrButton->setEnabled(t);
    ui->hpButton->setEnabled(t);
    ui->dmgButton->setEnabled(t);
    if (action.getHP(0) <= 0)
        lost();
//    ui->label->drawItems(action);
//    ui->label->drawHero(action);
//    ui->label->drawPath(action);
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->pushButton->text() == "START")
    {
        id = startTimer(50);
        ui->pushButton->setText("PAUSE");
    }
    else
    {
        killTimer(id);
        ui->pushButton->setText("START");
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_dmgButton_clicked()
{
    action.upDamage(0);
    action.changeLVL2();
}

void MainWindow::on_armrButton_clicked()
{
    action.upArmor(0);
    action.changeLVL2();
}

void MainWindow::on_agltyButton_clicked()
{
    action.upAttackSpeed(0);
    action.changeLVL2();
}

void MainWindow::on_hpButton_clicked()
{
    action.upHP(0);
    action.changeLVL2();
}

void MainWindow::checkLVL()
{
    if(action.getLVL2() > 0)
        t = true;
    else
        t = false;
}

void MainWindow::lost()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("You died");
//    msgBox.setIconPixmap(QPixmap(":/pictures/vader.jpg"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setButtonText(QMessageBox::Ok, "Restart");
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Cancel:
        on_pushButton_clicked();
        break;
    case QMessageBox::Ok:
        action.restart();
        break;
    default:
        break;
    }
}

void MainWindow::on_exitButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "The Circle", "Quit?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
      QApplication::quit();
}
