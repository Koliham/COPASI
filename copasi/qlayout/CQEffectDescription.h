// Copyright (C) 2013 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#ifndef QEFFECT_DESCRIPTION_H
#define QEFFECT_DESCRIPTION_H

#include <QtGui/QColor>
#include <string>

#include <qlayout/CQLayoutScene.h>
#include <qlayout/CQGaugeItem.h>

class CQEffectDescription
{
public:
  enum Mode
  {
    DropShadow,
    Colorize,
    Scale,
	Bar,
	Gauge,
	AutoGauge
  };

  enum Caption
  {
	  OnLegend,
	  OffLegend
  };

  CQEffectDescription(const std::string& cn, qreal startScale, qreal endScale);
  CQEffectDescription(const std::string& cn, Mode mode = Colorize, const QColor &startColor = Qt::white, const QColor& endColor = Qt::red);
  virtual ~CQEffectDescription();

  const std::string& getCN()const;
  void setCN(const std::string& cn);

  const QColor& getStartColor() const;
  void setStartColor(const QColor& color);

  const QColor& getEndColor() const;
  void setEndColor(const QColor& color);

  qreal getScaleStart() const;
  void setScaleStart(qreal scale);

  qreal getScaleEnd() const;
  void setScaleEnd(qreal scale);
  
  qreal getGaugeStart() const;
  void setGaugeStart(qreal scale);

  qreal getGaugeEnd() const;
  void setGaugeEnd(qreal scale);

  Mode getMode() const;
  void setMode(Mode mode);

  //for the Legends
  Caption getCaption() const;
  void setCaption(Caption caption);

  virtual void removeFromScene(CQLayoutScene& scene);

  void applyToScene(CQLayoutScene& scene, qreal t, qreal conce = 0.0, qreal change = 0.0, qreal leftborder = 0.0, qreal rightborder = 0.0);

protected:
  std::string mCN;
  QColor mStartColor;
  QColor mEndColor;
  qreal  mScaleStart;
  qreal mScaleEnd;
  qreal mGaugeStart;
  qreal mGaugeEnd;
  Mode mMode;
  Caption mCaption;
  CQGaugeItem* legend;
  bool shown;
};

#endif // QEFFECT_DESCRIPTION_H
