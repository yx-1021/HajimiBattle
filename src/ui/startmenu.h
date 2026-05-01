#ifndef STARTMENU_H
#define STARTMENU_H

#include <QWidget>
#include<QPushButton>
#include<QVBoxLayout>
#include<QLayout>
#include<QLabel>
#include<QPixmap>
#include<QPaintEvent>
#include<Qpainter>
class StartMenu: public QWidget
{
    Q_OBJECT
public:
    explicit StartMenu(QWidget *parent = nullptr);
    QPixmap bg;
        void paintEvent(QPaintEvent *event) override;

signals:
    void easygameclicked();
    void difgameclicked();
    void setgameclicked();
    void closeclicked();
};

#endif // MAINWINDOW_H

