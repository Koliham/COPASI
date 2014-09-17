// Copyright (C) 2013 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#ifndef QGAUGE_EFFECT_H
#define QGAUGE_EFFECT_H

#include <QtGui/QGraphicsEffect>

class QPainter;

class CQGaugeEffect : public QGraphicsEffect
{
  Q_OBJECT
public:
  CQGaugeEffect();
  virtual ~CQGaugeEffect();
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
