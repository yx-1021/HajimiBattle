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
        range = 60;
        thickness = 60;
    }
    else if (type == Battle::ha)
    {
        attdamage = 6;
        attdur = 16;
        range = 110;
        thickness = 90;
    }
    else if (type == Battle::plusattack)
    {
        attdamage = 25;
        attdur = 10;
        range = 80;
        thickness = 70;
    }

    QRectF attackRect;

    switch (owner.getDirection())
    {
    case Direction::up:
        attackRect = QRectF(body.center().x() - thickness / 2,
                            body.top() - range,
                            thickness,
                            range);
        break;

    case Direction::down:
        attackRect = QRectF(body.center().x() - thickness / 2,
                            body.bottom(),
                            thickness,
                            range);
        break;

    case Direction::left:
        attackRect = QRectF(body.left() - range,
                            body.center().y() - thickness / 2,
                            range,
                            thickness);

        break;

    case Direction::right:
        attackRect = QRectF(body.right(),
                            body.center().y() - thickness / 2,
                            range,
                            thickness);
        break;
    }

    return Attackbox(attackRect, attdamage,attdur,fromPlayer);
}