#include "gamewidget.h"
#include "gameconfig.h"
#include <QDebug>


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
    loadUiAssets();
    loadSounds();
    initUi();
    resize(mapw,maph);
    backmap.load(ASSET("background/backmap.png"));
    setFocusPolicy(Qt::StrongFocus);

    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&GameWidget::updategame);
}

//绘制画面
void GameWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.drawPixmap(rect(), backmap);

    drawCures(painter);

    if (config.isCaidan)
    {
        drawCaidan(painter);
    }
    else
    {
        player.paint(painter);
    }
    for(int i=0;i<enemies.size();i++)
    {
        enemies[i].paint(painter);
    }
    /*
    for (int i = 0; i < attackBoxes.size(); i++)
    {
        if (!attackBoxes[i].isExpired())
        {
            painter.save();
            painter.setPen(QPen(Qt::red));
            painter.setBrush(QColor(255, 0, 0, 80));
            painter.drawRect(attackBoxes[i].getrect());
            painter.restore();
        }
    }
*/
    HUD::drawHealthBar(painter, player);

    for (int i = 0; i < enemies.size(); i++)
    {
        HUD::drawHealthBar(painter, enemies[i]);
    }

    HUD::drawScore(painter, score);

    HUD::drawSkillIcons(painter,
                        skillAttackIcon,
                        skillHaIcon,
                        skillPlusAttackIcon,
                        width(),
                        height());
}

//彩蛋资源的绘制
void GameWidget::drawCaidan(QPainter &painter)
{
    QRectF r = player.rect();

    Action act = player.getAction();

    QPixmap pix;

    if (act == Action::attack ||
        act == Action::plusattack ||
        act == Action::ha)
    {
        pix = caidanHaPixmap;
    }
    else
    {
        pix = caidanRunPixmap;
    }

    if (!pix.isNull())
    {
        painter.drawPixmap(r.toRect(), pix);
    }
}
//一局游戏的配置
void GameWidget::setConfig(const GameConfig &newConfig)
{
    config = newConfig;

    enemies.clear();
    attackBoxes.clear();
    cureItems.clear();

   player.resetfornew();

    score = 0;
    enemyinfcount = 0;
    cureSpawnTimer = 0;
    gameover = false;
    win = false;
    paused = false;
    if (pauseOverlay)
    {
        pauseOverlay->hide();
    }

    if (!soundmodeinit|| currentcai!= config.isCaidan)
    {
        if (config.isCaidan)
        {
            applyCaidan();
        }
        else
        {
            applyNormal();
        }

        currentcai= config.isCaidan;
        soundmodeinit= true;
    }

    pendinitenemy= config.isendless ? 0 : config.enemynum;

    timer->start(20);

    for (int i = 0; i < 2; i++)
    {
        generateCure();
    }

    if (bgmPlayer)
    {
        bgmPlayer->stop();
        bgmPlayer->setPosition(0);
        bgmPlayer->play();
    }

    if (pauseButton)
    {
        pauseButton->show();
        pauseButton->raise();
    }

    update();
}




//同下面的函数一同处理键盘的按下与释放
void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if (gameover || paused)
    {
        return;
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

void GameWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        return;
    }

    if (gameover || paused)
    {
        return;
    }

    player.handleKeyR(event);
}

//处理鼠标左键的普攻
void GameWidget::mousePressEvent(QMouseEvent *event)
{
    if (gameover || paused)
    {
        return;
    }

    if (event->button() == Qt::LeftButton)
    {
        playeratt(Battle::attack);
    }
}

//处理玩家攻击
void GameWidget::playeratt(Battle att)
{
    if (gameover || paused)
    {
        return;
    }

    if (player.useskill(att))  //检查冷却僵直动作锁等是否准备好
    {
        if (att == Battle::attack)
        {
            playEffect(playerattplay);
        }
        else if (att == Battle::ha)
        {
            playEffect(playerhaplay);
        }
        else if (att == Battle::plusattack)
        {
            playEffect(playerplusplay);
        }

        attackBoxes.append(Skill::createAttbox(player, att, true));
    }
}

