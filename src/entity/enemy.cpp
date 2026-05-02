#include "src/entity/enemy.h"

Enemy::Enemy()
:Character(600, 600, 100, 100)
{
    initbat();
    load();
}
Enemy::Enemy(Enemytype t,double x, double y)
    :Character(x,y,100,100),
    type(t)
{
    initbat();
    load();
}

void Enemy::initbat()
{
    if(type==Enemytype::ocat)
    {
        hp=10;
        maxhp=10;
        speed=1.0;
        attack=3;
        attinterval=120;

    }
    else
    {
        hp=7;
        maxhp=7;
        speed=0.5;
        attack=2;
        attinterval=120;
    }
}

void Enemy::load()
{
    if(type==Enemytype::box)
    {
        epixmap.load(":/new/prefix1/rescource/box.png");
    }
    else if(type==Enemytype::ocat)
    {
        epixmap.load(":/new/prefix1/rescource/enemy_cat.png");
    }
}

void Enemy::paint(QPainter &painter)
{
    if (!ealive) {
        return;
    }
     painter.drawPixmap(rect().toRect(), epixmap);
}

bool Enemy::canatt()
{
    return ealive&&atttimer>=attinterval;
}

Attackbox Enemy::createAttBox()
{
    atttimer=0;
    return Skill::createAttbox(*this, battle, false);
}
void Enemy::updategame(double mapw, double maph,QPointF playerCenter)
{
    if (!ealive) {
        return;
    }

    QPointF enemyCenter = rect().center();

    double dx = playerCenter.x() - enemyCenter.x();
    double dy = playerCenter.y() - enemyCenter.y();

    double len = qSqrt(dx * dx + dy * dy);

    if (len > 1e-6)
    {
        double vx = dx / len;
        double vy = dy / len;

        epos.setX(epos.x() + vx *speed);
        epos.setY(epos.y() + vy *speed);

        if (qAbs(dx) > qAbs(dy)) {
            if (dx > 0) {
                direction = Direction::right;
            } else {
                direction = Direction::left;
            }
        } else {
            if (dy > 0) {
                direction = Direction::down;
            } else {
                direction = Direction::up;
            }
        }
    }

    epos.setX(qBound(0.0, epos.x(), qMax(0.0, mapw - esize.width())));
    epos.setY(qBound(0.0, epos.y(), qMax(0.0, maph - esize.height())));

    atttimer++;
}