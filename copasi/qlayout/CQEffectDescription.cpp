// Copyright (C) 2013 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsEffect>

#include <qlayout/CQEffectDescription.h>
#include <qlayout/CQCopasiEffect.h>
#include <qlayout/CQBarEffect.h>
#include <qlayout\CQGaugeEffect.h>

qreal linear(qreal a, qreal b, qreal t)
{
  return a * (1 - t) + b * t;
}

// A function with f(start) = 0, f(end) = 1
qreal percentage(qreal start, qreal end, qreal value)
{
	return ((1/(end-start))*value)-(start/(end-start)); 
}

/**
 * Interpolate between a, and b
 *
 * 0.0 <= t <= 1.0
 */
QColor interpolate(const QColor &a, const QColor &b, float t)
{
  int ah, as, av, aa;
  a.getHsv(&ah, &as, &av, &aa);

  int bh, bs, bv, ba;
  b.getHsv(&bh, &bs, &bv, &ba);

  int rh, rs, rv, ra;
  rh = linear(ah, bh, t);
  rs = linear(as, bs, t);
  rv = linear(av, bv, t);
  ra = linear(aa, ba, t);

  return QColor::fromHsv(rh, rs, rv, ra);
}

CQEffectDescription::CQEffectDescription(const std::string& cn, Mode mode, const QColor &startColor, const QColor& endColor)
  : mCN(cn)
  , mStartColor(startColor)
  , mEndColor(endColor)
  , mScaleStart(0.5)
  , mScaleEnd(2.0)
  , mGaugeStart(0.0)
  , mGaugeEnd(0.0)
  , mMode(mode)
{
}

CQEffectDescription::CQEffectDescription(const std::string& cn, qreal startScale, qreal endScale)
  : mCN(cn)
  , mStartColor(Qt::white)
  , mEndColor(Qt::red)
  , mScaleStart(startScale)
  , mScaleEnd(endScale)
  , mGaugeStart(0.0)
  , mGaugeEnd(0.0)
  , mMode(Scale)
{
}

CQEffectDescription::~CQEffectDescription()
{
}

const std::string& CQEffectDescription::getCN()const
{
  return mCN;
}

const QColor& CQEffectDescription::getStartColor() const
{
  return mStartColor;
}

const QColor& CQEffectDescription::getEndColor() const
{
  return mEndColor;
}

qreal CQEffectDescription::getScaleStart() const
{
  return mScaleStart;
}

qreal CQEffectDescription::getScaleEnd() const
{
  return mScaleEnd;
}

qreal CQEffectDescription::getGaugeStart() const
{
  return mGaugeStart;
}

qreal CQEffectDescription::getGaugeEnd() const
{
  return mGaugeEnd;
}

CQEffectDescription::Mode CQEffectDescription::getMode() const
{
  return mMode;
}
void CQEffectDescription::setMode(CQEffectDescription::Mode mode)
{
  mMode = mode;
}

void CQEffectDescription::removeFromScene(CQLayoutScene& scene)
{
  QGraphicsItem *item = scene.getItemFor(mCN);

  if (item == NULL)
    return;

  QGraphicsEffect* effect = item->graphicsEffect();

  if (effect == NULL)
    return;

  effect->setEnabled(false);
}

void CQEffectDescription::applyToScene(CQLayoutScene& scene, qreal t, qreal conce, qreal change)
{
  QGraphicsItem *item = scene.getItemFor(mCN);

  if (item == NULL)
    return;

  switch (mMode)
    {
      default:
      case DropShadow:
      {
        QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(&scene);
        effect->setColor(interpolate(mStartColor, mEndColor, t));
        effect->setBlurRadius(25);
        effect->setEnabled(true);
        effect->setOffset(0);
        item->setGraphicsEffect(effect);
      }
      break;

      case Colorize:
      {
        QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect(&scene);
        effect->setColor(interpolate(mStartColor, mEndColor, t));
        effect->setStrength(1);
        effect->setEnabled(true);
        item->setGraphicsEffect(effect);
      }
      break;

      case Scale:
      {
        CQCopasiEffect* effect = new CQCopasiEffect();
        effect->setScale(linear(mScaleStart, mScaleEnd, t));
        effect->setEnabled(true);
        item->setGraphicsEffect(effect);
      }
      break;

      case Bar:
      {
        CQBarEffect* effect = new CQBarEffect();
        effect->setScale(t);
		if (conce != NULL)
			effect->setValue(conce);
		effect->setChange(change);
		effect->setEnabled(true);
        item->setGraphicsEffect(effect);
      }
      break;

      case AutoGauge:
      {
        CQBarEffect* effect = new CQBarEffect();
        effect->setScale(linear(mGaugeStart, mGaugeEnd, t));
		if (conce != NULL)
			effect->setValue(conce);
		effect->setChange(change);
		effect->setEnabled(true);
        item->setGraphicsEffect(effect);
      }
      break;

      case Gauge:
      {
        CQGaugeEffect* effect = new CQGaugeEffect();
		
		if (mGaugeEnd == 0.0)
		{
			effect->setScale(t);
		}
		else
		{
			effect->setScale(percentage(mGaugeStart,mGaugeEnd,conce));
		}
		
		if (conce != NULL)
			effect->setValue(conce);
		effect->setChange(change);
		effect->setEnabled(true);
        item->setGraphicsEffect(effect);
      }
      break;

    }
}

void CQEffectDescription::setCN(const std::string& cn)
{
  mCN = cn;
}

void CQEffectDescription::setStartColor(const QColor& color)
{
  mStartColor = color;
}

void CQEffectDescription::setEndColor(const QColor& color)
{
  mEndColor = color;
}

void CQEffectDescription::setScaleStart(qreal scale)
{
  mScaleStart = scale;
}

void CQEffectDescription::setScaleEnd(qreal scale)
{
  mScaleEnd = scale;
}

void CQEffectDescription::setGaugeStart(qreal scale)
{
  mGaugeStart = scale;
}

void CQEffectDescription::setGaugeEnd(qreal scale)
{
  mGaugeEnd = scale;
}