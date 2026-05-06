#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include "src/core/gametypes.h"

class GameConfig
{
public:
    Mode mode;
    int level=1;
    int enemynum=5;
    int enemyspeed=2;
    bool isendless=false;
    int enemyinterval=1500;
    bool iswin;
    bool isgameover;
    bool isCaidan = false;

};

#endif // GAMECONFIG_H
