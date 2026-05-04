#ifndef PLAYER_H
#define PLAYER_H

#include<QRectF>
#include "src/entity/character.h"
#include <QKeyEvent>
#include <QtGlobal>
#include<QPaintEvent>
#include<QPainter>
#include<QPixmap>
#include<QtMath>
#include"src/combat/skill.h"

class Player:public Character
{
public:
    Player();

    void paint(QPainter &painter) override;
    void handleKeyP(QKeyEvent *event);
    void handleKeyR(QKeyEvent *event);
    void updategame(double mapw,double maph)override;
    void load();
    Attackbox createAttbox(Battle type);
    void updatecd();
    bool useskill(Battle type);


    bool keyW = false;
    bool keyS = false;
    bool keyA = false;
    bool keyD = false;
    Battle ptype;


};

#endif