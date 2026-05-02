#ifndef CHARACTER_H
#define CHARACTER_H

#include"src/entity/entity.h"
#include"src/core/gametypes.h"
#include<QPaintEvent>

#include<QKeyEvent>
class Character:public Entity
{
public:
    Character();
    Character(double x, double y, double w, double h);

    virtual void updategame(double mapw,double maph);
    void load();
    Direction getDirection();
    void hurt(double att);

    double hp=10;
    double maxhp=10;
    double speed=3;
    double attack;
    Direction direction=Direction::right;
    Action action;
};

#endif
