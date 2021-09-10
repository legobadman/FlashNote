#include "stdafx.h"
#include "screenwidget.h"
#include "MyStyle.h"
#include <QWindow>
#include <QScreen>
#include <QGraphicsSceneEvent>
#include <QFileDialog>
#include <QStandardPaths>
#include <QImageWriter>
#include <QMessageBox>
#include <QClipboard>


GrabDraggingRect::GrabDraggingRect(const QRectF& rect, QGraphicsItem* parent)
	: QGraphicsRectItem(rect, parent)
{
}

QRectF GrabDraggingRect::boundingRect() const
{
	return this->rect();
	QRectF br = QGraphicsRectItem::boundingRect();
	return br;
}


ScreenGrabRect::ScreenGrabRect(const QPixmap& original, const QRectF& rect, QGraphicsItem* parent)
	: _base(parent)
	, m_originalShot(original)
	, m_rect(rect)
	, m_transform(MOUSE_DONOTHING)
	, m_choosingRect(true)
{
	maxW = m_originalShot.width();
	maxH = m_originalShot.height();
	qreal w = m_rect.width();
	qreal h = m_rect.height();
	QPixmap grabImage = QPixmap(QSize(w, h));
	QPainter p2(&grabImage);
	QPointF topLeft = m_rect.topLeft();
	QPixmap slice = m_originalShot.copy(topLeft.x(), topLeft.y(), w, h);
	p2.drawPixmap(0, 0, slice);
	setPixmap(grabImage);
	setPos(topLeft);

	m_borderItem = new GrabDraggingRect(QRectF(0, 0, w, h), this);
	m_borderItem->setPen(QPen(QColor(21, 152, 255), m_borderWidth + 1));
	m_borderItem->setBrush(Qt::NoBrush);
	m_borderItem->setAcceptHoverEvents(true);
	m_borderItem->installSceneEventFilter(this);

	m_cursor_mapper.insert(make_pair(SCALE_LEFT_TOP, Qt::SizeFDiagCursor));
	m_cursor_mapper.insert(make_pair(SCALE_LEFT_MID, Qt::SizeHorCursor));
	m_cursor_mapper.insert(make_pair(SCALE_LEFT_BOTTOM, Qt::SizeBDiagCursor));
	m_cursor_mapper.insert(make_pair(SCALE_MID_TOP, Qt::SizeVerCursor));
	m_cursor_mapper.insert(make_pair(SCALE_MID_BOTTOM, Qt::SizeVerCursor));
	m_cursor_mapper.insert(make_pair(SCALE_RIGHT_TOP, Qt::SizeBDiagCursor));
	m_cursor_mapper.insert(make_pair(SCALE_RIGHT_MID, Qt::SizeHorCursor));
	m_cursor_mapper.insert(make_pair(SCALE_RIGHT_BOTTOM, Qt::SizeFDiagCursor));
	m_cursor_mapper.insert(make_pair(TRANSLATE, Qt::SizeAllCursor));

	QVector<QPointF> pts = {
		QPointF(-3, -3),
		QPointF(-3, h / 2 - 3),
		QPointF(-3, h - 3),

		QPointF(w / 2 - 3, -3),
		QPointF(w / 2 - 3, h - 3),

		QPointF(w - 3, -3),	
		QPointF(w - 3, h / 2 - 3),
		QPointF(w - 3, h - 3)
	};
	m_dragPoints.resize(pts.size());

	for (int i = 0; i < pts.size(); i++)
	{
		//防止越过全屏boundingrect
		qreal xp = pts[i].x(), yp = pts[i].y();
		QPointF scenePt = mapToScene(pts[i]);
		if (scenePt.x() < 0)
			xp = 0;
		if (scenePt.y() < 0)
			yp = 0;
		QRectF rcPoint = QRectF(xp, yp, 6, 6);
		m_dragPoints[i] = new QGraphicsRectItem(rcPoint, this);
		m_dragPoints[i]->setPen(Qt::NoPen);
		m_dragPoints[i]->setBrush(QColor(21, 152, 255));
		m_dragPoints[i]->installSceneEventFilter(this);
		m_dragPoints[i]->setAcceptHoverEvents(true);
		m_dragPoints[i]->hide();
	}

	setFlags(ItemIsMovable);
}

