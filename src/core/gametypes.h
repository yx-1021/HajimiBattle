#ifndef GAMETYPES_H
#define GAMETYPES_H


enum class Mode
{
    level,endless
};

enum class Direction
{
    up=0,down=1,left=2,right=3
};

enum class Action
{
    still=0,run=1,attack=2,ha=3,plusattack=4
};

enum class Battle
{
    attack,ha,plusattack
};

enum class Enemytype
{
    ocat,box
};
enum class ResultType
{
    win,lose,end
};

#endif // GAMETYPES_H