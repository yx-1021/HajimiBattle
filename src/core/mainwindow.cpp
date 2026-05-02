#include "mainwindow.h"
#include<src/ui/startmenu.h>
#include<src/core/gamewidget.h>
#include<src/core/gametypes.h>
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

    connect(startmenu,&StartMenu::levelclicked,this,[=](){
        GameConfig config;
        config.mode=Mode::level;
        entergame(config);
    });

    connect(startmenu,&StartMenu::endlessclicked,this,[=](){
        GameConfig config;
        config.mode=Mode::endless;
        entergame(config);
        });

    connect(startmenu,&StartMenu::setgameclicked,this,[=](){

    });

   connect(startmenu,&StartMenu::closeclicked,this,&MainWindow::close);


}

 void MainWindow::entergame(GameConfig & config)
{
     gamewidget->setConfig(config);
     stackwidget->setCurrentWidget(gamewidget);
    gamewidget->setFocus();
}
