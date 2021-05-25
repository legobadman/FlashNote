#include "stdafx.h"
#include "listpane.h"
#include "LeftSideItemDelegate.h"
#include "rpcservice.h"
#include "note_types.h"
#include "notewinservice.h"
#include "MyStyle.h"
#include "guihelper.h"
#include <QMenu>
#include "moc_listpane.cpp"


static QSizeF viewItemTextLayout(QTextLayout& textLayout, int lineWidth, int maxHeight = -1, int* lastVisibleLine = nullptr)
{
	if (lastVisibleLine)
		*lastVisibleLine = -1;
	qreal height = 0;
	qreal widthUsed = 0;
	textLayout.beginLayout();
	int i = 0;
	while (true) {
		QTextLine line = textLayout.createLine();
		if (!line.isValid())
			break;
		line.setLineWidth(lineWidth);
		line.setPosition(QPointF(0, height));
		height += line.height();
		widthUsed = qMax(widthUsed, line.naturalTextWidth());
		// we assume that the height of the next line is the same as the current one
		if (maxHeight > 0 && lastVisibleLine && height + line.height() > maxHeight) {
			const QTextLine nextLine = textLayout.createLine();
			*lastVisibleLine = nextLine.isValid() ? i : -1;
			break;
		}
		++i;
	}
	textLayout.endLayout();
	return QSizeF(widthUsed, height);
}


NewNoteItem::NewNoteItem(QWidget* parent)
	: QWidget(parent)
	, m_bPressed(false)
{
	setMouseTracking(true);
	setFixedSize(MyStyle::dpiScaledSize(QSize(255, 60)));
}

void NewNoteItem::initStyleOption(QStyleOptionViewItem* option) const
{
	if (!option)
		return;

	option->initFrom(this);
	option->icon = QIcon(":/icons/btn_addnote.png");
	option->state |= isDown() ? QStyle::State_Sunken : QStyle::State_Raised;
}

NewNoteItem::~NewNoteItem()
{
}

void NewNoteItem::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	QStyleOptionViewItem option;
	initStyleOption(&option);

	int offset = (option.state & QStyle::State_Sunken) ? 1 : 0;

	QIcon icon(":/icons/btn_addnote.png");
	painter.drawPixmap(16 + offset, 16 + offset, option.icon.pixmap(28, 28));

	QPen pen;
	if (option.state & QStyle::State_MouseOver)
	{
		pen.setColor(QColor("#FFFFFF"));
	}
	else
	{
		pen.setColor(QColor(213, 221, 227));
	}
	painter.setPen(pen);

	QFont font;
	font.setPointSize(13);
	font.setFamily("Microsoft YaHei");

	QPointF paintPosition(55 + offset, 19 + offset);

	QTextOption textOption;
	textOption.setWrapMode(QTextOption::ManualWrap);
	textOption.setTextDirection(Qt::LeftToRight);
	textOption.setAlignment(QStyle::visualAlignment(Qt::LeftToRight, Qt::AlignLeft));

	QTextLayout textLayout(u8"新建笔记", font);
	textLayout.setTextOption(textOption);
	viewItemTextLayout(textLayout, 100);
	textLayout.draw(&painter, paintPosition);
}

void NewNoteItem::setPressed(bool bPressed)
{
	if (bPressed == m_bPressed)
		return;
	m_bPressed = bPressed;
	update();
}

void NewNoteItem::mousePressEvent(QMouseEvent* e)
{
	if (e->button() == Qt::LeftButton)
	{
		emit LButtonPressed();
		setPressed(true);
	}
}

void NewNoteItem::mouseReleaseEvent(QMouseEvent* e)
{
	setPressed(false);
	emit clicked();
}

void NewNoteItem::enterEvent(QEvent* e)
{
	update();
}

void NewNoteItem::leaveEvent(QEvent* e)
{
	update();
}


NoteItemTreeView::NoteItemTreeView(QWidget* parent)
	: QTreeView(parent)
	, m_hoverObj(MOUSE_IN_OTHER)
{
	setMouseTracking(true);
}

void NoteItemTreeView::mousePressEvent(QMouseEvent* e)
{
	QPoint pos = e->pos();
	const QModelIndex& index = indexAt(pos);
	if (pos.x() < 30 && this->itemsExpandable())
	{
		setExpanded(index, !isExpanded(index));
	}
	else
	{
		
	}

	updateHoverState(e->pos());
	if (m_hoverObj == MOUSE_IN_EXPAND || m_hoverObj == MOUSE_IN_ADD)
	{
		emit clickObj(index, m_hoverObj);
	}
	else
	{
		QTreeView::mousePressEvent(e);
	}
}