void ScreenGrabRect::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (m_choosingRect)
	{
		//TODO：根据鼠标位置定位到当前可以形成的一个窗口区域。
		//要根据这个区域，实时更新pixmap区域。

		//当然用户也有可能想开始拖选，于是也得记下固定点。
		m_transform = SCALE_LEFT_TOP;
		m_movescale_info.fixed_point = event->scenePos();
		_base::mousePressEvent(event);
		emit grabStarted();
		return;
	}

	m_transform = getMouseEventType(event->pos());
	QRectF br = boundingRect();
	QPointF scenePos = this->scenePos();
	qreal W = br.width(), H = br.height();
	switch (m_transform)
	{
	case SCALE_RIGHT_BOTTOM:
		m_movescale_info.fixed_point = scenePos;
		break;
	case SCALE_RIGHT_TOP:
		m_movescale_info.fixed_point = QPointF(scenePos.x(), scenePos.y() + H - 1);
		break;
	case SCALE_LEFT_TOP:
		m_movescale_info.fixed_point = QPointF(scenePos.x() + W - 1, scenePos.y() + H - 1);
		break;
	case SCALE_LEFT_BOTTOM:
		m_movescale_info.fixed_point = QPointF(scenePos.x() + W - 1, scenePos.y());
		break;

	case SCALE_MID_TOP:
		m_movescale_info.fixed_x = scenePos.x();
		m_movescale_info.fixed_y = scenePos.y() + H - 1;
		break;

	case SCALE_MID_BOTTOM:
		m_movescale_info.fixed_x = scenePos.x();
		m_movescale_info.fixed_y = scenePos.y();
		break;

	case SCALE_LEFT_MID:
		m_movescale_info.fixed_y = scenePos.y();
		m_movescale_info.fixed_x = scenePos.x() + W - 1;
		break;
	case SCALE_RIGHT_MID:
		m_movescale_info.fixed_y = scenePos.y();
		m_movescale_info.fixed_x = scenePos.x();
		break;

	case TRANSLATE:
		m_movescale_info.old_width = W;
		m_movescale_info.old_height = H;
		break;
	}

	m_movescale_info.old_height = H;
	m_movescale_info.old_width = W;
	_base::mousePressEvent(event);
	emit grabStarted();
}

void ScreenGrabRect::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	m_currentShot = pixmap();
	if (m_transform != OUTSIDE)
	{
		emit grabFinish();
	}
	m_choosingRect = false;
	m_transform = OUTSIDE;
	_base::mouseReleaseEvent(event);
}

