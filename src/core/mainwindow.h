#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QStackedWidget>

#include"gamewidget.h"
#include"src/ui/startmenu.h"

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow()=default;

private:
    QStackedWidget *stackwidget;

    GameWidget* gamewidget;
    StartMenu* startmenu;



signals:

};

#endif // MAINWINDOW_H
