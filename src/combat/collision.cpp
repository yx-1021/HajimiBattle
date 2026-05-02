#include "collision.h"

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
            for (int j = 0; j < enemies.size(); j++)
            {
                Enemy &enemy = enemies[j];

                if (!enemy.isalive()) continue;

                if (box.getrect().intersects(enemy.rect()))
                {
                    enemy.hurt(box.Damage());
                    box.expire();
                    break;
                }
            }
        }
        else
        {
            if (player.isalive() && box.getrect().intersects(player.rect()))
            {
                player.hurt(box.Damage());
                box.expire();
            }
        }
    }
}
