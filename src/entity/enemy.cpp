#include "src/entity/enemy.h"

Enemy::Enemy()
:Character(600, 600, 100, 100),
 type(Enemytype::ocat)
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

//设置敌人属性
void Enemy::initbat()
{
    if(type==Enemytype::ocat)
    {
        hp=20;
        maxhp=20;
        speed=1.0;
        attack=10;
        attinterval=120;

    }
    else
    {
        hp=12;
        maxhp=12;
        speed=0.5;
        attack=5;
        attinterval=120;
    }
}

void Enemy::load()
{
    if (type == Enemytype::box)
    {
        loadaction("box");
    }
    else if (type == Enemytype::ocat)
    {
        loadaction("ocat");
    }

}



void Enemy::paint(QPainter &painter)
{
    if (!ealive) return;

    QPixmap pix = currentPixmap();

    if (!pix.isNull())
    {
        painter.drawPixmap(rect().toRect(), pix);
    }
}

void Enemy::updategame(double mapw, double maph,QPointF playerCenter)
{


    if (!ealive)
    {
        return;
    }

    updatecd();
    pendattack = false;

    if (isActionLocked())
    {
        updateStiff();
        updateaction();
        atttimer++;
        return;
    }

    if (isStiff())
    {
        action = Action::still;
        pendattack = false;
        skillFired = false;

        updateStiff();
        updateaction();

        atttimer++;
        return;
    }
    pendattack = false;

    QPointF enemyCenter = rect().center();

    double dx = playerCenter.x() - enemyCenter.x();
    double dy = playerCenter.y() - enemyCenter.y();

    double len = qSqrt(dx * dx + dy * dy);

    bool inSkillRange = (len <= attackrange);

    if (type == Enemytype::ocat)
    {
        inSkillRange = (len <= qMax(attackrange, harange));
    }

    if (inSkillRange)
    {
        prepareTimer++;
    }
    else
    {
        prepareTimer = 0;
    }

    bool prepared = prepareTimer >= prepareNeed;

    if (qAbs(dx) > qAbs(dy))
    {
        direction = dx > 0 ? Direction::right : Direction::left;
    }
    else
    {
        direction = dy > 0 ? Direction::down : Direction::up;
    }

    if (!prepared && inSkillRange)
    {
        aiState = Action::run;
    }
    else if (attinterval <= 0 && prepared && len <= attackrange && attackcd <= 0)
    {
        skillFired = false;
        chooseState(len);
    }
    else if (attinterval <= 0 && prepared && type == Enemytype::ocat && len <= harange && hacd <= 0)
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
    else if (aiState == Action::attack)
    {
        if (!skillFired && attackcd <= 0)
        {
            pendattack = true;
            pendbattle = Battle::attack;
            skillFired = true;

            attackcd = 90;
            attinterval = 50;
            prepareTimer = 0;

            playaction(Action::attack, 30);
        }
    }
    else if (aiState == Action::ha)
    {
        if (!skillFired && hacd <= 0)
        {
            pendattack = true;
            pendbattle = Battle::ha;
            skillFired = true;

            hacd = 120;
            attinterval = 35;
            prepareTimer = 0;

            halayer++;

            if (halayer >= 3)
            {
                halayer = 3;
                plusready = true;
            }

            playaction(Action::ha, 35);
        }
    }
    else if (aiState == Action::plusattack)
    {
        if (!skillFired && pluscd <= 0 && plusready && halayer >= 3)
        {
            pendattack = true;
            pendbattle = Battle::plusattack;
            skillFired = true;

            pluscd = 160;
            attinterval = 60;
            prepareTimer = 0;

            halayer = 0;
            plusready = false;

            playaction(Action::plusattack, 35);
        }
    }

    epos.setX(qBound(0.0, epos.x(), qMax(0.0, mapw - esize.width())));
    epos.setY(qBound(0.0, epos.y(), qMax(0.0, maph - esize.height())));
    updateaction();

    atttimer++;
}

void Enemy::updatecd()
{

    if (attackcd> 0) attackcd--;
    if (hacd > 0) hacd--;
    if (pluscd > 0) pluscd--;
     if (attinterval > 0) attinterval--;
}

//根据敌人类型，冷却状态，随机数选择ai状态
void Enemy::chooseState(double len)
{
    skillFired = false;

    int r = QRandomGenerator::global()->bounded(100);

    if (len > sightrange)
    {
        aiState = Action::still;
        aiTimer = QRandomGenerator::global()->bounded(40, 100);
        return;
    }

    //纸箱怪
    if (type == Enemytype::box)
    {
        if (len <= attackrange && attackcd <= 0)
        {
            aiState = Action::attack;
            aiTimer = 25;
            return;
        }

        if (r < 25)
        {
            aiState = Action::still;
            aiTimer = QRandomGenerator::global()->bounded(30, 80);
        }
        else
        {
            aiState = Action::run;
            aiTimer = QRandomGenerator::global()->bounded(50, 120);
        }

        return;
    }


    //橘猫怪
    if (plusready && halayer >= 3 && pluscd <= 0 && len <= attackrange + 40)
    {
        aiState = Action::plusattack;
        aiTimer = 35;
        return;
    }

    if (len <= attackrange && attackcd <= 0 && r < 60)
    {
        aiState = Action::attack;
        aiTimer = 25;
        return;
    }

    if (len <= harange && hacd <= 0 && r < 40)
    {
        aiState = Action::ha;
        aiTimer = 35;
        return;
    }

    if (r < 25)
    {
        aiState = Action::still;
        aiTimer = QRandomGenerator::global()->bounded(30, 80);
    }
    else
    {
        aiState = Action::run;
        aiTimer = QRandomGenerator::global()->bounded(50, 120);
    }
}
//返回是否有带生成的攻击框
bool Enemy::haspendatt()
{
   return pendattack;
}

//取出待释放的攻击框
Attackbox Enemy::takePendingAttackBox()
{
    pendattack = false;
    return Skill::createAttbox(*this, pendbattle, false);
}