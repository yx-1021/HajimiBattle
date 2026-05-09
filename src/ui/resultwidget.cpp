#include "resultwidget.h"
#include "src/core/gameconfig.h"
#include <QDebug>


//结算界面
ResultWidget::ResultWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(1000, 600);

    scoreLabel = new QLabel(this);
    scoreLabel->setAlignment(Qt::AlignCenter);
    scoreLabel->setStyleSheet(
        "QLabel {"
        "font-size: 42px;"
        "font-weight: bold;"
        "color: black;"
        "border-radius: 16px;"
        "padding: 12px 30px;"
        "}"
        );

    returnButton = new QPushButton("返回菜单", this);
    retryButton = new QPushButton("再来一局", this);

    returnButton->setFixedSize(160, 50);
    retryButton->setFixedSize(160, 50);

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


    returnButton->setStyleSheet(buttonStyle);
    retryButton->setStyleSheet(buttonStyle);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 350, 0,75);

    QHBoxLayout *scoreLayout = new QHBoxLayout;
    scoreLayout->addStretch();
    scoreLayout->addWidget(scoreLabel);
    scoreLayout->addStretch();

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(returnButton);
    buttonLayout->addSpacing(80);
    buttonLayout->addWidget(retryButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(scoreLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    connect(returnButton, &QPushButton::clicked,
            this, &ResultWidget::returnMenuClicked);

    connect(retryButton, &QPushButton::clicked,
            this, &ResultWidget::retryClicked);
}

void ResultWidget::setResult(ResultType type, int score)
{
    QString path;

    if (type == ResultType::win)
    {
       path = ASSET("background/win.png");
    }
    else if (type == ResultType::lose)
    {
        path = ASSET("background/lose.png");
    }
    else
    {
        path = ASSET("background/end.png");
    }

    bg.load(path);
    scoreLabel->setText(QStringLiteral(u"分数: %1").arg(score));

    update();
}

void ResultWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    if (!bg.isNull())
    {
        painter.drawPixmap(rect(), bg);
    }
    else
    {
        painter.fillRect(rect(), Qt::black);
    }
}