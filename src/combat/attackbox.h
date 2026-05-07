#ifndef ATTACKBOX_H
#define ATTACKBOX_H

#include <QRectF>
#include <QPainter>
#include"src/core/gametypes.h"
#include"src/entity/character.h"

//攻击判定
class Attackbox
{
public:
    Attackbox();
    Attackbox(QRectF &rec, double damage, double duration, bool ifplayer, Battle battleType);
    void update();
    double Damage();
    QRectF getrect();
    bool fromPlayer();
    bool alreadyHit() const;
    void markHit();

    bool isExpired();
    Battle Type();


    QRectF rect;
    double damage;
    double duration;
    bool isplayer;
    bool isexpired;
    bool hashit = false;
    Battle battleType = Battle::attack;

};

#endif // ATTACKBOX_H
