// Copyright (C) 2013 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#include <qlayout/CQBarEffect.h>

#include <QtGui/QPainter>
#include <QtGui/QGraphicsEffect>

CQBarEffect::CQBarEffect()
  : mScale(1.0)
{
}

CQBarEffect::~CQBarEffect()
{
}

void CQBarEffect::setScale(qreal scale)
{
  mScale = scale;
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

  QTransform trans = QTransform::fromScale(mScale, mScale);
  QPixmap transformed = pixmap.transformed(trans, Qt::SmoothTransformation);
  QRectF transRect = transformed.rect();

  QPointF newOffset = offset + (rect.center() - transRect.center());

  //painter->drawPixmap(newOffset, transformed);
  
  
  QPoint koord(offset.x(),offset.y());
  QPoint ykoord(offset.x()+41,offset.y()+21);
  QPoint koordi(offset.x()+1,offset.y()+1);
  QPoint ykoordi(offset.x()+40*(mScale), offset.y()+20);
  QRectF ort(koord,ykoord);
  QRectF ladebalken(koordi,ykoordi);
  
  painter->drawRect(ort);
  if (mScale > 0.0 && mScale <= 1.0)
  painter->fillRect(ladebalken,Qt::green);
  painter->restore();
}
