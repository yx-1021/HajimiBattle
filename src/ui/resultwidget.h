#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "src/core/gametypes.h"

class ResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResultWidget(QWidget *parent = nullptr);

    void setResult(ResultType type, int score);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void returnMenuClicked();
    void retryClicked();

private:
    QPixmap bg;

    QLabel *scoreLabel;
    QPushButton *returnButton;
    QPushButton *retryButton;
};

#endif