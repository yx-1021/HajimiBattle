#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include "src/core/gametypes.h"

class GameConfig
{
public:
    Mode mode;
    int level=1;
    int enemynum=7;
    int enemyspeed=3;
    bool isendless=false;
    int enemyinterval=1500;
    bool iswin;
    bool isgameover;

};

#endif // GAMECONFIG_H
