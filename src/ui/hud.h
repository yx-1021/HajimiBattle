#ifndef HUD_H
#define HUD_H

#include <QString>
#include <QPainter>
#include <QPixmap>
#include <QPen>
#include <QColor>
#include <QFont>
#include <QRect>
#include <QRectF>

#include "src/entity/character.h"

class HUD
{
public:
    HUD() = default;

    static QString iconButtonStyle();
    static QString pausePanelStyle();
    static QString menuButtonStyle();

    static void drawScore(QPainter &painter, int score);
    static void drawHealthBar(QPainter &painter, Character &ch);
    static void drawSkillIcons(QPainter &painter,
                               const QPixmap &attackIcon,
                               const QPixmap &haIcon,
                               const QPixmap &plusAttackIcon,
                               int widgetWidth,
                               int widgetHeight);
};

#endif
