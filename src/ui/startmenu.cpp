#include "startmenu.h"
#include "qtimer.h"
#include "src/core/gameconfig.h"

StartMenu::StartMenu(QWidget *parent)
    : QWidget{parent}
{
    this->setFixedSize(1000, 600);
    //创建四种按钮
    QPushButton *levelButton = new QPushButton("普通模式", this);
    QPushButton *endlessButton = new QPushButton("无尽模式", this);
    QPushButton *setbutton = new QPushButton("彩蛋", this);
    QPushButton *closebutton = new QPushButton("退出游戏", this);

    QString buttonStyle =
        "QPushButton {"
        "font-size: 22px;"
        "font-weight: 900;"
        "color: black;"
        "background-color: rgb(241, 233, 208);"
        "border: none;"
        "border-radius: 18px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgb(232, 222, 192);"
        "}"
        "QPushButton:pressed {"
        "background-color: rgb(220, 210, 180);"
        "}";

    levelButton->setStyleSheet(buttonStyle);
    endlessButton->setStyleSheet(buttonStyle);
    setbutton->setStyleSheet(buttonStyle);
    closebutton->setStyleSheet(buttonStyle);

    levelButton->setGeometry(100,400,300,50);
    endlessButton->setGeometry(100,500,300,50);
    setbutton->setGeometry(600,400,300,50);
    closebutton->setGeometry(600,500,300,50);
    //添加背景
    bg.load(ASSET("background/cover.png"));

    connect(levelButton, &QPushButton::clicked, this, &StartMenu::levelclicked);
    connect(endlessButton, &QPushButton::clicked, this, &StartMenu::endlessclicked);
      connect(setbutton,&QPushButton::clicked,this,&StartMenu::setgameclicked);
      connect(closebutton,&QPushButton::clicked,this,&StartMenu::closeclicked);

      caidanLabel = new QLabel("哈基米似乎发生了一些变化~", this);
      caidanLabel->setGeometry(250, 310, 500, 60);
      caidanLabel->setAlignment(Qt::AlignCenter);
      caidanLabel->setStyleSheet(
          "QLabel {"
          "font-size: 26px;"
          "color: rgb(238,173,14);"
          "background-color: transparent;"
          "border-radius: 18px;"
          "}"
          );
      caidanLabel->hide();

}

void StartMenu::showCaidanMessage(const QString &text)
{
    if (!caidanLabel)
    {
        return;
    }

    caidanLabel->setText(text);
    caidanLabel->show();
    caidanLabel->raise();

    QTimer::singleShot(1800, this, [=]() {
        if (caidanLabel)
        {
            caidanLabel->hide();
        }
    });
}

void StartMenu::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.drawPixmap(rect(),bg);
}
