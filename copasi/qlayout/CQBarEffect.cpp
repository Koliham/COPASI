// Copyright (C) 2013 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#include <qlayout/CQBarEffect.h>

#include <QtGui/QPainter>
#include <QtGui/QGraphicsEffect>

CQBarEffect::CQBarEffect()
  : mScale(1.0)
  , mValue(NULL)
  , mChange(0.0)
{
}

CQBarEffect::~CQBarEffect()
{
}

void CQBarEffect::setScale(qreal scale)
{
  mScale = scale;
}

void CQBarEffect::setChange(qreal change)
{
  mChange = change;
}

void CQBarEffect::setValue(qreal wert)
{
  mValue = wert;
}


void CQBarEffect::draw(QPainter* painter)
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

  painter->drawPixmap(newOffset, transformed);
  
  
  QPointF rahmenol(offset.x(),offset.y());
  //QPointF rahmenur(offset.x()+rect.width()-1,offset.y()+5+1);
  QPointF rahmenur(offset.x()+rect.width()-1,offset.y()+(rect.height()/10.0)+1);
  QPoint balkenol(offset.x()+1,offset.y()+1);
  //QPoint balkenur(offset.x()+(rect.width()-2)*(mScale), offset.y()+5);
  QPoint balkenur(offset.x()+(rect.width()-2)*(mScale), offset.y()+(rect.height()/10.0));
  QRectF ort(rahmenol,rahmenur);
  QRectF ladebalken(balkenol,balkenur);
  //dünner Rahmen

  painter->drawRect(ort);//statt Rahmen zeichne ich den verdammten Rect
  if (mScale > 0.0 && mScale <= 10.0)
  painter->fillRect(ladebalken,Qt::green);
  
  QPen pen(Qt::black);
  if (mChange == -1.0)
  pen.setColor(Qt::red);

  if (mChange == 1.0)
	  pen.setColor(Qt::darkGreen);

  painter->setPen(pen);
  QFont font = painter->font() ;
  font.setPointSizeF(rect.height()/4.5);
  painter->setFont(font);
  painter->drawText(offset.x()+(rect.width()/11.0),offset.y()+rect.height()-(rect.height()/9.0),QString::number(mValue));
  painter->restore();
}
