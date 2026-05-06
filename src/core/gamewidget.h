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
#include <QPushButton>
#include <QIcon>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QDir>
#include <QResizeEvent>

#include "gameconfig.h"
#include "src/core/gameconfig.h"
#include "src/core/gametypes.h"
#include "src/entity/player.h"
#include "src/entity/enemy.h"
#include "src/combat/attackbox.h"
#include"src/combat/collision.h"
#include "src/ui/hud.h"
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
    void endByUser();
    int enemyScore(const Enemy &enemy);

    void initUi();
    void updateUiGeometry();
    void togglePause();

    void loadUiAssets();
    void loadSounds();

    void playEffect(QMediaPlayer *player);

    void resizeEvent(QResizeEvent *event) override;

    void initPauseOverlay();
    void showPauseOverlay();
    void hidePauseOverlay();
    void continueGame();
    void returnMenuFromPause();

    void drawCaidan(QPainter &painter);
    void applyNormal();
    void applyCaidan();

    void generateCure();
    void updateCures();
    void drawCures(QPainter &painter);

    void clampEntity(Entity &e);
    QRectF bodyRect(Entity &e);
    QRectF bodyRectAt(Entity &e, const QPointF &pos);

    bool playerBlockedAt(const QPointF &pos);
    bool enemyBlockedAt(int index, const QPointF &pos);
    void movePlayerWithBlock();
    void moveEnemyWithBlock(int index, const QPointF &playerCenter);

    QTimer *timer;
    GameConfig config;
    QPixmap backmap;

    QPixmap caidanRunPixmap;
    QPixmap caidanHaPixmap;

    Player player;
    QVector<Enemy> enemies;
    QVector<Attackbox> attackBoxes;
    double mapw=1000;
    double maph=600;
    int enemyinfcount=0;
    int score=0;

    bool gameover=false;
    bool win=false;

    bool paused = false;

    QPixmap curePixmap;
    QVector<QRectF> cureItems;

    int cureSpawnTimer = 0;

    QPushButton *pauseButton = nullptr;

    QPixmap skillAttackIcon;
    QPixmap skillHaIcon;
    QPixmap skillPlusAttackIcon;

    QMediaPlayer *bgmPlayer = nullptr;
    QAudioOutput *bgmAudio = nullptr;

    QMediaPlayer *playerattplay = nullptr;
    QAudioOutput *playerattaudio = nullptr;

    QMediaPlayer *playerhaplay = nullptr;
    QAudioOutput *playerhaaudio = nullptr;

    QMediaPlayer *playerplusplay = nullptr;
    QAudioOutput *playerplusaudio= nullptr;

    QMediaPlayer *ocatattplay = nullptr;
    QAudioOutput *ocatattaudio = nullptr;

    QMediaPlayer *ocathaplay = nullptr;
    QAudioOutput *ocathaaudio = nullptr;

    QMediaPlayer *ocatplusplay = nullptr;
    QAudioOutput *ocatplusaudio= nullptr;


    QMediaPlayer *boxattplay = nullptr;
    QAudioOutput *boxattaudio = nullptr;

    QMediaPlayer *winplay = nullptr;
    QAudioOutput *winaudio= nullptr;

    QMediaPlayer *loseplay= nullptr;
    QAudioOutput *loseaudio= nullptr;

    QMediaPlayer *endPlayer = nullptr;
    QAudioOutput *endAudio = nullptr;

    QWidget *pauseOverlay = nullptr;
    QWidget *pausePanel = nullptr;

    QPushButton *continueButton = nullptr;
    QPushButton *returnMenuButton = nullptr;
    QPushButton *endGameButton = nullptr;

signals:
    void gameFinished(ResultType type, int score);
    void returnMenu();


};

#endif // GAMEWIDGET_H