void NoteItemTreeView::mouseMoveEvent(QMouseEvent* e)
{
	updateHoverState(e->pos());
	QTreeView::mouseMoveEvent(e);
}

void NoteItemTreeView::updateHoverState(QPoint pos)
{
	int width = this->width();
	const QModelIndex& index = indexAt(pos);
	static int xrange_expand = 30;
	static int xrange_addbtn = 35;

	if (pos.x() < xrange_expand && model()->hasChildren(index))
	{
		if (m_hoverObj != MOUSE_IN_EXPAND)
		{
			m_hoverObj = MOUSE_IN_EXPAND;
			update(index);
		}
	}
	else if (pos.x() > width - xrange_addbtn)
	{
		if (m_hoverObj != MOUSE_IN_ADD)
		{
			m_hoverObj = MOUSE_IN_ADD;
			update(index);
		}
	}
	else if (m_hoverObj != MOUSE_IN_OTHER)
	{
		m_hoverObj = MOUSE_IN_OTHER;
		update(index);
	}
}


NavigationPanel::NavigationPanel(QWidget* parent)
	: QWidget(parent)
	, m_model(NULL)
	, m_treeview(NULL)
	, m_pCustomMenu(NULL)
{
	m_treeview = new NoteItemTreeView(this);
	m_treeview->setFrameShape(QFrame::NoFrame);
	m_treeview->setFrameShadow(QFrame::Plain);
	m_treeview->setIndentation(0);
	m_treeview->setProperty("showDropIndicator", QVariant(false));
	m_treeview->header()->setVisible(false);
	m_treeview->setAlternatingRowColors(true);

	initModel();

	m_newnote = new NewNoteItem(this);

	QVBoxLayout* pLayout = new QVBoxLayout;
	pLayout->addWidget(m_newnote);
	pLayout->addWidget(m_treeview);
	pLayout->setMargin(0);
	setLayout(pLayout);

	connect(m_newnote, SIGNAL(clicked()), this, SIGNAL(newnote()));
	connect(m_treeview, SIGNAL(clicked(const QModelIndex&)), this, SIGNAL(clicked(const QModelIndex&)));
	connect(m_treeview, SIGNAL(clickObj(const QModelIndex&, MOUSE_HINT)), this,
		SLOT(onObjClick(const QModelIndex&, MOUSE_HINT)));

	m_treeview->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_treeview, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onCustomContextMenu(const QPoint&)));

}

NavigationPanel::~NavigationPanel()
{
}

void NavigationPanel::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QPen pen;
	painter.fillRect(rect(), QColor(42, 51, 60));
}

