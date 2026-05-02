#ifndef ENTITY_H
#define ENTITY_H

#include <QPixmap>
#include <QPainter>
#include <QRectF>
#include <QPointF>
#include <QSizeF>
class Entity
{
public:
    Entity();
    Entity(double x,double y,double w,double h);
     ~Entity() = default;

    virtual void update();
    virtual void paint(QPainter &painter);

    QRectF rect();

    QPointF point();
    void setp(double x,double y);

    QSizeF size();
    void setsize(double w,double h);

    bool isalive();
    void setal();


    QPointF epos;
    QSizeF esize;
    QPixmap epixmap;
    bool ealive=true;

};

#endif // ENTITY_H
