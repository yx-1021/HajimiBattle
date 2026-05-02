#include "gamewidget.h"



GameWidget::GameWidget(QWidget *parent)
    : GameWidget(GameConfig(), parent)
{
}
GameWidget::GameWidget(const GameConfig &config, QWidget *parent)
    :QWidget(parent),
    config(config),
    enemyinfcount(0),
    score(0),
    gameover(false),
    win(false)
{
    resize(mapw,maph);
    backmap.load(":/new/prefix1/rescource/backmap.png");
    setFocusPolicy(Qt::StrongFocus);

    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&GameWidget::updategame);
    timer->start(10);

    manyenemy();
}
void GameWidget::setConfig(const GameConfig &newConfig)
{
    config = newConfig;

    enemies.clear();
    attackBoxes.clear();

    player = Player();

    score = 0;
    enemyinfcount = 0;
    gameover = false;
    win = false;

    manyenemy();

    update();
}
void GameWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.drawPixmap(rect(), backmap);
    player.paint(painter);
    for(int i=0;i<enemies.size();i++)
    {
        enemies[i].paint(painter);
    }
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if (gameover) return;
    if (event->key() == Qt::LeftButton)
    {
        playeratt(Battle::attack);
    }

    if (event->key() == Qt::Key_E)
    {
          playeratt(Battle::ha);

    }

    if (event->key() == Qt::Key_Q)
    {
         playeratt(Battle::plusattack);
    }
    player.handleKeyP(event);

}
void GameWidget::playeratt(Battle att)
{

    if (gameover) {
        return;
    }

    attackBoxes.append(Skill::createAttbox(player, att, true));
}


void GameWidget::enemyatt(Battle att,Enemy type)
{
    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i].canatt()) {
            attackBoxes.append(Skill::createAttbox(enemies[i], att, false));
        }
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) return;

    if(event->key()==Qt::Key_W)
    {
        player.handleKeyR(event);
    }
    if(event->key()==Qt::Key_S)
    {
        player.handleKeyR(event);
    }
    if(event->key()==Qt::Key_A)
    {
        player.handleKeyR(event);
    }
    if(event->key()==Qt::Key_D)
    {
        player.handleKeyR(event);
    }

}


void GameWidget::manyenemy()
{
    for(int i=0;i<config.enemynum;i++)
    {
        generateenemy();
    }
}

void GameWidget::generateenemy()
{
    double x=QRandomGenerator::global()->bounded(0,width()-100);
    double y=QRandomGenerator::global()->bounded(0,height()-100);

    Enemytype type;

    if (QRandomGenerator::global()->bounded(2) == 0)
    {
        type = Enemytype::ocat;
    }
    else
    {
        type = Enemytype::box;
    }

    Enemy enemy(type, x, y);

    if (notoverlap(enemy.rect())) {
        enemies.append(enemy);
        return;
    }

}

void GameWidget::updategame()
{
    if (!gameover)
    {
        player.updategame(mapw, maph);
        QPointF playerCenter = player.rect().center();

        for (int i = 0; i < enemies.size(); i++) {
            enemies[i].updategame(mapw, maph, playerCenter);

            if (enemies[i].canatt()) {
                attackBoxes.append(enemies[i].createAttBox());
            }
        }

        for (int i = 0; i < attackBoxes.size(); i++) {
            attackBoxes[i].update();
        }

        Collision::handleColl(attackBoxes, player, enemies);

        removedead();
        gamestate();

        if (config.isendless) {
            enemyinfcount += 16;

            if (enemyinfcount >= config.enemyinterval) {
                enemyinfcount = 0;
                generateenemy();
            }
        }
    }

    update();
}

void GameWidget::removedead()
{
    for (int i = enemies.size() - 1; i >= 0; i--)
    {
        if (!enemies[i].isalive())
        {
            enemies.removeAt(i);
        }
    }
}

void GameWidget::gamestate()
{
    if (!player.isalive())
    {
        gameover = true;
        win = false;
        return;
    }

    if (!config.isendless && enemies.isEmpty())
    {
        gameover = true;
        win = true;
        return;
    }
}

bool GameWidget::notoverlap(QRectF rect)
{
    QRectF playerSafeRect = player.rect().adjusted(-30, -30, 30, 30);

    if (rect.intersects(playerSafeRect)) {
        return false;
    }

    for (int i = 0; i < enemies.size(); i++) {
        QRectF enemySafeRect = enemies[i].rect().adjusted(-20, -20, 20, 20);

        if (rect.intersects(enemySafeRect)) {
            return false;
        }
    }

    return true;
}
