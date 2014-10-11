#include "CQGaugeItem.h"
#include <qmath.h>

CQGaugeItem::CQGaugeItem()
{
   setFlag(ItemIsMovable);
   mScale = 0.0;
   mValue = 0.0;
   mChange = 0.0;
}

QRectF CQGaugeItem::boundingRect() const
{
   return QRect(10,20,100,100);
}
void CQGaugeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

   
   painter->setRenderHint(QPainter::Antialiasing);
   QRectF rectangle(10.0, 20.0, 100.0, 100.0);
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

	   //for a big line on top
    //   if(i==4)
    //        {
    //       pen.setWidthF(pen.widthF()+2.0);
    //        strich2.setY(midpoint.y()+(rectangle.width()*0.35)*sin(M_PI*((5.0/6.0)+i*(1.0/6.0))));
    //        }

       painter->setPen(pen);
	   //if you like some little lines on the Gauge, you can uncomment the drawLine command
      // painter->drawLine(strich1,strich2); 
        }
    
    //Value
    qreal startvalue = 5.0/6.0; //startangle of the gauge at 5/6 of 180 degree
        qreal span = 2.0/3.0; // the wing span of the gauge
        qreal k = M_PI*2;
        QPointF basis(rectangle.x()+(rectangle.width()/2.0),rectangle.y()+(rectangle.height()/2.0));
        QPointF boden1(basis.x()+(rectangle.width()/12)*cos(M_PI*(9.0/6.0)+(mScale*k*span)),basis.y()+(rectangle.height()/12)*sin(M_PI*(9.0/6.0)+(mScale*k*span)));
        QPointF boden2(basis.x()+(rectangle.width()/12)*cos(M_PI*(1.0/6.0)+(mScale*k*span)),basis.y()+(rectangle.height()/12)*sin(M_PI*(1.0/6.0)+(mScale*k*span)));

        //qreal laenge = sqrt(rectangle.width()*rectangle.width()+rectangle.height()*rectangle.height())*0.4;
        QPoint spitze(basis.x()+cos((M_PI*startvalue)+(mScale*k*span))*rectangle.width()*0.4,basis.y()+sin((M_PI*startvalue)+(mScale*k*span))*rectangle.height()*0.4);


    QPainterPath pointer;
    pointer.moveTo(boden1);
    pointer.lineTo(boden2);
    pointer.lineTo(spitze);
    pointer.lineTo(boden1);
    painter->fillPath(pointer,QColor(255,0,255));
   //Value

	QPen pen(Qt::black);
	 if (mChange == -1.0)
		pen.setColor(Qt::red);
	else if (mChange == 1.0)
	  pen.setColor(Qt::darkGreen);
	else
		pen.setColor(Qt::black);

	painter->setPen(pen);
	QFont font = painter->font() ;
	font.setPointSizeF(rectangle.height()/4.5);
	painter->setFont(font);
	if (mValue == 0.0)
		painter->drawText(rectangle.x()+rectangle.width()*0.5,rectangle.y()+rectangle.height()+(rectangle.height()/8.0),QString::number(mValue));
	else
		painter->drawText(rectangle.x(),rectangle.y()+rectangle.height()+(rectangle.height()/8.0),QString::number(mValue));
}

void CQGaugeItem::dragMoveEvent()
{
   update();
}

void CQGaugeItem::setValue(qreal wert)
{
	mScale = wert;
	
}

void CQGaugeItem::setConcentration(qreal wert)
{
	mValue = wert;
	
}

void CQGaugeItem::setChange(qreal wert)
{
	mChange = wert;
	
}

void CQGaugeItem::dragLeaveEvent()
{

update();
}