//同下一个函数一起处理敌人的生成
void GameWidget::manyenemy()
{
    for(int i=0;i<config.enemynum;i++)
    {
        generateenemy();
    }
}

void GameWidget::generateenemy()
{
    for (int t = 0; t < 50; t++)
    {
        double x = QRandomGenerator::global()->bounded(0, int(mapw - 100));
        double y = QRandomGenerator::global()->bounded(0, int(maph - 100));

        QRectF testRect(x, y, 100, 100);

        if (!notoverlap(testRect))
        {
            continue;
        }

        Enemytype type;

        if (QRandomGenerator::global()->bounded(2) == 0)
        {
            type = Enemytype::ocat;
        }
        else
        {
            type = Enemytype::box;
        }

        enemies.append(Enemy(type, x, y));
        return;
    }
}


//游戏主循环，更新玩家，敌人，攻击框，碰撞，画面刷新等
void GameWidget::updategame()
{
    if (gameover || paused)
    {
        update();
        return;
    }
    if (!config.isendless && pendinitenemy > 0)
    {
        int oldSize = enemies.size();

        generateenemy();

        if (enemies.size() > oldSize)
        {
            pendinitenemy--;
        }
    }

    player.updatecd();//更新玩家技能

    movePlayerWithBlock();

    updateCures();

    QPointF playerCenter = player.rect().center();

    for (int i = 0; i < enemies.size(); i++)
    {
        if (!enemies[i].isalive())
        {
            continue;
        }

        moveEnemyWithBlock(i, playerCenter);

        if (enemies[i].haspendatt())//不直接操作 attackBoxes，而是通过 pendattack 把攻击传给 GameWidget 处理
        {
            Battle enemyBattle = enemies[i].pendbattle;

            if (enemies[i].type == Enemytype::box)
            {
                playEffect(boxattplay);
            }
            else if (enemies[i].type == Enemytype::ocat)
            {
                if (enemyBattle == Battle::attack)
                {
                    playEffect(ocatattplay);
                }
                else if (enemyBattle == Battle::ha)
                {
                    playEffect(ocathaplay);
                }
                else if (enemyBattle == Battle::plusattack)
                {
                    playEffect(ocatplusplay);
                }
            }

            attackBoxes.append(enemies[i].takePendingAttackBox());
        }
    }

    for (int i = 0; i < attackBoxes.size(); i++)
    {
        attackBoxes[i].update();
    }

    Collision::handleColl(attackBoxes, player, enemies);
    removeboxes();

    removedead();
    gamestate();

    //无尽模式处理
    if (config.isendless)
    {
        if (enemies.size() < config.enemynum)
        {
            generateenemy();
        }
    }
    update();
}

//游戏状态的判断
void GameWidget::gamestate()
{
    if (!player.isalive())
    {
        gameover = true;
        win = false;

        timer->stop();

        if (bgmPlayer) bgmPlayer->stop();
        if (playerattplay) playerattplay->stop();
        if (playerhaplay) playerhaplay->stop();
        if (playerplusplay) playerplusplay->stop();

        if (ocatattplay) ocatattplay->stop();
        if (ocathaplay) ocathaplay->stop();
        if (ocatplusplay) ocatplusplay->stop();

        if (boxattplay) boxattplay->stop();

        playEffect(loseplay);
        emit gameFinished(ResultType::lose, score);

        return;
    }

   if (!config.isendless && pendinitenemy== 0 && enemies.isEmpty())
    {
        gameover = true;
        win = true;

        timer->stop();

        if (bgmPlayer) bgmPlayer->stop();
        if (playerattplay) playerattplay->stop();
        if (playerhaplay) playerhaplay->stop();
        if (playerplusplay) playerplusplay->stop();

        if (ocatattplay) ocatattplay->stop();
        if (ocathaplay) ocathaplay->stop();
        if (ocatplusplay) ocatplusplay->stop();

        if (boxattplay) boxattplay->stop();

        playEffect(winplay);
        emit gameFinished(ResultType::win, score);

        return;
    }
}

