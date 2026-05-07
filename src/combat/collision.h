#ifndef COLLISION_H
#define COLLISION_H

#include <QVector>
#include "attackbox.h"
#include "src/entity/player.h"
#include "src/entity/enemy.h"

//判断命中和伤害僵直
class Collision
{
public:
    Collision();

    static void handleColl(QVector<Attackbox> &attackboxes,
                                Player &player,
                                QVector<Enemy> &enemies);
    static int stiffTime(Battle type);
};

#endif // COLLISION_H