void ScreenGrabRect::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	QPointF pt = event->pos();
	if (m_transform == TRANSLATE)
	{
		_base::mouseMoveEvent(event);
		qreal w = m_movescale_info.old_width;
		qreal h = m_movescale_info.old_height;
		QPixmap grabImage = QPixmap(QSize(w, h));
		QPainter p2(&grabImage);
		QPointF topLeft = this->scenePos();
		qreal px = topLeft.x(), py = topLeft.y();
		if (px < 0 || py < 0 || px + w  > maxW || py + h > maxH)
		{
			px = min(max(0., px), maxW - w);
			py = min(max(0., py), maxH - h);
			//修正位置。
			setPos(px, py);
		}
		QPixmap slice = m_originalShot.copy(px, py, w, h);
		p2.drawPixmap(0, 0, slice);
		setPixmap(grabImage);
		update();
	}
	else if (m_transform == OUTSIDE)
	{
		return;
	}
	else
	{
		QPointF scenePos = event->scenePos();
		qreal newWidth = 0, newHeight = 0;
		QPointF newTopLeft;

		switch (m_transform)
		{
			case SCALE_LEFT_TOP:
			case SCALE_RIGHT_TOP:
			case SCALE_LEFT_BOTTOM:
			case SCALE_RIGHT_BOTTOM:
			{
				//fixed_bottomright
				qreal left = min(scenePos.x(), m_movescale_info.fixed_point.x());
				qreal right = max(scenePos.x(), m_movescale_info.fixed_point.x());
				qreal top = min(scenePos.y(), m_movescale_info.fixed_point.y());
				qreal bottom = max(scenePos.y(), m_movescale_info.fixed_point.y());
				newWidth = right - left + 1;
				newHeight = bottom - top + 1;
				newTopLeft = QPointF(left, top);
				break;
			}
			case SCALE_LEFT_MID:
			case SCALE_RIGHT_MID:
			{
				qreal left = min(scenePos.x(), m_movescale_info.fixed_x);
				qreal right = max(scenePos.x(), m_movescale_info.fixed_x);
				qreal top = m_movescale_info.fixed_y;
				newWidth = right - left + 1;
				newHeight = m_movescale_info.old_height;
				newTopLeft = QPointF(left, top);
				break;
			}
			case SCALE_MID_TOP:
			case SCALE_MID_BOTTOM:
			{
				qreal left = m_movescale_info.fixed_x;
				qreal top = min(scenePos.y(), m_movescale_info.fixed_y);
				qreal bottom = max(scenePos.y(), m_movescale_info.fixed_y);
				newWidth = m_movescale_info.old_width;
				newHeight = bottom - top + 1;
				newTopLeft = QPointF(left, top);
				break;
			}
		}

		QPixmap grabImage = QPixmap(QSize(newWidth, newHeight));
		QPainter p2(&grabImage);
		QPixmap slice = m_originalShot.copy(newTopLeft.x(), newTopLeft.y(), newWidth, newHeight);
		p2.drawPixmap(0, 0, slice);
		setPixmap(grabImage);
		setPos(newTopLeft);

		if (m_borderItem)
		{
			m_borderItem->setPen(QPen(QColor(21, 152, 255), m_borderWidth));
			m_borderItem->setRect(QRectF(0, 0, newWidth, newHeight));
		}

		QVector<QPointF> pts = {
			QPointF(-3, -3),
			QPointF(-3, newHeight / 2 - 3),
			QPointF(-3, newHeight - 3),

			QPointF(newWidth / 2 - 3, -3),
			QPointF(newWidth / 2 - 3, newHeight - 3),

			QPointF(newWidth - 3, -3),
			QPointF(newWidth - 3, newHeight / 2 - 3),
			QPointF(newWidth - 3, newHeight - 3)
		};
		for (int i = 0; i < m_dragPoints.size(); i++)
		{
			qreal xp = pts[i].x(), yp = pts[i].y();
			QPointF scenePt = mapToScene(pts[i]);
			if (scenePt.x() < 0)
				xp = 0;
			if (scenePt.y() < 0)
				yp = 0;
			QRectF rcPoint = QRectF(xp, yp, 6, 6);
			m_dragPoints[i]->setRect(rcPoint);
			m_dragPoints[i]->setVisible(true);
			m_dragPoints[i]->installSceneEventFilter(this);
			m_dragPoints[i]->setAcceptHoverEvents(true);
		}
		update();
	}
}