void NavigationPanel::initModel()
{
	m_model = new	QStandardItemModel(this);

	QStandardItem* pAllNotesItem = new QStandardItem(
		QIcon(":/icons/allnotes.png"),
		u8"全部笔记"
	);
	pAllNotesItem->setEditable(false);
	pAllNotesItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(
		ITEM_CONTENT_TYPE::ITEM_ALLNOTE), ItemContentTypeRole);
	pAllNotesItem->setData(QVariant::fromValue<ITEM_WIDGET_TYPE>(
		ITEM_WIDGET_TYPE::ITEM_TOPLEVEL), ItemWidgetTypeRole);

	QStandardItem* pMaterialItem = new QStandardItem(
		QIcon(":/icons/material.png"),
		u8"全部素材"
	);
	pMaterialItem->setEditable(false);
	pMaterialItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(
		ITEM_CONTENT_TYPE::ITEM_MATERIAL), ItemContentTypeRole);
	pMaterialItem->setData(QVariant::fromValue<ITEM_WIDGET_TYPE>(
		ITEM_WIDGET_TYPE::ITEM_TOPLEVEL), ItemWidgetTypeRole);

	QStandardItem* pFragmentItem = new QStandardItem(
		QIcon(":/icons/socialmediashare.png"),
		u8"碎片信息"
	);
	pFragmentItem->setEditable(false);
	pFragmentItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(
		ITEM_CONTENT_TYPE::ITEM_SOCIALMEDIA), ItemContentTypeRole);
	pFragmentItem->setData(QVariant::fromValue<ITEM_WIDGET_TYPE>(
		ITEM_WIDGET_TYPE::ITEM_TOPLEVEL), ItemWidgetTypeRole);

	QStandardItem* pDiaryItem = new QStandardItem(
		QIcon(":/icons/myspace.png"),
		u8"日记"
	);
	pDiaryItem->setEditable(false);
	pDiaryItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(
		ITEM_CONTENT_TYPE::ITEM_DIARY), ItemContentTypeRole);
	pDiaryItem->setData(QVariant::fromValue<ITEM_WIDGET_TYPE>(
		ITEM_WIDGET_TYPE::ITEM_TOPLEVEL), ItemWidgetTypeRole);

	QStandardItem* pScheduleItem = new QStandardItem(
		QIcon(":/icons/schedules.png"),
		u8"进度表"
	);
	pScheduleItem->setEditable(false);
	pScheduleItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(
		ITEM_CONTENT_TYPE::ITEM_SCHEDULE), ItemContentTypeRole);
	pScheduleItem->setData(QVariant::fromValue<ITEM_WIDGET_TYPE>(
		ITEM_WIDGET_TYPE::ITEM_TOPLEVEL), ItemWidgetTypeRole);

	QStandardItem* pDraftItem = new QStandardItem(
		QIcon(":/icons/draft.png"),
		u8"草稿"
	);
	pDraftItem->setEditable(false);
	pDraftItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(
		ITEM_CONTENT_TYPE::ITEM_DRAFT), ItemContentTypeRole);
	pDraftItem->setData(QVariant::fromValue<ITEM_WIDGET_TYPE>(
		ITEM_WIDGET_TYPE::ITEM_TOPLEVEL), ItemWidgetTypeRole);

	QStandardItem* pTrashItem = new QStandardItem(
		QIcon(":/icons/trash.png"),
		u8"废纸篓"
	);
	pTrashItem->setEditable(false);
	pTrashItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(
		ITEM_CONTENT_TYPE::ITEM_TRASH), ItemContentTypeRole);
	pTrashItem->setData(QVariant::fromValue<ITEM_WIDGET_TYPE>(
		ITEM_WIDGET_TYPE::ITEM_TOPLEVEL), ItemWidgetTypeRole);

	m_model->appendRow(pAllNotesItem);
	initNotebookItem();
	m_model->appendRow(pMaterialItem);
	m_model->appendRow(pFragmentItem);
	m_model->appendRow(pDiaryItem);
	m_model->appendRow(pScheduleItem);
	m_model->appendRow(pDraftItem);
	m_model->appendRow(pTrashItem);

	m_treeview->setModel(m_model);
	m_treeview->setItemDelegate(new LeftSideItemDelegate(m_treeview));
}

void NavigationPanel::onCustomContextMenu(const QPoint& point)
{
	QModelIndex index = m_treeview->indexAt(point);

	if (m_pCustomMenu == NULL)
	{
		m_pCustomMenu = new QMenu(this);
		connect(m_pCustomMenu, SIGNAL(triggered(QAction*)), this, SLOT(MenuActionSlot(QAction*)));
	}

	//暂时只处理Notebooks
	ITEM_CONTENT_TYPE type = index.data(LEFT_SIDE_ROLE::ItemContentTypeRole).value<ITEM_CONTENT_TYPE>();
	if (type == ITEM_CONTENT_TYPE::ITEM_NOTEBOOKITEM)
	{
		m_pCustomMenu->clear();

		QAction* pDelete = new QAction(u8"删除笔记本", m_pCustomMenu);
		pDelete->setData((int)NavigationPanel::DELETE_NOTEBOOK);
		m_pCustomMenu->addAction(pDelete);

		m_pCustomMenu->popup(QCursor::pos());
	}
}

void NavigationPanel::MenuActionSlot(QAction* action)
{
	if (action == NULL)
		return;

	MENU_ITEM nIndex = (MENU_ITEM)action->data().toInt();
	if (nIndex == DELETE_NOTEBOOK)
	{
		QModelIndex index = m_treeview->currentIndex();
		QString bookid = index.data(ItemCoreObjIdRole).toString();

		com_sptr<INotebook> spNotebook;
		AppHelper::GetNotebookById(bookid, &spNotebook);

		bool bRet = RPCService::GetInstance().RemoveNotebook(spNotebook);
	}
}

