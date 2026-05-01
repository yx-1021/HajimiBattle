#include "gamewidget.h"

GameWidget::GameWidget(QWidget *parent)
    : QWidget{parent},
    playerx(300),
    playery(300),
    playerwidth(100),
    playerheight(100)
{
    backmap.load(":/new/prefix1/rescource/backmap.png");
    playermap.load(":/new/prefix1/rescource/player_right.png");
    setFocusPolicy(Qt::StrongFocus);

    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&GameWidget::updategame);
    timer->start(10);
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.drawPixmap(rect(),backmap);
    painter.drawPixmap(playerx, playery, playerwidth, playerheight, playermap);

}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_W)
    {
        keyW=true;
    }
    if(event->key()==Qt::Key_S)
    {
        keyS=true;
    }
    if(event->key()==Qt::Key_A)
    {
        keyA=true;
    }
    if(event->key()==Qt::Key_D)
    {
       keyD=true;
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;
    if(event->key()==Qt::Key_W)
    {
        keyW=false;
    }
    if(event->key()==Qt::Key_S)
    {
        keyS=false;
    }
    if(event->key()==Qt::Key_A)
    {
        keyA=false;
    }
    if(event->key()==Qt::Key_D)
    {
        keyD=false;
    }

}


 void GameWidget::updategame()
{
     if (keyW) playery -=speed;
     if (keyS)  playery+=speed;
     if (keyA)  playerx-=speed;
     if (keyD)  playerx+=speed;

     playerx = qBound(0, playerx, 1000-playerwidth);
     playery = qBound(0, playery, 600-playerheight);
     update();
}