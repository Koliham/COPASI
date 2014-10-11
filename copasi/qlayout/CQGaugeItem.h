#ifndef CQGAUGEITEM_H
#define CQGAUGEITEM_H

#include <QtGui/QPainter>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsItem>

class CQGaugeItem : public QObject, public QGraphicsItem
{
 //! needed if you want to use slots
 Q_OBJECT
 //! needed so you can still cast to graphicsitem
 Q_INTERFACES(QGraphicsItem)

 public:
   CQGaugeItem();
   QRectF boundingRect() const;
   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
   void setValue(qreal wert);

public slots:
   void dragLeaveEvent();
   void dragMoveEvent();

protected:
	qreal xpos, ypos, height, width, mScale;

};

#endif // CQGAUGEITEM_H
