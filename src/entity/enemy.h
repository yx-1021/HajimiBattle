#ifndef ENEMY_H
#define ENEMY_H

#include <QPointF>
#include <QtMath>
#include<QRandomGenerator>
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
    void updatecd();
    void chooseState(double distance);
    bool haspendatt();
    Attackbox takePendingAttackBox();


    Action aiState =Action::still;

    int aiTimer = 0;
    bool skillFired = false;

    bool pendattack = false;
    Battle pendbattle = Battle::attack;

    double sightrange = 450;
    double attackrange = 100;
    double harange =100;
    Enemytype type;
    Battle battle=Battle::attack;
    double attinterval=0;
    double atttimer=0;


};

#endif // ENEMY_H
