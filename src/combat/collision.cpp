#include "collision.h"


//碰撞检测
Collision::Collision()
{
}

void Collision::handleColl(QVector<Attackbox> &attackboxes,
                                             Player &player,
                                             QVector<Enemy> &enemies)
{
    for (int i = 0; i < attackboxes.size(); i++)
    {
        Attackbox &box = attackboxes[i];

        if (box.isExpired()) continue;

        if (box.fromPlayer())
        {
            if (box.alreadyHit()) continue;
            for (int j = 0; j < enemies.size(); j++)
            {
                Enemy &enemy = enemies[j];

                if (!enemy.isalive()) continue;

                if (box.getrect().intersects(enemy.rect()))
                {
                    enemy.hurt(box.Damage());
                    enemy.addStiff(stiffTime(box.Type()));
                    box.markHit();
                    break;
                }
            }
        }
        else
        {
            if (box.alreadyHit()) continue;
            if (player.isalive() && box.getrect().intersects(player.rect()))
            {
                player.hurt(box.Damage());
                player.addStiff(stiffTime(box.Type()));
                box.markHit();
            }
        }
    }
}


//僵直处理
int Collision::stiffTime(Battle type)
{
    if (type == Battle::attack)
    {
        return 2;
    }

    if (type == Battle::ha)
    {
        return 4;
    }

    if (type == Battle::plusattack)
    {
        return 2;
    }

    return 3;
}