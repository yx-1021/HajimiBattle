#ifndef SKILL_H
#define SKILL_H

#include "src/entity/character.h"
#include "src/combat/attackbox.h"
#include "src/core/gametypes.h"

class Skill
{
public:
    static Attackbox createAttbox(Character &owner,
                                     Battle type,
                                     bool fromPlayer);


};

#endif
