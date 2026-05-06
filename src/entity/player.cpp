#include "player.h"
#include "src/combat/attackbox.h"

Player::Player()
    : Character(300, 300, 100, 100)
{
    maxhp = 100;
    hp = 100;
    attack = 10;
    speed = 3;

    direction = Direction::right;
    action = Action::still;
    ptype = Battle::attack;

    keyW = false;
    keyS = false;
    keyA = false;
    keyD = false;

    load();
}


void Player::paint(QPainter &painter)
{
    if (!ealive) return;

    QPixmap pix = currentPixmap();

    if (!pix.isNull())
    {
        painter.drawPixmap(rect().toRect(), pix);
    }

}

void Player::handleKeyP(QKeyEvent *event)
{
    if (isStiff()) return;
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

    updateStiff();

    if (isStiff())
    {
        keyW = false;
        keyS = false;
        keyA = false;
        keyD = false;

        if (!isActionLocked())
        {
            action = Action::still;
        }

        updateaction();
        return;
    }
    double dx = 0;
    double dy = 0;

    if (keyW) dy -= 1;
    if (keyS) dy += 1;
    if (keyA) dx -= 1;
    if (keyD) dx += 1;

    bool moving = (dx != 0 || dy != 0);

    if (moving)
    {
        double len = qSqrt(dx * dx + dy * dy);
        dx /= len;
        dy /= len;

        epos.setX(epos.x() + dx * speed);
        epos.setY(epos.y() + dy * speed);

        if (qAbs(dx) > qAbs(dy))
        {
            direction = dx > 0 ? Direction::right : Direction::left;
        }
        else
        {
            direction = dy > 0 ? Direction::down : Direction::up;
        }

        if (!isActionLocked())
        {
            action = Action::run;
        }
    }
    else
    {
        if (!isActionLocked())
        {
            action = Action::still;
        }
    }

    epos.setX(qBound(0.0, epos.x(), qMax(0.0, mapw - esize.width())));
    epos.setY(qBound(0.0, epos.y(), qMax(0.0, maph - esize.height())));

    updateaction();

}

void Player::load()
{
    loadaction("player");
}

Attackbox Player::createAttbox(Battle type)
{
    return Skill::createAttbox(*this, type, true);
}

void Player::updatecd()
{

    if(attackcd>0) attackcd--;
    if(hacd>0) hacd--;
    if(pluscd>0) pluscd--;
}

bool Player::useskill(Battle type)
{
    if (!ealive) return false;
    if (isStiff()) return false;

    if (isActionLocked()) return false;

    if (type == Battle::attack)
    {
        if (attackcd > 0) return false;

        attackcd = 35;
        playaction(Action::attack, 24);
        return true;
    }

    if (type == Battle::ha)
    {
        if (hacd > 0) return false;

        hacd = 60;
        halayer++;

        if (halayer >= 3)
        {
            halayer = 3;
            plusready = true;
        }

        playaction(Action::ha, 30);
        return true;
    }

    if (type == Battle::plusattack)
    {
        if (pluscd > 0) return false;
        if (!plusready) return false;
        if (halayer < 3) return false;

        pluscd = 100;
        halayer = 0;
        plusready = false;

        playaction(Action::plusattack, 36);
        return true;
    }

    return false;
}
