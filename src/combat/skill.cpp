#include "skill.h"

Attackbox Skill::createAttbox( Character &owner,
                                 Battle  type,
                                 bool fromPlayer)
{
    QRectF body = owner.rect();

    double attdamage=0;
    double attdur=0;
    double range=0;
    double thickness=0;

    if (type == Battle::attack)
    {
        attdamage = 10;
        attdur = 8;
        range =70;
        thickness =70;
    }
    else if (type == Battle::ha)
    {
        attdamage = 6;
        attdur = 16;
        range =70;
        thickness =70;
    }
    else if (type == Battle::plusattack)
    {
        attdamage = 25;
        attdur = 10;
        range =70;
        thickness = 70;
    }

    QPointF c = body.center();

    QRectF attackRect;

    switch (owner.getDirection())
    {
    case Direction::up:
        attackRect = QRectF(
            c.x() - thickness / 2,
            c.y() - range,
            thickness,
            range
            );
        break;

    case Direction::down:
        attackRect = QRectF(
            c.x() - thickness / 2,
            c.y(),
            thickness,
            range
            );
        break;

    case Direction::left:
        attackRect = QRectF(
            c.x() - range,
            c.y() - thickness / 2,
            range,
            thickness
            );
        break;

    case Direction::right:
        attackRect = QRectF(
            c.x(),
            c.y() - thickness / 2,
            range,
            thickness
            );
        break;
    }

    return Attackbox(attackRect, attdamage, attdur, fromPlayer, type);
}