#include "stdafx.h"
#include "mindmapscene.h"
#include "mindprogressnode.h"
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QMenu>
#include <QToolTip>


MindProgressNode::MindProgressNode(const QString& text, MindNode* parent)
	: MindNode(text, parent)
	, m_progress(0.0)
	, m_workinghours(0.0)
	, m_tipItem(NULL)
{
	m_tipItem = new QGraphicsPixmapItem(this);
}

MindProgressNode::~MindProgressNode()
{
	if (m_tipItem)
		delete m_tipItem;
}

void MindProgressNode::setup(MindMapScene* pScene)
{
	MindNode::setup(pScene);
	updateNodeColor();
	if (hasNoChildren() && Parent())
	{
		updateToParent();
	}
	QGraphicsTextItem::setProgress(m_progress);
	updateToolTip();
	updateTipIcon();
}

void MindProgressNode::initUIColor()
{
	QColor selectedBorder = QColor(23, 157, 235);
	QColor mainThemeColor = QColor(0, 181, 72);
	QColor backgroundColor;
	QColor highlightBorder;
	if (hasNoChildren())
	{
		backgroundColor = QColor(255, 255, 255);
		highlightBorder = QColor(23, 157, 235);
	}
	else
	{
		backgroundColor = QColor(242, 242, 242);
		highlightBorder = QColor(136, 203, 242);
	}
	QColor borderFocusout = QColor(backgroundColor);
	setColors(mainThemeColor, backgroundColor, selectedBorder, highlightBorder, borderFocusout);
}

void MindProgressNode::updateNodeColor()
{
	setTextColor(QColor(0, 0, 0));
	if (hasNoChildren() && !isTopRoot())
	{
		QGraphicsTextItem::setProgressColor(QColor(255, 255, 255), QColor(255, 255, 255));
	}
	else
	{
		QGraphicsTextItem::setProgressColor(QColor(242, 242, 242), QColor(0, 181, 72));
	}
}

void MindProgressNode::updateMenuItem()
{
	if (getMenu())
	{
		getMenu()->clear();
		initMenu();
	}
}

void MindProgressNode::initMenu()
{
	MindNode::initMenu();
	if (hasNoChildren())
	{
		getMenu()->addAction(QString(u8"设置工作时间"), this, SLOT(setWorkingHourDlg()));
		getMenu()->addAction(QString(u8"标记完成"), this, SLOT(markFinish()));
		getMenu()->addAction(QString(u8"清空进度"), this, SLOT(zeroSchedule()));
	}
}

void MindProgressNode::resetDecoration()
{
	if (needShowDecoration())
	{
		setDecoration(2, QIcon(":/icons/16x16/link_note_black.png"));
	}
}

bool MindProgressNode::sceneEvent(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::GraphicsSceneHoverEnter:
		updateToolTip();
		break;
	}
	return MindNode::sceneEvent(event);
}

void MindProgressNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	MindNode::paint(painter, option, widget);
}

void MindProgressNode::updateToolTip()
{
	QString toolTipText;
	if (m_workinghours <= 0)
	{
		toolTipText = QString(u8"当前进度或者子进度还没设置工时。");
	}
	else
	{
		toolTipText = QString(u8"总工时为%1个小时，当前进度为%2%。").arg(QString::number(m_workinghours)).arg(QString::number(m_progress * 100));
	}
	setToolTip(toolTipText);
}

float MindProgressNode::progress()
{
	return m_progress;
}

void MindProgressNode::setProgress(float progress)
{
	m_progress = progress;
	updateTipIcon();
}

void MindProgressNode::_setProgress(float progress)
{
	if (m_workinghours == 0)
	{
		progress = 0;
	}

	setProgress(progress);

	QGraphicsTextItem::setProgress(m_progress);
	updateNodeColor();
	update();

	emit dataChanged(false);
}

void MindProgressNode::setPosition(QPointF pos)
{
	MindNode::setPosition(pos);
	adjustIndicator();
}

void MindProgressNode::onDeleteNode()
{
	MindNode::onDeleteNode();
	MindProgressNode* progressNode = qobject_cast<MindProgressNode*>(Parent());
	if (progressNode)
	{
		progressNode->updateStatus();
	}
}

void MindProgressNode::updateTipIcon()
{
	if (m_tipItem)
	{
		if (m_workinghours == 0)
		{
			QIcon icon(":/icons/warning.png");
			m_tipItem->setPixmap(icon.pixmap(16, 16));
			m_tipItem->show();
		}
		else if (m_progress == 1 && hasNoChildren())
		{
			QIcon icon(":/icons/checked.png");
			m_tipItem->setPixmap(icon.pixmap(16, 16));
			m_tipItem->show();
		}
		else
		{
			m_tipItem->hide();
		}
	}
}

void MindProgressNode::adjustIndicator()
{
	if (m_tipItem)
	{
		m_tipItem->setZValue(zValue() + 1);
		m_tipItem->setPos(boundingRect().bottomRight() - QPointF(12, 12));
	}
}

float MindProgressNode::workHours()
{
	return m_workinghours;
}

void MindProgressNode::setWorkhours(float hours)
{
	m_workinghours = hours;
	updateTipIcon();
}

void MindProgressNode::_setWorkhours(float hours)
{
	if (hasNoChildren())
	{
		_setProgress(0.0);
	}
	setWorkhours(hours);
	emit dataChanged(false);
}

void MindProgressNode::updateStatus()
{
	updateNodeColor();	//新增节点或删除节点需要重新调整颜色。
	updateMenuItem();

	float totalHours = 0.0;
	float totalFinished = 0.0;
	QList<MindNode*> children = Children();
	for (auto it = children.begin(); it != children.end(); it++)
	{
		MindProgressNode* pNode = qobject_cast<MindProgressNode*>((*it));
		Q_ASSERT(pNode);
		float hours = pNode->workHours();
		float progress = pNode->progress();
		if (hours == 0)
		{
			//conflict
			_setWorkhours(0);
			_setProgress(0);
			updateToParent();
			return;
		}
		totalHours += hours;
		totalFinished += hours * progress;
	}
	_setWorkhours(totalHours);
	_setProgress(totalFinished / totalHours);

	updateToParent();
}

void MindProgressNode::updateToParent()
{
	if (!Parent())
		return;
	MindProgressNode* parent = qobject_cast<MindProgressNode*>(Parent());
	Q_ASSERT(parent);
	parent->updateStatus();
}

void MindProgressNode::setWorkingHourDlg()
{
	QGraphicsView* pView = scene()->views()[0];
	QString text = QInputDialog::getText(pView,
		u8"输入该进度的工作时间", u8"设置工作时间（单位：小时）",
		QLineEdit::Normal, QString::number(m_workinghours));
	if (!text.isEmpty())
	{
		_setWorkhours(text.toFloat());
		updateToParent();
	}
}

void MindProgressNode::markFinish()
{
	_setProgress(1.0);
	updateToParent();
}

void MindProgressNode::zeroSchedule()
{
	_setProgress(0.0);
	updateToParent();
}
