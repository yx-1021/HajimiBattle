#include "mainwindow.h"
#include<src/ui/startmenu.h>
#include<src/core/gamewidget.h>
MainWindow::MainWindow(QWidget *parent)
    :QMainWindow{parent}
{
    resize(640,480);
    //构建菜单和游玩窗口
    stackwidget=new QStackedWidget(this);
    gamewidget=new GameWidget(this);
    startmenu=new StartMenu(this);

    stackwidget->addWidget(gamewidget);
    stackwidget->addWidget(startmenu);
    //实现窗口切换
    setCentralWidget(stackwidget);
    stackwidget->setCurrentWidget(startmenu);

    connect(startmenu,&StartMenu::easygameclicked,this,[=](){
        stackwidget->setCurrentWidget(gamewidget);
        gamewidget->setFocus();
    });

    connect(startmenu,&StartMenu::difgameclicked,this,[=](){
        stackwidget->setCurrentWidget(gamewidget);
        gamewidget->setFocus();
        });

    connect(startmenu,&StartMenu::setgameclicked,this,[=](){
        stackwidget->setCurrentWidget(gamewidget);
        gamewidget->setFocus();
    });

   connect(startmenu,&StartMenu::closeclicked,this,&MainWindow::close);


}
