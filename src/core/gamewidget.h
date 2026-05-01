#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include<QPainter>
#include<QTimer>
#include<QKeyEvent>
#include<QPixmap>
#include<QtGlobal>
class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event)override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event)override;
    void updategame();
    QTimer *timer;

    QPixmap backmap;
    QPixmap playermap;

    int playerx,playery;
    int playerwidth,playerheight;

    bool keyW = false;
    bool keyS = false;
    bool keyA = false;
    bool keyD = false;
    int speed=3;


};

#endif // GAMEWIDGET_H
