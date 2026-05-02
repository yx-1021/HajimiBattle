#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QStackedWidget>

#include"gamewidget.h"
#include "src/core/gameconfig.h"
#include"src/ui/startmenu.h"
#include<src/core/gametypes.h>

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow()=default;
    QStackedWidget *stackwidget;

    GameWidget* gamewidget;
    StartMenu* startmenu;
    void entergame(GameConfig & config);



signals:

};

#endif // MAINWINDOW_H