ScreenGrabRect::MOUSE_TRANSFORM ScreenGrabRect::getMouseEventType(QPointF pressPoint)
{
	qreal xp = pressPoint.x(), yp = pressPoint.y();
	qreal h = boundingRect().height(), w = boundingRect().width();
	static int offset = 5;		//为了更容易捕获鼠标而设的容错偏移
	if (xp < -3 - offset)
	{
		return OUTSIDE;
	}
	else if (xp >= -3 - offset && xp <= 6 + offset)
	{
		if (yp < -3 - offset)
		{
			return OUTSIDE;
		}
		else if (yp >= -3 - offset && yp <= 6 + offset)
		{
			return SCALE_LEFT_TOP;
		}
		else if (yp >= h / 2 - 3 - offset && yp <= h / 2 + 3 + offset)
		{
			return SCALE_LEFT_MID;
		}
		else if (yp >= h - 3 - offset && yp <= h + 3 + offset)
		{
			return SCALE_LEFT_BOTTOM;
		}
		else if (yp > h + 3 + offset)
		{
			return OUTSIDE;
		}
		else
		{
			return TRANSLATE;
		}
	}
	else if (xp >= w / 2 - 3 - offset && xp <= w / 2 + 3 + offset)
	{
		if (yp < -3 - offset)
		{
			return OUTSIDE;
		}
		else if (yp >= -3 - offset && yp <= 6 + offset)
		{
			return SCALE_MID_TOP;
		}
		else if (yp >= h - 3 - offset && yp <= h + 3 + offset)
		{
			return SCALE_MID_BOTTOM;
		}
		else if (yp > h + 3 + offset)
		{
			return OUTSIDE;
		}
		else
		{
			return TRANSLATE;
		}
	}
	else if (xp >= w - 3 - offset && xp <= w + 3 + offset)
	{
		if (yp < -3 - offset)
		{
			return OUTSIDE;
		}
		else if (yp >= -3 - offset && yp <= 6 + offset)
		{
			return SCALE_RIGHT_TOP;
		}
		else if (yp >= h / 2 - 3 - offset && yp <= h / 2 + 3 + offset)
		{
			return SCALE_RIGHT_MID;
		}
		else if (yp >= h - 3 - offset && yp <= h + 3 + offset)
		{
			return SCALE_RIGHT_BOTTOM;
		}
		else if (yp > h + 3 + offset)
		{
			return OUTSIDE;
		}
		else
		{
			return TRANSLATE;
		}
	}
	else if (xp > w + 3 + offset)
	{
		return OUTSIDE;
	}
	else
	{
		return TRANSLATE;
	}
}

QRectF ScreenGrabRect::boundingRect() const
{
	return _base::boundingRect();
}

QRectF ScreenGrabRect::getCurrentShot(QPixmap& retImage)
{
	retImage = m_currentShot;
	return mapRectToScene(boundingRect());
}

bool ScreenGrabRect::sceneEventFilter(QGraphicsItem* watched, QEvent* event)
{
	if (event->type() == QEvent::GraphicsSceneHoverEnter || event->type() == QEvent::GraphicsSceneHoverMove)
	{
		QGraphicsSceneHoverEvent* e = static_cast<QGraphicsSceneHoverEvent*>(event);
		if (!m_choosingRect)
		{
			MOUSE_TRANSFORM mouse_type = getMouseEventType(e->pos());
			setCursor(m_cursor_mapper[mouse_type]);
		}
	}
	else if (event->type() == QEvent::GraphicsSceneHoverLeave)
	{
		this->setCursor(Qt::ArrowCursor);
	}
	return _base::sceneEventFilter(watched, event);
}

void ScreenGrabRect::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QStyleOptionGraphicsItem myOption(*option);
	myOption.state &= ~QStyle::State_Selected;
	_base::paint(painter, &myOption, widget);
}


