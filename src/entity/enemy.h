#ifndef ENEMY_H
#define ENEMY_H

#include <QPointF>
#include <QtMath>
#include "src/entity/character.h"
#include "src/core/gametypes.h"
#include "src/combat/attackbox.h"
#include"src/combat/skill.h"

class Enemy:public Character
{
public:
    Enemy();
    Enemy(Enemytype type,double x,double y);

    Attackbox createAttbkBox();
    bool canatt();
    void initbat();
    void load();
    void paint(QPainter &painter);
    Attackbox createAttBox();
    void updategame(double mapw, double maph,QPointF playerCenter);

    Enemytype type;
    Battle battle=Battle::attack;
    double attinterval;
    double atttimer=0;


};

#endif // ENEMY_H
