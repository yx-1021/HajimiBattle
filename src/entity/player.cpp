#include "player.h"
#include "src/combat/attackbox.h"

Player::Player()
    :Character(300,300,100,100)
{
    load();
    maxhp = 100;
    hp = 100;
    attack = 10;
    speed = 3;
    direction = Direction::right;
    action=Action::still;
}

void Player::load()
{
    epixmap.load(":/new/prefix1/rescource/player_right.png");
}

void Player::paint(QPainter &painter)
{
    if(!ealive) return;
    painter.drawPixmap(rect().toRect(),epixmap);

}

void Player::handleKeyP(QKeyEvent *event)
{
    if(event->key()==Qt::Key_W)
    {
        keyW=true;
        direction = Direction::up;
    }
    if(event->key()==Qt::Key_S)
    {
        keyS=true;
        direction = Direction::down;
    }
    if(event->key()==Qt::Key_A)
    {
        keyA=true;
        direction = Direction::left;
    }
    if(event->key()==Qt::Key_D)
    {
        keyD=true;
        direction = Direction::right;
    }
}

void Player::handleKeyR(QKeyEvent *event)
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


void Player::updategame(double mapw,double maph)
{
    if (!ealive) return;

    if (keyW) epos.setY(epos.y() - speed);
    if (keyS) epos.setY(epos.y() + speed);
    if (keyA) epos.setX(epos.x() - speed);
    if (keyD) epos.setX(epos.x() + speed);

    epos.setX(qBound(0.0, epos.x(), qMax(0.0, double(mapw) - esize.width())));
    epos.setY(qBound(0.0, epos.y(), qMax(0.0, double(maph) - esize.height())));

}

Attackbox Player::createAttbox(Battle type)
{
    return Skill::createAttbox(*this, type, true);
}
