#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H


#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPixmap>
#include <QVector>
#include <QRandomGenerator>

#include "src/core/gameconfig.h"
#include "src/core/gametypes.h"
#include "src/entity/player.h"
#include "src/entity/enemy.h"
#include "src/combat/attackbox.h"
#include "src/combat/skill.h"
#include "src/combat/collision.h"
class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent=nullptr);
    explicit GameWidget(const GameConfig &config, QWidget *parent=nullptr);

    void paintEvent(QPaintEvent *event)override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event)override;
    void keyReleaseEvent(QKeyEvent *event)override;
    void updategame();

    void removedead();
    void gamestate();
    void playeratt(Battle att);
    void enemyatt(Battle att,Enemy type);
    void generateenemy();
    void manyenemy();
    void setConfig(const GameConfig &newConfig);
    bool notoverlap(QRectF rect);
    void removeboxes();

    QTimer *timer;
    GameConfig config;
    QPixmap backmap;
    Player player;
    QVector<Enemy> enemies;
    QVector<Attackbox> attackBoxes;
    double mapw=1000;
    double maph=600;
    int enemyinfcount=0;
    int score=0;

    bool gameover=false;
    bool win=false;
    double attcool=0;
    double plusattcool=0;
    double hacool=0;



};

#endif // GAMEWIDGET_H
