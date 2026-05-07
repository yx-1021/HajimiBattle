#include "hud.h"


//界面ui绘制
QString HUD::iconButtonStyle()
{
    return
        "QPushButton {"
        "border: none;"
        "background-color: transparent;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(255, 255, 255, 70);"
        "border-radius: 10px;"
        "}";
}

QString HUD::pausePanelStyle()
{
    return
        "QWidget {"
        "background-color: rgba(255, 255, 255, 210);"
        "border-radius: 22px;"
        "}";
}

QString HUD::menuButtonStyle()
{
    return
        "QPushButton {"
        "font-size: 22px;"
        "font-weight: bold;"
        "border-radius: 14px;"
        "background-color: rgba(255, 245, 220, 230);"
        "color: black;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(255, 220, 160, 240);"
        "}";
}

void HUD::drawScore(QPainter &painter, int score)
{
    painter.save();

    painter.setPen(Qt::white);
    painter.setFont(QFont("Microsoft YaHei", 24, QFont::Bold));

    painter.drawText(QRect(20, 15, 260, 50),
                     Qt::AlignLeft | Qt::AlignVCenter,
                     QString("分数：%1").arg(score));

    painter.restore();
}

void HUD::drawHealthBar(QPainter &painter, Character &ch)
{
    if (!ch.isalive())
    {
        return;
    }

    if (ch.maxhp <= 0)
    {
        return;
    }

    QRectF r = ch.rect();

    double barW = r.width() * 0.75;
    double barH = 7;

    double x = r.center().x() - barW / 2;
    double y = r.top() - 12;

    if (y < 2)
    {
        y = r.top() + 4;
    }

    double rate = ch.hp / ch.maxhp;

    if (rate < 0) rate = 0;
    if (rate > 1) rate = 1;

    QRectF bgRect(x, y, barW, barH);
    QRectF hpRect(x, y, barW * rate, barH);

    painter.save();

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(60, 60, 60, 180));
    painter.drawRoundedRect(bgRect, 3, 3);

    painter.setBrush(QColor(80, 220, 80, 230));
    painter.drawRoundedRect(hpRect, 3, 3);

    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(bgRect, 3, 3);

    painter.restore();
}

void HUD::drawSkillIcons(QPainter &painter,
                         const QPixmap &attackIcon,
                         const QPixmap &haIcon,
                         const QPixmap &plusAttackIcon,
                         int widgetWidth,
                         int widgetHeight)
{
    const int iconSize = 58;
    const int spacing = 14;
    const int margin = 24;

    int totalW = iconSize * 3 + spacing * 2;

    int startX = widgetWidth - margin - totalW;
    int y = widgetHeight - margin - iconSize;

    auto drawOneSkill = [&](const QPixmap &pix, int x)
    {
        QRect iconRect(x, y, iconSize, iconSize);

        painter.save();

        painter.setPen(QPen(Qt::white, 2));
        painter.setBrush(QColor(0, 0, 0, 100));
        painter.drawRoundedRect(iconRect.adjusted(-3, -3, 3, 3), 10, 10);

        if (!pix.isNull())
        {
            painter.drawPixmap(iconRect, pix);
        }

        painter.restore();
    };

    drawOneSkill(attackIcon, startX);
    drawOneSkill(haIcon, startX + iconSize + spacing);
    drawOneSkill(plusAttackIcon, startX + (iconSize + spacing) * 2);
}


