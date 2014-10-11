#include "CQGaugeItem.h"
#include <qmath.h>

CQGaugeItem::CQGaugeItem()
{
   setFlag(ItemIsMovable);
}

QRectF CQGaugeItem::boundingRect() const
{
   return QRect(10,20,400,400);
}
void CQGaugeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

   qreal mScale = 0.5;
   painter->setRenderHint(QPainter::Antialiasing);
   QRectF rectangle(10.0, 20.0, 400.0, 200.0);
   QRectF offset = rectangle;
   QRectF rect = rectangle;
   QRectF innerrect(rectangle.x()+(rectangle.width()*0.2),rectangle.y()+rectangle.height()*0.2,rectangle.width()*0.6,rectangle.height()*0.6);
   qreal startAngle;
   QPoint midpoint(rectangle.x()+rectangle.width()*0.5,rectangle.y()+rectangle.height()*0.5);


   QPainterPath path;
   QPointF outerpoint(midpoint.x()+(rectangle.width()*0.5)*cos(M_PI*((5.0/6.0)+0*(1.0/6.0))),midpoint.y()+(rectangle.height()*0.5)*sin(M_PI*((5.0/6.0)+0*(1.0/6.0))));
   QPointF innerpoint(midpoint.x()+(rectangle.width()*0.5)*cos(M_PI*(1.0+7*(1.0/6.0))),midpoint.y()+(rectangle.width()*0.5)*sin(M_PI*(1.0+7*(1.0/6.0))));

   startAngle = (210.0-0*30.0) ;
   path.moveTo(outerpoint);
   path.arcTo(rectangle,startAngle,-241.0);
   path.arcTo(innerrect,startAngle-241.0,241.0);
   path.closeSubpath();
   QConicalGradient gradient(midpoint,220);
   gradient.setColorAt(0.0,Qt::green);
   gradient.setColorAt((2.0/3.0),Qt::yellow);
   gradient.setColorAt(1.0,Qt::red);

   painter->fillPath(path,gradient);

   for (int i = 0; i <= 8; i=i+1)
       {
       QPointF strich1(midpoint.x()+(rectangle.width()*0.49)*cos(M_PI*((5.0/6.0)+i*(1.0/6.0))),midpoint.y()+(rectangle.height()*0.49)*sin(M_PI*((5.0/6.0)+i*(1.0/6.0))));
       QPointF strich2(midpoint.x()+(rectangle.width()*0.4)*cos(M_PI*((5.0/6.0)+i*(1.0/6.0))),midpoint.y()+(rectangle.height()*0.4)*sin(M_PI*((5.0/6.0)+i*(1.0/6.0))));

      QPen pen(Qt::black);

      pen.setWidthF(rectangle.width()/100.0);



      if (i ==0 || i == 8)
           {
          pen.setWidthF(pen.widthF()+0.0);
          strich1.setX(midpoint.x()+(rectangle.width()*0.49)*cos(M_PI*((5.0/6.0)+i*(1.0/6.0))));
          strich1.setY(midpoint.y()+(rectangle.height()*0.49)*sin(M_PI*((5.0/6.0)+i*(1.0/6.0))));
           strich2.setX(midpoint.x()+(rectangle.width()*0.3)*cos(M_PI*((5.0/6.0)+i*(1.0/6.0))));
           strich2.setY(midpoint.y()+(rectangle.height()*0.3)*sin(M_PI*((5.0/6.0)+i*(1.0/6.0))));

      }

   //   if(i==4)
   //        {
   //       pen.setWidthF(pen.widthF()+2.0);
   //        strich2.setY(midpoint.y()+(rectangle.width()*0.35)*sin(M_PI*((5.0/6.0)+i*(1.0/6.0))));
   //        }
      painter->setPen(pen);
      painter->drawLine(strich1,strich2);
       }
   //Value
   qreal startvalue = 5.0/6.0; //startangle of the gauge at 5/6 of 180 degree
       qreal span = 2.0/3.0; // the wing span of the gauge
       qreal k = M_PI*2;
       QPointF basis(offset.x()+(rect.width()/2.0),offset.y()+(rect.height()/2.0));
       QPointF boden1(basis.x()+(rect.width()/12)*cos(M_PI*(9.0/6.0)+(mScale*k*span)),basis.y()+(rect.height()/12)*sin(M_PI*(9.0/6.0)+(mScale*k*span)));
       QPointF boden2(basis.x()+(rect.width()/12)*cos(M_PI*(1.0/6.0)+(mScale*k*span)),basis.y()+(rect.height()/12)*sin(M_PI*(1.0/6.0)+(mScale*k*span)));

       //qreal laenge = sqrt(rect.width()*rect.width()+rect.height()*rect.height())*0.4;
       QPoint spitze(basis.x()+cos((M_PI*startvalue)+(mScale*k*span))*rect.width()*0.4,basis.y()+sin((M_PI*startvalue)+(mScale*k*span))*rect.height()*0.4);


   QPainterPath pointer;
   pointer.moveTo(boden1);
   pointer.lineTo(boden2);
   pointer.lineTo(spitze);
   pointer.lineTo(boden1);
   painter->fillPath(pointer,QColor(255,0,255));
   //Value
}

void CQGaugeItem::dragMoveEvent()
{
   update();
}

void CQGaugeItem::dragLeaveEvent()
{

update();
}


