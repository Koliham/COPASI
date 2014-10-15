// Copyright (C) 2013 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#include <qlayout/CQGaugeEffect.h>

#include <QtGui/QPainter>
#include <QtGui/QGraphicsEffect>
#include <qmath.h>
#include <qbrush.h>

CQGaugeEffect::CQGaugeEffect()
  : mScale(0.0)
  , mValue(0.0)
  , mChange(0.0)
{
}

CQGaugeEffect::~CQGaugeEffect()
{
}

void CQGaugeEffect::setScale(qreal scale)
{
	mScale = 0.0;
	if (scale <= 1.0 && scale >= 0.0)
		mScale = scale;



  if (scale > 1.0)
	  mScale = 1.0;
  if (scale < 0.0)
	  mScale = 0.0;
}

void CQGaugeEffect::setChange(qreal change)
{
  mChange = change;
}

void CQGaugeEffect::setValue(qreal wert)
{

	mValue = 0.0;
	if (wert != NULL)
  mValue = wert;

}


void CQGaugeEffect::draw(QPainter* painter)
{
  QPoint offset;
  const QPixmap pixmap = sourcePixmap(Qt::DeviceCoordinates, &offset);

  if (pixmap.isNull())
    return;

  painter->save();
  painter->setWorldTransform(QTransform());
  QRectF rect = pixmap.rect();

  QTransform trans = QTransform::fromScale(1.0,1.0);
  QPixmap transformed = pixmap.transformed(trans, Qt::SmoothTransformation);
  QRectF transRect = transformed.rect();

  QPointF newOffset = offset + (rect.center() - transRect.center());

  //painter->drawPixmap(newOffset, transformed);
  
  ///////////DRAWING THE GAUGE//////////////////
painter->setRenderHint(QPainter::Antialiasing);
QRectF rectangle(offset.x(), offset.y(), rect.width(), rect.height());
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
    gradient.setColorAt(0.0,QColor(140,255,140));
    gradient.setColorAt((2.0/3.0),QColor(255,255,140));
    gradient.setColorAt(1.0,QColor(255,140,140));

	if (mScale >= 1.0)
	{
	gradient.setColorAt(0.0,QColor(140,255,140));
    gradient.setColorAt((2.0/3.0),QColor(140,255,140));
    gradient.setColorAt(1.0,QColor(140,255,140));
	}
	if (mScale <= 0.0)
	{
	gradient.setColorAt(0.0,QColor(255,140,140));
    gradient.setColorAt((2.0/3.0),QColor(255,140,140));
    gradient.setColorAt(1.0,QColor(255,140,140));
	}

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
    painter->fillPath(pointer,QColor(140,64,140));
//////////////////

  QPen pen(Qt::black);
  if (mChange == -1.0)
  pen.setColor(Qt::red);

  if (mChange == 1.0)
	  pen.setColor(Qt::darkGreen);

  painter->setPen(pen);
  QFont font = painter->font() ;
  font.setPointSizeF(rect.height()/4.5);
  painter->setFont(font);
    painter->drawText(offset.x(),offset.y()+rect.height()+(rect.height()/8.0),QString::number(mValue));
   painter->restore();
}
