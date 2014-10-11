// Copyright (C) 2013 by Pedro Mendes, Virginia Tech Intellectual
// Properties, Inc., University of Heidelberg, and The University
// of Manchester.
// All rights reserved.

#include <QtGui/QGraphicsItem>
#include <QtGui/QGraphicsEffect>

#include <qlayout/CQEffectDescription.h>
#include <qlayout/CQCopasiEffect.h>
#include <qlayout/CQBarEffect.h>
#include <qlayout/CQGaugeEffect.h>
#include <qlayout/CQGaugeItem.h>


qreal linear(qreal a, qreal b, qreal t)
{
  return a * (1 - t) + b * t;
}

// A function with f(start) = 0, f(end) = 1
qreal percentage(qreal start, qreal end, qreal value)
{
	if (value <= 0.0 || value <= start)
		return 0.0;

	if (value >= end) //this lets the gauge be at maximum for nonzero and constant concentrations
		return 1.0;

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
  , mCaption(OffLegend)
  , shown(0)
  , legend(new CQGaugeItem())
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
  , mCaption(OffLegend)
  , shown(0)
  , legend(new CQGaugeItem())
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

//for the Legends

CQEffectDescription::Caption CQEffectDescription::getCaption() const
{
	return mCaption;
}

void CQEffectDescription::setCaption(CQEffectDescription::Caption caption)
{
  mCaption = caption;
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

void CQEffectDescription::applyToScene(CQLayoutScene& scene, qreal t, qreal conce, qreal change, qreal leftborder, qreal rightborder)
{
  QGraphicsItem *item = scene.getItemFor(mCN);

  if (item == NULL)
    return;

  //if AutoFit sets the borders for each Metabolite, then "rightborder" is not zero

  if (rightborder != 0.0)
  {
	  mGaugeStart = leftborder;
	  mGaugeEnd = rightborder;
  }
  QList<QGraphicsItem*> alltheitems;
  alltheitems = scene.items();
  if (alltheitems.contains(legend))
  {

  }


    if (mCaption == CQEffectDescription::OffLegend && alltheitems.contains(legend))
	  scene.removeItem(legend);

  if (mCaption == CQEffectDescription::OnLegend)
  {
	  if (!(alltheitems.contains(legend)))
	  {

	//add the item to the scene

			legend = new CQGaugeItem();
			if (mGaugeEnd == 0.0)
				legend->setValue(t);
			else
				legend->setValue(percentage(mGaugeStart,mGaugeEnd,conce));

			legend->setConcentration(conce);
			legend->setChange(change);
			//legend->update();
			scene.addItem(legend);
			//legend->update();
	  }
	  else
	  {

			if (mGaugeEnd == 0.0)
				legend->setValue(t);
			else
				legend->setValue(percentage(mGaugeStart,mGaugeEnd,conce));
		  legend->setConcentration(conce);
		  legend->setChange(change);
	  }
  }


	//legend->setValue(t);
	//legend->update();

  switch (mMode)
    {
      default:
      case DropShadow:
      {
        QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(&scene);

		if (mGaugeEnd == 0.0)
			effect->setColor(interpolate(mStartColor, mEndColor, t));
		else
			effect->setColor(interpolate(mStartColor, mEndColor, percentage(mGaugeStart,mGaugeEnd,conce)));
		
		effect->setBlurRadius(25);
        effect->setEnabled(true);
        effect->setOffset(0);
        item->setGraphicsEffect(effect);
      }
      break;

      case Colorize:
      {
        QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect(&scene);

		if (mGaugeEnd == 0.0)
			effect->setColor(interpolate(mStartColor, mEndColor, t));
		else
			effect->setColor(interpolate(mStartColor, mEndColor, percentage(mGaugeStart,mGaugeEnd,conce)));
		
		effect->setStrength(1);
        effect->setEnabled(true);
        item->setGraphicsEffect(effect);
      }
      break;

      case Scale:
      {
        CQCopasiEffect* effect = new CQCopasiEffect();
		if (mGaugeEnd == 0.0)
		{
			effect->setScale(linear(mScaleStart, mScaleEnd, t));
		}
		else
		{
			effect->setScale(linear(mScaleStart, mScaleEnd, percentage(mGaugeStart,mGaugeEnd,conce)));
		}

        effect->setEnabled(true);
        item->setGraphicsEffect(effect);
      }
      break;

      case Bar:
      {
        CQBarEffect* effect = new CQBarEffect();
		if (mGaugeEnd == 0.0)
			effect->setScale(t);
		else
			effect->setScale(percentage(mGaugeStart,mGaugeEnd,conce));


		if (conce != NULL) // if (conce != NULL)
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
			//effect->setValue(conce); //old one
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