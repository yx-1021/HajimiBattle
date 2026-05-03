#ifndef GAMETYPES_H
#define GAMETYPES_H


enum class Mode
{
    level,endless
};

enum class Direction
{
    up,down,left,right
};

enum class Action
{
    still,run,attack,ha,plusattack,win,lose
};

enum class Battle
{
    attack,ha,plusattack
};

enum class Enemytype
{
    ocat,box
};

#endif // GAMETYPES_H