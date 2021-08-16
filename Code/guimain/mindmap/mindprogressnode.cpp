#include "stdafx.h"
#include "mindmapscene.h"
#include "mindprogressnode.h"
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QMenu>
#include <QTooltip>


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
	if (m_children.isEmpty() && m_parent)
	{
		updateToParent();
	}
	QGraphicsTextItem::setProgress(m_progress);
	updateToolTip();
}

void MindProgressNode::initUIColor()
{
	m_selectedBorder = QColor(23, 157, 235);
	if (m_children.empty())
	{
		m_backgroudColor = QColor(255, 255, 255);
		m_highlightedBorder = QColor(23, 157, 235);
	}
	else
	{
		m_backgroudColor = QColor(242, 242, 242);
		m_highlightedBorder = QColor(136, 203, 242);
	}
	m_borderFocusout = QColor(m_backgroudColor);	//由于文本框的绘制策略，只能将同色的边框视为无边框。
}

void MindProgressNode::updateNodeColor()
{
	m_textColor = QColor(0, 0, 0);
	if (m_children.empty())
	{
		QGraphicsTextItem::setProgressColor(QColor(255, 255, 255), QColor(255, 255, 255));
	}
}

void MindProgressNode::initMenu()
{
	MindNode::initMenu();
	if (m_children.empty())
	{
		m_pMenu->addAction(QString(u8"设置工作时间"), this, SLOT(setWorkingHourDlg()));
		m_pMenu->addAction(QString(u8"标记完成"), this, SLOT(markFinish()));
		m_pMenu->addAction(QString(u8"清空进度"), this, SLOT(zeroSchedule()));
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
	MindProgressNode* progressNode = qobject_cast<MindProgressNode*>(m_parent);
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
		else if (m_progress == 1 && m_level >= 2)
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
	if (m_children.isEmpty())
	{
		_setProgress(0.0);
	}
	setWorkhours(hours);
	emit dataChanged(false);
}

void MindProgressNode::updateStatus()
{
	float totalHours = 0.0;
	float totalFinished = 0.0;
	for (auto it = m_children.begin(); it != m_children.end(); it++)
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
	if (!m_parent)
		return;
	MindProgressNode* parent = qobject_cast<MindProgressNode*>(m_parent);
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