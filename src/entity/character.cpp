#include "character.h"

Character::Character()
    :Entity(),
    hp(10),
    maxhp(10),
    speed(3),
    direction(Direction::right),
    action(Action::still)
{
}

Character::Character(double x, double y, double w, double h)
    : Entity(x, y, w, h),
    hp(10),
    maxhp(10),
    speed(3),
    attack(1),
    direction(Direction::right),
    action(Action::still)
{
}


void Character::hurt(double att)
{
    if(!ealive) return;
    hp-=att;
    if(hp<=0)
    {
        hp=0;
        Entity::ealive=false;
    }
}



Direction Character:: getDirection()
{
     return direction;
}

void Character::updategame(double mapw,double maph)
{
    Q_UNUSED(mapw);
    Q_UNUSED(maph);
    return;
}