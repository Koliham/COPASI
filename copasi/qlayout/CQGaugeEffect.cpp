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
  : mScale(1.0)
  , mValue(NULL)
  , mChange(0.0)
{
}

CQGaugeEffect::~CQGaugeEffect()
{
}

void CQGaugeEffect::setScale(qreal scale)
{
  mScale = scale;
}

void CQGaugeEffect::setChange(qreal change)
{
  mChange = change;
}

void CQGaugeEffect::setValue(qreal wert)
{
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

for (int i = 0; i <= 8; i=i+1)
    {
    QPainterPath path;
    QPointF outerpoint(midpoint.x()+(rectangle.width()*0.5)*cos(M_PI*((5.0/6.0)+i*(1.0/6.0))),midpoint.y()+(rectangle.width()*0.5)*sin(M_PI*((5.0/6.0)+i*(1.0/6.0))));
    QPointF innerpoint(midpoint.x()+(rectangle.width()*0.5)*cos(M_PI*(1.0+i*(1.0/6.0))),midpoint.y()+(rectangle.width()*0.5)*sin(M_PI*(1.0+i*(1.0/6.0))));
    QPointF strich1(midpoint.x()+(rectangle.width()*0.49)*cos(M_PI*((5.0/6.0)+i*(1.0/6.0))),midpoint.y()+(rectangle.width()*0.49)*sin(M_PI*((5.0/6.0)+i*(1.0/6.0))));
    QPointF strich2(midpoint.x()+(rectangle.width()*0.4)*cos(M_PI*((5.0/6.0)+i*(1.0/6.0))),midpoint.y()+(rectangle.width()*0.4)*sin(M_PI*((5.0/6.0)+i*(1.0/6.0))));

    startAngle = (210.0-i*30.0) ;
    path.moveTo(outerpoint);
    path.arcTo(rectangle,startAngle,-31.0);
    path.arcTo(innerrect,startAngle-31.0,31.0);
    path.closeSubpath();
    QConicalGradient gradient(midpoint,220);
    gradient.setColorAt(0.0,Qt::green);
    gradient.setColorAt((2.0/3.0),Qt::yellow);
    gradient.setColorAt(1.0,Qt::red);


	
   if (i != 8)
    painter->fillPath(path,gradient);
   //optional, small black lines, if you want
   /*
   QPen pen(Qt::black);
   pen.setWidthF(rectangle.width()/100.0);



   if (i ==0 || i == 8)
        {
       pen.setWidthF(pen.widthF()+1.0);
    strich2.setX(midpoint.x()+(rectangle.width()*0.3)*cos(M_PI*((5.0/6.0)+i*(1.0/6.0))));
    strich2.setY(midpoint.y()+(rectangle.width()*0.3)*sin(M_PI*((5.0/6.0)+i*(1.0/6.0))));
   }

   if(i==4)
        {
       pen.setWidthF(pen.widthF()+1.0);
        strich2.setY(midpoint.y()+(rectangle.width()*0.35)*sin(M_PI*((5.0/6.0)+i*(1.0/6.0))));
        }
   painter->setPen(pen);
   painter->drawLine(strich1,strich2);*/
    }

  ////////////DRAWING THE GAUGE/////////////////

  
  
  double k = M_PI*2;

  //It is also possible to take a picture
//QPixmap bild("D:/Qt/Projekte/painter1/gauge1.png");
//painter->drawPixmap(offset.x(),offset.y(),rect.width(),rect.height(),bild);



    QPointF basis(offset.x()+(rect.width()/2.0),offset.y()+(rect.height()/2.0));
    QPointF boden1(basis.x()+(rect.width()/12)*cos(k*0.8+(mScale*k*0.75)),basis.y()+(rect.width()/12)*sin(k*0.8+(mScale*k*0.75)));
    QPointF boden2(basis.x()+(rect.width()/12)*cos(k*0.95+(mScale*k*0.75)),basis.y()+(rect.width()/12)*sin(k*0.95+(mScale*k*0.75)));

    qreal laenge = rect.width()*0.4;
    QPoint spitze(basis.x()+cos((k*0.375)+(mScale*k*0.75))*laenge,basis.y()+sin((k*0.375)+(mScale*k*0.75))*laenge);


QPainterPath path;
path.moveTo(boden1);
path.lineTo(boden2);
path.lineTo(spitze);
path.lineTo(boden1);
//painter->fillPath(path,QColor(255,127,0));
painter->fillPath(path,QColor(255,0,255));
//////////////////

  QPen pen(Qt::black);
  if (mChange == -1.0)
  pen.setColor(Qt::red);

  if (mChange == 1.0)
	  pen.setColor(Qt::darkGreen);

  painter->setPen(pen);
  QFont font = painter->font() ;
  font.setPointSizeF(rect.width()/4.5);
  painter->setFont(font);
    painter->drawText(offset.x(),offset.y()+rect.height()+(rect.height()/8.0),QString::number(mValue));
   painter->restore();
}
