#include "attackbox.h"

Attackbox::Attackbox() {}
Attackbox::Attackbox(QRectF &rec,double damage,double duration,bool ifplayer)
    :rect(rec),damage(damage),duration(duration),isplayer(ifplayer),isexpired(false)
{

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
void Attackbox::expire()
{
    isexpired=true;
}
 double Attackbox:: Damage()
{
     return damage;
}