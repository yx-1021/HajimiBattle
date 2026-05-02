#include "entity.h"

Entity::Entity() {

}
Entity::Entity(double x,double y,double w,double h)
{
    epos=QPointF(x,y);
    esize=QSizeF(w,h);
    ealive=true;
}
void Entity::update()
{

}
void Entity::paint(QPainter &painter)
{
    if(ealive==true)
    {
        painter.drawPixmap(rect().toRect(),epixmap);
    }
    else return;
}

QRectF Entity::rect()
{
    return QRectF(epos,esize);
}
 QPointF Entity::point()
{
     return epos;
}
void Entity::setp(double x,double y)
{
    epos=QPointF(x,y);
}

QSizeF Entity::size()
{
    return esize;
}
void Entity::setsize(double w,double h)
{
    esize=QSizeF(w,h);
}

void Entity::setal()
{
    ealive=false;
}

bool Entity::isalive()
{
    return ealive;
}