#include "stdafx.h"
#include "notelistview.h"


NotesListView::NotesListView(QWidget* parent)
	: QListView(parent)
{
}

NotesListView::~NotesListView()
{

}

bool NotesListView::viewportEvent(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::HoverMove:
	case QEvent::HoverEnter:
		{
			//TODO: 滚动时hover状态混乱。
			if (m_hoverIndex.row() > 0)
			{
				int lastRow = m_hoverIndex.row() - 1;
				QModelIndex idx = model()->index(lastRow, 0);
				update(idx);
			}
			m_hoverIndex = indexAt(static_cast<QHoverEvent*>(event)->pos());
		}
		break;
	case QEvent::HoverLeave:
		m_hoverIndex = QModelIndex();
		break;
	}
	return QListView::viewportEvent(event);
}

QModelIndex NotesListView::getHoverIndex() const
{
	return m_hoverIndex;
}

QVector<QModelIndex> NotesListView::getRenderedIndice(const QRect r)
{
	//TODO: 滚动条向下滚动的时候底部的item没有收集到。
	QVector<QModelIndex> vec;
	const int itemHeight = 87;
	int x = (float)r.width() / 2;

	const QModelIndex current = currentIndex();
	const QModelIndex hover = getHoverIndex();

	for (int y = r.top(); y < r.bottom(); y += itemHeight)
	{
		QModelIndex idx = indexAt(QPoint(x, y));
		if (idx.isValid())
		{
			vec.push_back(idx);
		}
	}
	return vec;
}

void NotesListView::paintEvent(QPaintEvent* e)
{
	QStyleOptionViewItem option = viewOptions();
	QPainter painter(this->viewport());

	const QRect r = e->rect();
	const QVector<QModelIndex> toBeRendered = getRenderedIndice(r);

	const QModelIndex current = currentIndex();
	const QModelIndex hover = getHoverIndex();
	const QAbstractItemModel* itemModel = model();
	const QItemSelectionModel* selections = this->selectionModel();
	const bool focus = (hasFocus() || viewport()->hasFocus()) && current.isValid();
	const QStyle::State state = option.state;
	const QAbstractItemView::State viewState = this->state();
	const bool enabled = (state & QStyle::State_Enabled) != 0;

	int maxSize = (flow() == TopToBottom)
		? qMax(viewport()->size().width(), contentsSize().width()) - 2 * spacing()
		: qMax(viewport()->size().height(), contentsSize().height()) - 2 * spacing();

	QVector<QModelIndex>::const_iterator end = toBeRendered.constEnd();
	for (QVector<QModelIndex>::const_iterator it = toBeRendered.constBegin(); it != end; ++it)
	{
		Q_ASSERT((*it).isValid());
		option.rect = visualRect(*it);

		if (flow() == TopToBottom)
			option.rect.setWidth(qMin(maxSize, option.rect.width()));
		else
			option.rect.setHeight(qMin(maxSize, option.rect.height()));

		option.state = state;
		if (selections && selections->isSelected(*it))
			option.state |= QStyle::State_Selected;
		if (enabled)
		{
			QPalette::ColorGroup cg;
			if ((itemModel->flags(*it) & Qt::ItemIsEnabled) == 0)
			{
				option.state &= ~QStyle::State_Enabled;
				cg = QPalette::Disabled;
			}
			else
			{
				cg = QPalette::Normal;
			}
			option.palette.setCurrentColorGroup(cg);
		}
		if (focus && current == *it)
		{
			option.state |= QStyle::State_HasFocus;
			if (viewState == EditingState)
				option.state |= QStyle::State_Editing;
		}
		option.state.setFlag(QStyle::State_MouseOver, *it == hover);
		itemDelegate()->paint(&painter, option, *it);

		int row = it->row();
		if (row != hover.row() && row != hover.row() - 1)
		{
			painter.setPen(QColor(217, 220, 221));
			painter.drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
		}
	}
}