//同下一个函数一起处理被击败的敌人及得分规定
void GameWidget::removedead()
{
    for (int i = enemies.size() - 1; i >= 0; i--)
    {
        if (!enemies[i].isalive())
        {
            score += enemyScore(enemies[i]);
            enemies.removeAt(i);
        }
    }
}

int GameWidget::enemyScore(const Enemy &enemy)
{
    if (enemy.type == Enemytype::ocat)
    {
        return 20;
    }

    if (enemy.type == Enemytype::box)
    {
        return 10;
    }

}


//同下面的函数一起处理ui和暂停窗口
//初始化按钮
void GameWidget::initUi()
{
    pauseButton = new QPushButton(this);
    pauseButton->setFixedSize(48, 48);

   pauseButton->setIcon(QIcon(ASSET("ui/pause.png")));
    pauseButton->setIconSize(QSize(44, 44));

   pauseButton->setStyleSheet(HUD::iconButtonStyle());

    connect(pauseButton, &QPushButton::clicked,
            this, &GameWidget::showPauseOverlay);

    initPauseOverlay();

    updateUiGeometry();
    pauseButton->raise();
}
//初始化暂停页面及上面的按钮
void GameWidget::initPauseOverlay()
{
    pauseOverlay = new QWidget(this);
    pauseOverlay->setGeometry(rect());
    pauseOverlay->setStyleSheet(
        "background-color: rgba(0, 0, 0, 120);"
        );
    pauseOverlay->hide();

    pausePanel = new QWidget(pauseOverlay);
    pausePanel->setFixedSize(420, 280);
    pausePanel->setStyleSheet(HUD::pausePanelStyle());

    continueButton = new QPushButton("继续游戏", pausePanel);
    returnMenuButton = new QPushButton("回到菜单", pausePanel);
    endGameButton = new QPushButton("结束游戏", pausePanel);

    continueButton->setGeometry(110, 40, 200, 55);
    returnMenuButton->setGeometry(110, 115, 200, 55);
    endGameButton->setGeometry(110, 190, 200, 55);

    continueButton->setStyleSheet(HUD::menuButtonStyle());
    returnMenuButton->setStyleSheet(HUD::menuButtonStyle());
    endGameButton->setStyleSheet(HUD::menuButtonStyle());

    connect(continueButton, &QPushButton::clicked,
            this, &GameWidget::continueGame);

    connect(returnMenuButton, &QPushButton::clicked,
            this, &GameWidget::returnMenuFromPause);

    connect(endGameButton, &QPushButton::clicked,
            this, &GameWidget::endByUser);
}

//根据窗口大小调整位置
void GameWidget::updateUiGeometry()
{
    if (pauseButton)
    {
        pauseButton->move(width() - 60, 12);
    }

    if (pauseOverlay)
    {
        pauseOverlay->setGeometry(rect());
    }

    if (pausePanel)
    {
        pausePanel->move((width() - pausePanel->width()) / 2,
                         (height() - pausePanel->height()) / 2);
    }
}

void GameWidget::showPauseOverlay()
{
    if (gameover)
    {
        return;
    }

    paused = true;

    player.keyW = false;
    player.keyS = false;
    player.keyA = false;
    player.keyD = false;

    if (pauseOverlay)
    {
        pauseOverlay->show();
        pauseOverlay->raise();
    }

    update();
}

void GameWidget::hidePauseOverlay()
{
    if (pauseOverlay)
    {
        pauseOverlay->hide();
    }

    if (pauseButton)
    {
        pauseButton->raise();
    }

    update();
}
//继续游戏
void GameWidget::continueGame()
{
    if (gameover)
    {
        return;
    }

    paused = false;
    hidePauseOverlay();
}
//返回菜单
void GameWidget::returnMenuFromPause()
{
    paused = false;
    gameover = true;

    if (timer)
    {
        timer->stop();
    }

    if (bgmPlayer)
    {
        bgmPlayer->stop();
    }

    hidePauseOverlay();

    emit returnMenu();
}
//结束游戏
void GameWidget::endByUser()
{
    if (gameover)
    {
        return;
    }

    gameover = true;
    paused = false;
    win = false;

    if (timer)
    {
        timer->stop();
    }

    if (bgmPlayer)
    {
        bgmPlayer->stop();
    }

    hidePauseOverlay();

    if (playerattplay) playerattplay->stop();
    if (playerhaplay) playerhaplay->stop();
    if (playerplusplay) playerplusplay->stop();

    if (ocatattplay) ocatattplay->stop();
    if (ocathaplay) ocathaplay->stop();
    if (ocatplusplay) ocatplusplay->stop();

    if (boxattplay) boxattplay->stop();

    playEffect(loseplay);

    emit gameFinished(ResultType::end, score);
}

void GameWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateUiGeometry();
}



//播放音效
void GameWidget::playEffect(QMediaPlayer *player)
{
    if (player == nullptr)
    {
        return;
    }

    player->stop();
    player->setPosition(0);
    player->play();
}

//资源加载
void GameWidget::loadUiAssets()
{

    curePixmap.load(ASSET("player/cure.png"));
    skillAttackIcon.load(ASSET("ui/attack.png"));
    skillHaIcon.load(ASSET("ui/ha.png"));
    skillPlusAttackIcon.load(ASSET("ui/plusattack.png"));

    caidanRunPixmap.load(ASSET("caidan/caidan_run.png"));
    caidanHaPixmap.load(ASSET("caidan/caidan_ha.png"));
}


void GameWidget::loadSounds()
{
    bgmPlayer = new QMediaPlayer(this);
    bgmAudio = new QAudioOutput(this);

    bgmPlayer->setAudioOutput(bgmAudio);
    bgmPlayer->setSource(QUrl::fromLocalFile(ASSET("sound/background.mp3")));

    bgmAudio->setVolume(0.35f);
    bgmPlayer->setLoops(QMediaPlayer::Infinite);

    playerattplay = new QMediaPlayer(this);
    playerattaudio = new QAudioOutput(this);
    playerattplay->setAudioOutput(playerattaudio);
   playerattplay->setSource(QUrl::fromLocalFile(ASSET("sound/player_attack.mp3")));
    playerattaudio->setVolume(0.75f);



    playerhaplay = new QMediaPlayer(this);
    playerhaaudio = new QAudioOutput(this);
    playerhaplay->setAudioOutput(playerhaaudio);
    playerhaplay->setSource(QUrl::fromLocalFile(ASSET("sound/player_ha.mp3")));
    playerhaaudio->setVolume(0.75f);

    playerplusplay = new QMediaPlayer(this);
    playerplusaudio = new QAudioOutput(this);
    playerplusplay->setAudioOutput(playerplusaudio);
    playerplusplay->setSource(QUrl::fromLocalFile(ASSET("sound/player_plusattack.mp3")));
    playerplusaudio->setVolume(0.8f);

    ocatattplay = new QMediaPlayer(this);
    ocatattaudio = new QAudioOutput(this);
    ocatattplay->setAudioOutput(ocatattaudio);
    ocatattplay->setSource(QUrl::fromLocalFile(ASSET("sound/ocat_attack.mp3")));
    ocatattaudio->setVolume(0.7f);

    ocathaplay = new QMediaPlayer(this);
    ocathaaudio = new QAudioOutput(this);
    ocathaplay->setAudioOutput(ocathaaudio);
    ocathaplay->setSource(QUrl::fromLocalFile(ASSET("sound/ocat_ha.mp3")));
    ocathaaudio->setVolume(0.75f);

    ocatplusplay = new QMediaPlayer(this);
    ocatplusaudio = new QAudioOutput(this);
    ocatplusplay->setAudioOutput(ocatplusaudio);
    ocatplusplay->setSource(QUrl::fromLocalFile(ASSET("sound/ocat_plusattack.mp3")));
    ocatplusaudio->setVolume(0.8f);


    boxattplay= new QMediaPlayer(this);
    boxattaudio = new QAudioOutput(this);
    boxattplay->setAudioOutput(boxattaudio);
    boxattplay->setSource(QUrl::fromLocalFile(ASSET("sound/box_attack.mp3")));
    boxattaudio->setVolume(0.7f);

    winplay = new QMediaPlayer(this);
    winaudio = new QAudioOutput(this);
    winplay->setAudioOutput(winaudio);
    winplay->setSource(QUrl::fromLocalFile(ASSET("sound/win.mp3")));
    winaudio->setVolume(0.8f);

    loseplay = new QMediaPlayer(this);
    loseaudio = new QAudioOutput(this);
    loseplay->setAudioOutput(loseaudio);
    loseplay->setSource(QUrl::fromLocalFile(ASSET("sound/lose.mp3")));

    endPlayer = new QMediaPlayer(this);
    endAudio = new QAudioOutput(this);
    endPlayer->setAudioOutput(endAudio);
    endPlayer->setSource(QUrl::fromLocalFile(ASSET("sound/end.mp3")));
    endAudio->setVolume(0.8f);
}

