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
    updatecd();
    pendattack = false;

    QPointF enemyCenter = rect().center();

    double dx = playerCenter.x() - enemyCenter.x();
    double dy = playerCenter.y() - enemyCenter.y();

    double len = qSqrt(dx * dx + dy * dy);

    if (qAbs(dx) > qAbs(dy))
    {
        direction = dx > 0 ? Direction::right : Direction::left;
    }
    else
    {
        direction = dy > 0 ? Direction::down : Direction::up;
    }

    if (attinterval <= 0 && len <= attackrange && attackcd <= 0)
    {
        skillFired = false;
        chooseState(len);
    }
    else if (attinterval <= 0 && len <= harange && hacd <= 0)
    {
        skillFired = false;
        chooseState(len);
    }
    else
    {
        aiTimer--;

        if (aiTimer <= 0 && attinterval <= 0)
        {
            skillFired = false;
            chooseState(len);
        }
    }
    if (aiState == Action::still)
    {
        action = Action::still;
    }
    else if (aiState == Action::run)
    {
        action = Action::run;

        if (len> 1e-6)
        {
            double vx = dx / len;
            double vy = dy / len;

            epos.setX(epos.x() + vx * speed);
            epos.setY(epos.y() + vy * speed);
        }
    }
    else if (aiState ==Action::attack)
    {
        action = Action::attack;

        if (!skillFired && attackcd<= 0)
        {
            pendattack = true;
            pendbattle = Battle::attack;
            skillFired = true;
            attackcd= 90;
            attinterval=50;        }
    }
    else if (aiState ==Action::ha)
    {
        action = Action::ha;

        if (!skillFired && hacd<= 0)
        {
            pendattack = true;
            pendbattle = Battle::ha;
            skillFired = true;

            hacd= 120;
            attinterval=35;
            halayer++;

            if (halayer >= 3)
            {
                halayer = 3;
                plusready = true;
            }
        }
    }
    else if (aiState ==Action::plusattack)
    {
        action = Action::plusattack;

        if (!skillFired && pluscd<= 0 && plusready && halayer >= 3)
        {
            pendattack = true;
            pendbattle = Battle::plusattack;
            skillFired = true;

            pluscd= 160;
            attinterval=60;
            halayer = 0;
            plusready = false;
        }
    }

    epos.setX(qBound(0.0, epos.x(), qMax(0.0, mapw - esize.width())));
    epos.setY(qBound(0.0, epos.y(), qMax(0.0, maph - esize.height())));

    atttimer++;
}

void Enemy::updatecd()
{

    if (attackcd> 0) attackcd--;
    if (hacd > 0) hacd--;
    if (pluscd > 0) pluscd--;
     if (attinterval > 0) attinterval--;
}

void Enemy::chooseState(double len)
{
    skillFired = false;

    int r = QRandomGenerator::global()->bounded(100);

    if (len >sightrange)
    {
        aiState = Action::still;
        aiTimer = QRandomGenerator::global()->bounded(40, 100);
        action = Action::still;
        return;
    }

    if (plusready && halayer >= 3 && pluscd<= 0 && len <= attackrange+ 40)
    {
        aiState = Action::plusattack;
        aiTimer = 35;
        action = Action::plusattack;
        return;
    }

    if (len <= attackrange&&attackcd<= 0 && r < 60)
    {
        aiState = Action::attack;
        aiTimer = 25;
        action = Action::attack;
        return;
    }

    if (len <= harange&&hacd<= 0 && r <40)
    {
        aiState = Action::ha;
        aiTimer = 35;
        action = Action::ha;
        return;
    }

    if (r < 25)
    {
        aiState = Action::still;
        aiTimer = QRandomGenerator::global()->bounded(30, 80);
        action = Action::still;
    }
    else
    {
        aiState = Action::run;
        aiTimer = QRandomGenerator::global()->bounded(50, 120);
        action = Action::run;
    }
}

bool Enemy::haspendatt()
{
   return pendattack;
}

Attackbox Enemy::takePendingAttackBox()
{
    pendattack = false;
    return Skill::createAttbox(*this, pendbattle, false);
}