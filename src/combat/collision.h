#ifndef COLLISION_H
#define COLLISION_H

#include <QVector>
#include "attackbox.h"
#include "src/entity/player.h"
#include "src/entity/enemy.h"
#include"skill.h"


class Collision
{
public:
    Collision();

    static void handleColl(QVector<Attackbox> &attackboxes,
                                Player &player,
                                QVector<Enemy> &enemies);
    static int stiffTimeByBattle(Battle type);
};

#endif // COLLISION_H
