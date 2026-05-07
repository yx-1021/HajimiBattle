#ifndef CHARACTER_H
#define CHARACTER_H

#include "src/entity/entity.h"
#include "src/core/gametypes.h"

#include <QPaintEvent>
#include <QVector>
#include <QString>
#include <QPixmap>
#include <QKeyEvent>
#include<QString>

class Character : public Entity
{
public:
    Character();
    Character(double x, double y, double w, double h);

    virtual void updategame(double mapw, double maph);
    void load();

    Direction getDirection();
    Action getAction();

    void setDirection(Direction d);
    void setAction(Action a);

    void playaction(Action a, int lockTicks);
    bool isActionLocked();

    void updateaction();
    void updateanimation();
    void updateactionlock();

    void hurt(double att);

    QPixmap currentPixmap();

    void loadaction(const QString &roleName);

    void changeRole(const QString &roleName);
    bool actionLocked();

    static QString Actionname(Action a);
    static QString Direcname(Direction d);

    void addStiff(int ticks);
    void updateStiff();
    bool isStiff();
    QString assetRoot();

    double attackcd = 0;
    double hacd = 0;
    double pluscd = 0;
    double halayer = 0;
    bool plusready = false;

    double hp = 10;
    double maxhp = 10;
    double speed = 3;
    double attack = 1;

    Direction direction = Direction::right;
    Action action = Action::still;


    static const int ACTION = 5;
    static const int DIRECT = 4;

    QString role;

    QVector<QPixmap> sprites[ACTION][DIRECT];

    int frameid = 0;
    int frametick = 0;
    int frameinterval = 8;

    Action lastAction = Action::still;
    Direction lastDirection = Direction::right;

    int actionLock = 0;
    int stiffTicks = 0;
};

#endif