#ifndef PLAYER_H
#define PLAYER_H

#include <QRect>
#include <QPainter>
#include <QKeyEvent>
#include <QPixmap>
#include <QtMath>

#include "character.h"
#include "src/combat/attackbox.h"
#include "src/combat/skill.h"
#include"src/core/gametypes.h"

class Player:public Character
{
public:
    Player();

    void paint(QPainter &painter) override;
    void handleKeyP(QKeyEvent *event);
    void handleKeyR(QKeyEvent *event);
    void updategame(double mapw,double maph)override;
    void load();
    void updatecd();
    bool useskill(Battle type);
    void resetfornew();


    bool keyW = false;
    bool keyS = false;
    bool keyA = false;
    bool keyD = false;
    Battle ptype;


};

#endif