//防止重叠
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



void GameWidget::removeboxes()
{
        for (int i = attackBoxes.size() - 1; i >= 0; i--)
        {
            if (attackBoxes[i].isExpired())
            {
                attackBoxes.removeAt(i);
            }
        }
}

//正常模式
void GameWidget::applyNormal()
{
    if (bgmPlayer)
    {
        bgmPlayer->stop();
        bgmPlayer->setSource(QUrl::fromLocalFile(ASSET("sound/background.mp3")));
        bgmPlayer->setLoops(QMediaPlayer::Infinite);
    }

    if (playerattplay)
    {
        playerattplay->setSource(QUrl::fromLocalFile(ASSET("sound/player_attack.mp3")));
    }

    if (playerplusplay)
    {
        playerplusplay->setSource(QUrl::fromLocalFile(ASSET("sound/player_plusattack.mp3")));
    }

    if (playerhaplay)
    {
        playerhaplay->setSource(QUrl::fromLocalFile(ASSET("sound/player_ha.mp3")));
    }
}
//彩蛋模式
void GameWidget::applyCaidan()
{
    if (bgmPlayer)
    {
        bgmPlayer->stop();
        bgmPlayer->setSource(QUrl::fromLocalFile(ASSET("caidan/caidan_bgm.mp3")));
        bgmPlayer->setLoops(QMediaPlayer::Infinite);
    }

    if (playerattplay)
    {
        playerattplay->setSource(QUrl::fromLocalFile(ASSET("caidan/caidan_attack.mp3")));
    }

    if (playerplusplay)
    {
        playerplusplay->setSource(QUrl::fromLocalFile(ASSET("caidan/caidan_attack.mp3")));
    }

    if (playerhaplay)
    {
        playerhaplay->setSource(QUrl::fromLocalFile(ASSET("caidan/caidan_ha.mp3")));
    }
}
//和下面的函数一起处理回血道具的生成和更新
void GameWidget::generateCure()
{
    for (int t = 0; t < 50; t++)
    {
        double x = QRandomGenerator::global()->bounded(40, width() - 90);
        double y = QRandomGenerator::global()->bounded(40, height() - 90);

        QRectF cureRect(x, y, 50, 50);

        bool ok = true;

        if (cureRect.intersects(player.rect().adjusted(-40, -40, 40, 40)))
        {
            ok = false;
        }

        for (int i = 0; i < enemies.size(); i++)
        {
            if (cureRect.intersects(enemies[i].rect().adjusted(-20, -20, 20, 20)))
            {
                ok = false;
                break;
            }
        }

        for (int i = 0; i < cureItems.size(); i++)
        {
            if (cureRect.intersects(cureItems[i].adjusted(-20, -20, 20, 20)))
            {
                ok = false;
                break;
            }
        }

        if (ok)
        {
            cureItems.append(cureRect);
            return;
        }
    }
}

