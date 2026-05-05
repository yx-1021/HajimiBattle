#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include"src/ui/resultwidget.h"
#include "src/core/gamewidget.h"
#include "src/core/gameconfig.h"
#include "src/core/gametypes.h"
#include "src/ui/startmenu.h"
#include "src/ui/resultwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

    QStackedWidget *stackwidget;

    GameWidget *gamewidget;
    StartMenu *startmenu;
    ResultWidget *resultwidget;

    GameConfig lastConfig;

    void entergame(const GameConfig &config);
    void showResult(ResultType type, int score);
};

#endif