/////////////////////////////////////////////////
ScreenShotWidget::ScreenShotWidget(QWidget* parent)
	: _base(parent)
	, m_toolbar(NULL)
{
	m_scene = new QGraphicsScene;
	m_background = new QGraphicsPixmapItem;
	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
	
	setScene(m_scene);
	m_scene->addItem(m_background);
	setFrameShape(QFrame::NoFrame);
	setFrameStyle(QFrame::NoFrame);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_toolbar = new ScreenToolBar(this);
	m_toolbar->hide();
	grab();
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
	QSize sz = m_originalShot.size();
	QPixmap backgroundShot(sz);
	int alpha = 125;
	QPainter p(&backgroundShot);
	p.setCompositionMode(QPainter::CompositionMode_Source);
	p.drawPixmap(0, 0, m_originalShot);
	p.setCompositionMode(QPainter::CompositionMode_DestinationIn);
	p.fillRect(backgroundShot.rect(), QColor(0, 0, 0, alpha));
	m_background->setPixmap(backgroundShot);

	int offset = 10;
	QRectF rc(offset, offset, sz.width() - offset * 2, sz.height() - offset * 2);
	m_grabber = new ScreenGrabRect(m_originalShot, rc, m_background);
	m_grabber->installEventFilter(this);

	connect(m_grabber, SIGNAL(grabStarted()), this, SLOT(onGrabStarted()));
	connect(m_grabber, SIGNAL(grabFinish()), this, SLOT(onGrabFinish()));
	connect(m_toolbar, SIGNAL(closeTriggered()), this, SLOT(close()));
	connect(m_toolbar, SIGNAL(finishTriggered()), this, SLOT(onShotFinished()));
	connect(m_toolbar, SIGNAL(saveTriggered()), this, SLOT(onSaveShot()));
}

void ScreenShotWidget::onGrabFinish()
{
	static const int sMarginToBar = MyStyle::dpiScaled(5);
	QPixmap retImage;
	QRectF rc = getGrabImage(retImage);
	QSize sz = m_toolbar->sizeHint();
	//TODO:如果位于最下方
	qreal top = rc.bottom() + sMarginToBar;
	qreal left = rc.right() - sz.width();
	m_toolbar->setGeometry(QRect(left, top, sz.width(), sz.height()));
	m_toolbar->show();
}

void ScreenShotWidget::onGrabStarted()
{
	m_toolbar->hide();
}

void ScreenShotWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape)
	{
		this->close();
	}
	_base::keyPressEvent(event);
}

void ScreenShotWidget::paintEvent(QPaintEvent* event)
{
	_base::paintEvent(event);
}

QRectF ScreenShotWidget::getGrabImage(QPixmap& retImage)
{
	if (m_grabber)
	{
		return m_grabber->getCurrentShot(retImage);
	}
	return QRectF();
}

void ScreenShotWidget::onShotFinished()
{
	QPixmap retImage;
	m_grabber->getCurrentShot(retImage);
	
	if (!retImage.isNull())
	{
		QClipboard* pClipBoard = QApplication::clipboard();
		pClipBoard->setPixmap(retImage);
	}
	close();
}

void ScreenShotWidget::onSaveShot()
{
	QPixmap retImage;
	m_grabber->getCurrentShot(retImage);

	const QString format = "png";
	QString initialPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
	if (initialPath.isEmpty())
		initialPath = QDir::currentPath();
	initialPath += tr("/untitled.") + format;

	QFileDialog fileDialog(this, tr("Save As"), initialPath);
	fileDialog.setAcceptMode(QFileDialog::AcceptSave);
	fileDialog.setFileMode(QFileDialog::AnyFile);
	fileDialog.setDirectory(initialPath);
	QStringList mimeTypes;
	const QList<QByteArray> baMimeTypes = QImageWriter::supportedMimeTypes();
	for (const QByteArray& bf : baMimeTypes)
		mimeTypes.append(QLatin1String(bf));
	fileDialog.setMimeTypeFilters(mimeTypes);
	fileDialog.selectMimeTypeFilter("image/" + format);
	fileDialog.setDefaultSuffix(format);
	if (fileDialog.exec() != QDialog::Accepted)
		return;
	const QString fileName = fileDialog.selectedFiles().first();
	if (!retImage.save(fileName)) {
		QMessageBox::warning(this, tr("Save Error"), tr("The image could not be saved to \"%1\".")
			.arg(QDir::toNativeSeparators(fileName)));
	}
	close();
}