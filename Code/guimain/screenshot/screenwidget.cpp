#include "stdafx.h"
#include "screenwidget.h"
#include <QWindow>
#include <QScreen>


ScreenGrabRect::ScreenGrabRect(const QPixmap& original, const QRectF& rect, QGraphicsItem* parent)
	: _base(parent)
	, m_originalShot(original)
	, m_rect(rect)
{
	int w = m_rect.width();
	int h = m_rect.height();
	QPixmap grabImage = QPixmap(QSize(w, h));
	QPainter p2(&grabImage);
	QPointF topLeft = m_rect.topLeft();
	QPixmap slice = m_originalShot.copy(topLeft.x(), topLeft.y(), w, h);
	int alpha = 125;
	p2.drawPixmap(0, 0, slice);
	setPixmap(grabImage);
	setPos(topLeft);

	m_borderItem = new QGraphicsRectItem(QRectF(0, 0, w, h), this);
	m_borderItem->setPen(QPen(QColor(21, 152, 255), m_borderWidth));
	m_borderItem->setBrush(Qt::NoBrush);

	//9¸öÀ­³¶µã
	m_dragHolderItem = new QGraphicsRectItem(QRectF(-3, -3, 6, 6), this);
	m_dragHolderItem->setPen(Qt::NoPen);
	m_dragHolderItem->setBrush(QColor(21, 152, 255));

}

QRectF ScreenGrabRect::boundingRect() const
{
	return _base::boundingRect();
}

void ScreenGrabRect::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	_base::paint(painter, option, widget);
}


/////////////////////////////////////////////////
ScreenShotWidget::ScreenShotWidget(QWidget* parent)
	: _base(parent)
{
	m_scene = new QGraphicsScene;
	m_background = new QGraphicsPixmapItem;
	this->setScene(m_scene);
	m_scene->addItem(m_background);
	this->setFrameShape(QFrame::NoFrame);
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	this->grab();
}

ScreenShotWidget::~ScreenShotWidget()
{
}

void ScreenShotWidget::grab()
{
	QScreen* screen = QGuiApplication::primaryScreen();
	if (const QWindow* window = windowHandle())
		screen = window->screen();
	if (!screen)
		return;

	m_originalShot = screen->grabWindow(0);

	QPixmap backgroundShot(m_originalShot.size());
	int alpha = 125;
	QPainter p(&backgroundShot);
	p.setCompositionMode(QPainter::CompositionMode_Source);
	p.drawPixmap(0, 0, m_originalShot);
	p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
	p.fillRect(backgroundShot.rect(), QColor(0, 0, 0, alpha));
	m_background->setPixmap(backgroundShot);

	QPoint topLeft(500, 250);
	QPoint bottomRight(1000, 550);
	QRectF rc(topLeft, bottomRight);
	m_grabber = new ScreenGrabRect(m_originalShot, rc, nullptr);
	m_scene->addItem(m_grabber);
}