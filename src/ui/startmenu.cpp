#include "startmenu.h"

StartMenu::StartMenu(QWidget *parent)
    : QWidget{parent}
{
    this->setFixedSize(1000, 600);
    //创建四种模式
    QPushButton *levelButton = new QPushButton("关卡模式", this);
    QPushButton *endlessButton = new QPushButton("无尽模式", this);
    QPushButton * setbutton=new QPushButton("设置",this);
    QPushButton * closebutton=new QPushButton("退出游戏",this);
    levelButton->setGeometry(100,400,300,50);
    endlessButton->setGeometry(100,500,300,50);
    setbutton->setGeometry(600,400,300,50);
    closebutton->setGeometry(600,500,300,50);
    //添加背景
    bg.load(":/new/prefix1/rescource/cover.png");

    connect(levelButton, &QPushButton::clicked, this, &StartMenu::levelclicked);
    connect(endlessButton, &QPushButton::clicked, this, &StartMenu::endlessclicked);
      connect(setbutton,&QPushButton::clicked,this,&StartMenu::setgameclicked);
      connect(closebutton,&QPushButton::clicked,this,&StartMenu::closeclicked);

}

void StartMenu::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.drawPixmap(rect(),bg);
}
