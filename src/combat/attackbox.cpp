#include "attackbox.h"

//攻击判定区域
Attackbox::Attackbox() {}
Attackbox::Attackbox(QRectF &rec, double damage, double duration, bool ifplayer, Battle battleType)
    : rect(rec),
    damage(damage),
    duration(duration),
    isplayer(ifplayer),
    isexpired(false),
    battleType(battleType)
{
}

Battle Attackbox::Type()
{
    return battleType;
}

QRectF Attackbox::getrect()
{
     return rect;
}
 void Attackbox::update()
{
     if (isexpired) return;

     duration--;

     if (duration <= 0)
     {
         isexpired = true;
     }

}
bool Attackbox::fromPlayer()
{
    return isplayer;
}


bool Attackbox::alreadyHit() const
{
 return hashit;
}

void Attackbox::markHit()
{
      hashit = true;
}
 bool Attackbox::isExpired()
{
     return isexpired;
}
 double Attackbox:: Damage()
{
     return damage;
}