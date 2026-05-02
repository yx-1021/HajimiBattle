#ifndef ATTACKBOX_H
#define ATTACKBOX_H

#include <QRectF>
#include <QPainter>
#include"src/core/gametypes.h"
#include"src/entity/character.h"

class Attackbox
{
public:
    Attackbox();
    Attackbox(QRectF &rec,double damage,double duration,bool ifplayer);
    void update();
    double Damage();
    QRectF getrect();
    bool fromPlayer();

    bool isExpired();
    void expire();


    QRectF rect;
    double damage;
    double duration;
    bool isplayer;
    bool isexpired;
};

#endif // ATTACKBOX_H