void GameWidget::updateCures()
{
    for (int i = cureItems.size() - 1; i >= 0; i--)
    {
        if (player.rect().intersects(cureItems[i]))
        {
            player.hp = qMin(player.maxhp, player.hp + 10.0);
            cureItems.removeAt(i);
        }
    }

    if (config.isendless)
    {
        cureSpawnTimer += timer->interval();

        if (cureSpawnTimer >= 8000)
        {
            cureSpawnTimer = 0;

            if (cureItems.size() < 3)
            {
                generateCure();
            }
        }
    }
}

void GameWidget::drawCures(QPainter &painter)
{
    for (int i = 0; i < cureItems.size(); i++)
    {
        if (!curePixmap.isNull())
        {
            painter.drawPixmap(cureItems[i].toRect(), curePixmap);
        }
        else
        {
            painter.save();
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(80, 230, 120, 180));
            painter.drawEllipse(cureItems[i]);
            painter.restore();
        }
    }
}

//以下函数用于防止重叠或超出地图
QRectF GameWidget::bodyRect(Entity &e)
{
    QRectF r = e.rect();

    double leftRight = r.width() * 0.22;
    double top = r.height() * 0.28;
    double bottom = r.height() * 0.10;

    return r.adjusted(leftRight, top, -leftRight, -bottom);
}

void GameWidget::clampEntity(Entity &e)
{
    e.epos.setX(qBound(0.0, e.epos.x(), qMax(0.0, mapw - e.esize.width())));
    e.epos.setY(qBound(0.0, e.epos.y(), qMax(0.0, maph - e.esize.height())));
}

//计算碰撞区域
QRectF GameWidget::bodyRectAt(Entity &e, const QPointF &pos)
{
    QRectF r(pos, e.esize);

    double leftRight = r.width() * 0.25;
    double top = r.height() * 0.30;
    double bottom = r.height() * 0.12;

    return r.adjusted(leftRight, top, -leftRight, -bottom);
}

//判断玩家
bool GameWidget::playerBlockedAt(const QPointF &pos)
{
    QRectF pr = bodyRectAt(player, pos);

    for (int i = 0; i < enemies.size(); i++)
    {
        if (!enemies[i].isalive())
        {
            continue;
        }

        if (pr.intersects(bodyRect(enemies[i])))
        {
            return true;
        }
    }

    return false;
}

//判断敌人
bool GameWidget::enemyBlockedAt(int index, const QPointF &pos)
{
    if (index < 0 || index >= enemies.size())
    {
        return true;
    }

    QRectF er = bodyRectAt(enemies[index], pos);

    if (player.isalive() && er.intersects(bodyRect(player)))
    {
        return true;
    }

    for (int i = 0; i < enemies.size(); i++)
    {
        if (i == index)
        {
            continue;
        }

        if (!enemies[i].isalive())
        {
            continue;
        }

        if (er.intersects(bodyRect(enemies[i])))
        {
            return true;
        }
    }

    return false;
}

//移动玩家并防止碰撞重叠
void GameWidget::movePlayerWithBlock()
{
    QPointF oldPos = player.epos;

    player.updategame(mapw, maph);

    QPointF targetPos = player.epos;
    QPointF fixedPos = oldPos;

    QPointF tryX(targetPos.x(), oldPos.y());

    if (!playerBlockedAt(tryX))
    {
        fixedPos.setX(targetPos.x());
    }

    QPointF tryY(fixedPos.x(), targetPos.y());

    if (!playerBlockedAt(tryY))
    {
        fixedPos.setY(targetPos.y());
    }

    player.epos = fixedPos;
}

//移动敌人并防止碰撞重叠
void GameWidget::moveEnemyWithBlock(int index, const QPointF &playerCenter)
{
    QPointF oldPos = enemies[index].epos;

    enemies[index].updategame(mapw, maph, playerCenter);

    QPointF targetPos = enemies[index].epos;
    QPointF fixedPos = oldPos;

    QPointF tryX(targetPos.x(), oldPos.y());

    if (!enemyBlockedAt(index, tryX))
    {
        fixedPos.setX(targetPos.x());
    }

    QPointF tryY(fixedPos.x(), targetPos.y());

    if (!enemyBlockedAt(index, tryY))
    {
        fixedPos.setY(targetPos.y());
    }

    enemies[index].epos = fixedPos;
}

