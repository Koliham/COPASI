// Copyright (C) 2013 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#ifndef QBAR_EFFECT_H
#define QBAR_EFFECT_H

#include <QtGui/QGraphicsEffect>

class QPainter;

class CQBarEffect : public QGraphicsEffect
{
  Q_OBJECT
public:
  CQBarEffect();
  virtual ~CQBarEffect();
  void setScale(qreal scale);
  void setValue(qreal wert);
  void setChange(qreal change);

protected:
  void draw(QPainter* painter);

  qreal mScale;
  qreal mValue;
  qreal mChange;
};

#endif // QBAR_EFFECT_H
