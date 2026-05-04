#include "character.h"
#include "character.h"

#include <QDebug>
#include <QStringList>
namespace
{
bool tryLoadPixmap(QPixmap &pix, const QString &fileName)
{
    QStringList paths;

    paths << QString(":/new/prefix1/resource/%1").arg(fileName);
    paths << QString(":/resource/%1").arg(fileName);

    paths << QString(":/new/prefix1/rescource/%1").arg(fileName);
    paths << QString(":/rescource/%1").arg(fileName);

    for (const QString &path : paths)
    {
        if (pix.load(path))
        {
            qDebug() << "sprite loaded:" << path;
            return true;
        }
    }

    qDebug() << "sprite load failed:" << fileName;
    return false;
}
}


Character::Character()
    :Entity(),
    hp(10),
    maxhp(10),
    speed(3),
    attack(1),
    direction(Direction::right),
    action(Action::still)
{
}

Character::Character(double x, double y, double w, double h)
    : Entity(x, y, w, h),
    hp(10),
    maxhp(10),
    speed(3),
    attack(1),
    direction(Direction::right),
    action(Action::still)
{
}

void Character::load()
{
}
void Character::hurt(double att)
{
    if(!ealive) return;
    hp-=att;
    if(hp<=0)
    {
        hp=0;
        Entity::ealive=false;
    }
}
void Character::setRoleName(const QString &name)
{
    roleName = name;
}


QPixmap Character::currentPixmap()
{ int a = static_cast<int>(action);
    int d = static_cast<int>(direction);

    int still = static_cast<int>(Action::still);
    int right = static_cast<int>(Direction::right);

    if (a >= 0 && a < ACTION && d >= 0 && d < DIRECT)
    {
        if (!animations[a][d].isEmpty())
        {
            int id = frameid % animations[a][d].size();
            return animations[a][d][id];
        }

        if (!animations[still][d].isEmpty())
        {
            int id = frameid % animations[still][d].size();
            return animations[still][d][id];
        }

        if (!animations[still][right].isEmpty())
        {
            int id = frameid % animations[still][right].size();
            return animations[still][right][id];
        }
    }

    return epixmap;
}

void Character::startAction(Action a, int keepTicks)
{
    action = a;
    actionLock = keepTicks;
}

bool Character::actionLocked()
{
 return actionLock > 0;
}



QString Character::Actionname(Action a)
{
    switch (a)
    {
    case Action::still:
        return "still";
    case Action::run:
        return "run";
    case Action::attack:
        return "attack";
    case Action::ha:
        return "ha";
    case Action::plusattack:
        return "plusattack";
    }

    return "still";

}

QString Character::Direcname(Direction d)
{
    switch (d)
    {
    case Direction::up:
        return "up";
    case Direction::down:
        return "down";
    case Direction::left:
        return "left";
    case Direction::right:
        return "right";
    }

    return "right";

}

void Character::loadaction(const QString &roleName)
{
    const int MAX_FRAME = 20;

    for (int a = 0; a < ACTION; a++)
    {
        for (int d = 0; d < DIRECT; d++)
        {
            animations[a][d].clear();

            Action act = static_cast<Action>(a);
            Direction dir = static_cast<Direction>(d);

            for (int f = 0; f < MAX_FRAME; f++)
            {
                QString fileName = QString("%1_%2_%3_%4.png")
                .arg(roleName)
                    .arg(Direcname(dir))
                    .arg(Actionname(act))
                    .arg(f);

                QPixmap pix;
                if (tryLoadPixmap(pix, fileName))
                {
                    animations[a][d].append(pix);
                }
            }


            if (animations[a][d].isEmpty())
            {
                QString fileName = QString("%1_%2_%3.png")
                .arg(roleName)
                    .arg(Direcname(dir))
                    .arg(Actionname(act));

                QPixmap pix;
                if (tryLoadPixmap(pix, fileName))
                {
                    animations[a][d].append(pix);
                }
            }

            if (!animations[a][d].isEmpty())
            {
                qDebug() << "loaded animation:"
                         << roleName
                         << Direcname(dir)
                         << Actionname(act)
                         << "frames =" << animations[a][d].size();
            }
        }
    }
}

void Character::updategame(double mapw,double maph)
{
    Q_UNUSED(mapw);
    Q_UNUSED(maph);
    return;
}

Direction Character::getDirection()
{

    return direction;
}

Action Character::getAction()
{
    return action;
}

void Character::setDirection(Direction d)
{
    direction = d;
}

void Character::setAction(Action a)
{
    action=a;
}

void Character::playaction(Action a, int lockTicks)
{
    action = a;
    actionLock = lockTicks;

    frameid = 0;
    frametick = 0;

    lastAction = a;
    lastDirection = direction;
}

bool Character::isActionLocked()
{
    return actionLock > 0;
}

void Character::updateaction()
{
    if (action != lastAction || direction != lastDirection)
    {
        frameid = 0;
        frametick = 0;
        lastAction = action;
        lastDirection = direction;
    }
    int a = static_cast<int>(action);
    int d = static_cast<int>(direction);

    if (a >= 0 && a < ACTION && d >= 0 && d < DIRECT)
    {
        if (!animations[a][d].isEmpty())
        {
            frametick++;

            if (frametick >= frameinterval)
            {
                frametick = 0;
                frameid++;

                if (frameid >= animations[a][d].size())
                {
                    frameid = 0;
                }
            }
        }
    }

    if (actionLock > 0)
    {
        actionLock--;
    }
}

void Character::updateactionlock()
{
    if (actionLock > 0)
    {
        actionLock--;
    }
}