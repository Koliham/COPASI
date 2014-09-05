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
{
}

CQBarEffect::~CQBarEffect()
{
}

void CQBarEffect::setScale(qreal scale)
{
  mScale = scale;
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
  
  
  //QPointF rahmenol(offset.x(),offset.y());
  //QPointF rahmenur(offset.x()+41,offset.y()+21);
  QPointF rahmenol(offset.x(),offset.y());
  QPointF rahmenur(offset.x()+rect.width(),offset.y()+5+1);
  QPoint balkenol(offset.x()+1,offset.y()+1);
  QPoint balkenur(offset.x()+(rect.width()-1)*(mScale), offset.y()+5);
  QRectF ort(rahmenol,rahmenur);
  QRectF ladebalken(balkenol,balkenur);
  //dünner Rahmen

  painter->drawRect(ort);//statt Rahmen zeichne ich den verdammten Rect
  if (mScale > 0.0 && mScale <= 10.0)
  painter->fillRect(ladebalken,Qt::green);
  painter->drawText(offset.x()+rect.width(),offset.y()+20,QString::number(mValue));
  painter->restore();
}
