#include "stdafx.h"
#include "mindprogressnode.h"
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QMenu>


MindProgressNode::MindProgressNode(const QString& text, MindProgressNode* parent)
	: MindNode(text, parent)
	, m_progress(0.0)
	, m_workinghours(0.0)
{
}

MindProgressNode::~MindProgressNode()
{
}

void MindProgressNode::setup()
{
	if (m_level == 0)
	{
		m_backgroudColor = QColor(242, 242, 242);
		m_highlightedBorder = QColor(23, 157, 235);
		m_selectedBorder = QColor(23, 157, 235);
		m_borderFocusout = QColor(m_backgroudColor);	//由于文本框的绘制策略，只能将同色的边框视为无边框。
	}
	else if (m_level == 1) {
		m_highlightedBorder = QColor(136, 203, 242);
		m_selectedBorder = QColor(23, 157, 235);
		m_backgroudColor = QColor(242, 242, 242);
		m_borderFocusout = QColor(m_backgroudColor);	//由于文本框的绘制策略，只能将同色的边框视为无边框。
	}
	else if (m_level >= 2) {
		m_highlightedBorder = QColor(136, 203, 242);
		m_selectedBorder = QColor(23, 157, 235);
		m_backgroudColor = QColor(255, 255, 255);
		m_borderFocusout = QColor(m_backgroudColor);	//由于文本框的绘制策略，只能将同色的边框视为无边框。
	}

	init();

	m_textColor = QColor(0, 0, 0);	//由于有绿白色，需要将文本设置为黑色。

	if (m_children.isEmpty() && m_parent)
	{
		updateToParent();
	}

	QGraphicsTextItem::setProgress(m_progress);
}

void MindProgressNode::initMenu()
{
	m_pMenu = new QMenu(NULL);
	m_pMenu->addAction(QString(u8"增加子级节点"), this, SLOT(onCreateChildNodeRight()));
	if (m_parent != NULL)
	{
		m_pMenu->addAction(QString(u8"增加同级节点"), this, SLOT(onCreateSliblingNode()));
		m_pMenu->addAction(QString(u8"删除节点"), this, SLOT(onDeleteNode()));
	}
	if (children().empty())
	{
		m_pMenu->addAction(QString(u8"设置工作时间"), this, SLOT(setWorkingHourDlg()));
		m_pMenu->addAction(QString(u8"标记完成"), this, SLOT(markFinish()));
		m_pMenu->addAction(QString(u8"清空进度"), this, SLOT(zeroSchedule()));
	}
}

void MindProgressNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	MindNode::paint(painter, option, widget);

	if (m_workinghours <= 0)
	{
		QIcon icon(":/icons/conflict.png");
		QPointF pt = boundingRect().bottomRight();
		pt.setX(pt.x() - 9);
		pt.setY(pt.y() - 22);
		painter->drawPixmap(pt, icon.pixmap(28, 28));
	}
}

float MindProgressNode::progress()
{
	return m_progress;
}

void MindProgressNode::setProgress(float progress)
{
	m_progress = progress;
}

void MindProgressNode::_setProgress(float progress)
{
	if (m_workinghours == 0)
	{
		progress = 0;
	}
	m_progress = progress;
	QGraphicsTextItem::setProgress(m_progress);
	update();

	emit dataChanged();
}

float MindProgressNode::workHours()
{
	return m_workinghours;
}

void MindProgressNode::setWorkhours(float hours)
{
	m_workinghours = hours;
}

void MindProgressNode::_setWorkhours(float hours)
{
	m_workinghours = hours;
	if (m_children.isEmpty())
	{
		_setProgress(0.0);
	}
	emit dataChanged();
}

void MindProgressNode::updateStatus()
{
	float totalHours = 0.0;
	float totalFinished = 0.0;
	for (auto it = m_children.begin(); it != m_children.end(); it++)
	{
		MindProgressNode* pNode = qobject_cast<MindProgressNode*>(*it);
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