HRESULT NavigationPanel::onCoreNotify(INoteCoreObj* pCoreObj, NotifyArg arg)
{
	if (com_sptr<INotebooks>(pCoreObj))
	{
		com_sptr<INotebooks> spNotebooks = pCoreObj;
		QModelIndex booksIdx = m_model->index((int)ITEM_CONTENT_TYPE::ITEM_NOTEBOOK, 0);
		
		switch (arg.ope)
		{
			case NotifyOperator::Add:
			{
				//Notebook::AddNotebook
				com_sptr<INotebook> spNotebook = arg.pObj;
				Q_ASSERT(spNotebook);
				QString bookName = AppHelper::GetNotebookName(spNotebook);
				QString bookId = AppHelper::GetNotebookId(spNotebook);

				int insertIdx = 0;
				bool bRet = m_model->insertRow(insertIdx, booksIdx);
				Q_ASSERT(bRet);

				QModelIndex newItem = booksIdx.child(insertIdx, 0);
				QStandardItem* pItem = m_model->itemFromIndex(newItem);

				QString showContent = bookName;
				pItem->setText(showContent);
				pItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(
					ITEM_CONTENT_TYPE::ITEM_NOTEBOOKITEM), ItemContentTypeRole);
				pItem->setData(QVariant::fromValue<ITEM_WIDGET_TYPE>(
					ITEM_WIDGET_TYPE::ITEM_CHILDLEVEL), ItemWidgetTypeRole);
				pItem->setData(QVariant(bookId), ItemCoreObjIdRole);
				pItem->setSelectable(true);
				pItem->setEditable(false);
				break;
			}
			case NotifyOperator::Delete:
			{
				//Notebook::DeleteNotebook
				com_sptr<INotebook> spNotebook = arg.pObj;
			 	QString bookid = AppHelper::GetNotebookId(spNotebook);
				QModelIndexList indexs = m_model->match(
						m_model->index(0,0),
						ItemCoreObjIdRole,
						QVariant(bookid),
						1,		//hits
						Qt::MatchRecursive);
				if (!indexs.isEmpty())
				{
					Q_ASSERT(indexs.size() == 1);
					QModelIndex index = indexs.at(0);
					m_model->removeRow(index.row(), booksIdx);
					QItemSelectionModel* pModel = m_treeview->selectionModel();
					QModelIndex newIndex = pModel->currentIndex();
					emit clicked(newIndex);
				}
			}
		}
	}
	return E_NOTIMPL;
}

void NavigationPanel::onObjClick(const QModelIndex& index, MOUSE_HINT hint)
{
	ITEM_CONTENT_TYPE type = index.data(ItemContentTypeRole).value<ITEM_CONTENT_TYPE>();
	if (type == ITEM_CONTENT_TYPE::ITEM_NOTEBOOK)
	{
		if (hint == MOUSE_IN_ADD)
			emit addnotebook();
		else if (hint == MOUSE_IN_EXPAND)
			emit expand_changed(index);
	}
}

void NavigationPanel::initNotebookItem()
{
	QStandardItem* pNoteBookItem = new QStandardItem(
		QIcon(":/icons/notebooks.png"),
		u8"笔记本"
	);
	pNoteBookItem->setEditable(false);
	pNoteBookItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(
		ITEM_CONTENT_TYPE::ITEM_NOTEBOOK), ItemContentTypeRole);
	pNoteBookItem->setData(QVariant::fromValue<ITEM_WIDGET_TYPE>(
		ITEM_WIDGET_TYPE::ITEM_TOPLEVEL), ItemWidgetTypeRole);

	com_sptr<INoteApplication> spApp = coreApp;
	com_sptr<INotebooks> spNotebooks;
	spApp->GetNotebooks(&spNotebooks);

	spNotebooks->addWatcher(this);

	int count = 0;
	spNotebooks->GetCount(&count);
	QModelIndex activeIndex;
	for (int i = 0; i < count; i++)
	{
		VARIANT index;
		V_VT(&index) = VT_I4;
		V_I4(&index) = i;

		com_sptr<INotebook> spNotebook;
		spNotebooks->Item(index, &spNotebook);

		QString bookName = AppHelper::GetNotebookName(spNotebook);
		QString bookId = AppHelper::GetNotebookId(spNotebook);

		QStandardItem* pItem = new QStandardItem(bookName);
		pItem->setEditable(false);
		pItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(
			ITEM_CONTENT_TYPE::ITEM_NOTEBOOKITEM), ItemContentTypeRole);
		pItem->setData(QVariant::fromValue<ITEM_WIDGET_TYPE>(
			ITEM_WIDGET_TYPE::ITEM_CHILDLEVEL), ItemWidgetTypeRole);
		pItem->setData(QVariant(bookId), ItemCoreObjIdRole);

		pNoteBookItem->appendRow(pItem);
		activeIndex = pItem->index();
	}
	m_model->appendRow(pNoteBookItem);
}
