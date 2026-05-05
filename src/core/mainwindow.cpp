#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent}
{
    resize(1000, 600);

    stackwidget = new QStackedWidget(this);

    startmenu = new StartMenu(this);
    gamewidget = new GameWidget(this);
    resultwidget = new ResultWidget(this);

    stackwidget->addWidget(startmenu);
    stackwidget->addWidget(gamewidget);
    stackwidget->addWidget(resultwidget);

    setCentralWidget(stackwidget);
    stackwidget->setCurrentWidget(startmenu);

    connect(startmenu, &StartMenu::levelclicked, this, [=]() {
        GameConfig config;
        config.mode = Mode::level;
        config.isendless = false;
        config.enemynum = 7;
        config.enemyinterval = 1500;

        entergame(config);
    });

    connect(startmenu, &StartMenu::endlessclicked, this, [=]() {
        GameConfig config;
        config.mode = Mode::endless;
        config.isendless = true;
        config.enemynum = 7;
        config.enemyinterval = 1500;

        entergame(config);
    });

    connect(startmenu, &StartMenu::setgameclicked, this, [=]() {
    });

    connect(startmenu, &StartMenu::closeclicked,
            this, &MainWindow::close);

    connect(gamewidget, &GameWidget::gameFinished,
            this, &MainWindow::showResult);

    connect(resultwidget, &ResultWidget::returnMenuClicked, this, [=]() {
        if (gamewidget->timer)
        {
            gamewidget->timer->stop();
        }

        stackwidget->setCurrentWidget(startmenu);
    });

    connect(resultwidget, &ResultWidget::retryClicked, this, [=]() {
        entergame(lastConfig);
    });
}

void MainWindow::entergame(const GameConfig &config)
{
    lastConfig = config;

    gamewidget->setConfig(config);
    stackwidget->setCurrentWidget(gamewidget);

    gamewidget->setFocus();
}

void MainWindow::showResult(ResultType type, int score)
{
    resultwidget->setResult(type, score);
    stackwidget->setCurrentWidget(resultwidget);
}