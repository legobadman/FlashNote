#include "stdafx.h"
#include "listpane.h"
#include "LeftSideItemDelegate.h"
#include "newitemdelegate.h"
#include "rpcservice.h"
#include "dbservice.h"
#include "note_types.h"
#include "MyStyle.h"
#include "guihelper.h"
#include "popupwidget.h"
#include <QMenu>
#include "moc_listpane.cpp"
#include "newnotewindow.h"
#include "labelbutton.h"


NewNoteItem::NewNoteItem(QWidget* parent)
	: QWidget(parent)
{
	setMouseTracking(true);
	QPalette palette = this->palette();
	palette.setBrush(QPalette::Window, QColor(42, 51, 60));
	setPalette(palette);

	static const int left = 16, top = 13, right = 21, bottom = 13;

	QHBoxLayout* pLayout = new QHBoxLayout;
	pLayout->setContentsMargins(MyStyle::dpiScaled(left), MyStyle::dpiScaled(top),
		MyStyle::dpiScaled(right), MyStyle::dpiScaled(bottom));
	pLayout->setSpacing(MyStyle::dpiScaled(9));

	m_pAddBtn = new QLabel;
	m_pAddBtn->setPixmap(QIcon(":/icons/btn_addnote.png").pixmap(MyStyle::dpiScaledSize(QSize(28, 28))));
	m_pAddBtn->installEventFilter(this);
	pLayout->addWidget(m_pAddBtn);

	m_pText = new QLabel(u8"新建笔记");
	QFont font("Microsoft YaHei", 13);
	m_pText->setFont(font);
	palette = m_pText->palette();
	palette.setBrush(QPalette::WindowText, QColor(213, 221, 227));
	m_pText->setPalette(palette);
	m_pText->installEventFilter(this);
	pLayout->addWidget(m_pText);

	pLayout->addStretch();

	//采用NLabelButton导致前面两个控件没法偏移！很是诡异
	m_pMore = new QLabel;
	m_pMore->setPixmap(QIcon(":/icons/newnotemenu.png").pixmap(MyStyle::dpiScaledSize(QSize(16, 16))));
	m_pMore->installEventFilter(this);
	pLayout->addWidget(m_pMore);

	setLayout(pLayout);
}

bool NewNoteItem::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == m_pText || watched == m_pAddBtn)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
            QRect rc = m_pText->geometry();
            m_pText->setGeometry(rc.adjusted(1, 1, 1, 1));
            rc = m_pAddBtn->geometry();
            m_pAddBtn->setGeometry(rc.adjusted(1, 1, 1, 1));
		}
		if (event->type() == QEvent::Enter)
		{
			QPalette palette = m_pText->palette();
			palette.setBrush(QPalette::WindowText, QColor(255, 255, 255));
			m_pText->setPalette(palette);
		}
		if (event->type() == QEvent::Leave)
		{
            QPalette palette = m_pText->palette();
            palette.setBrush(QPalette::WindowText, QColor(213, 221, 227));
			m_pText->setPalette(palette);
		}
		if (event->type() == QEvent::MouseButtonRelease)
		{
			QRect rc = m_pText->geometry();
			m_pText->setGeometry(rc.adjusted(-1, -1, -1, -1));
			rc = m_pAddBtn->geometry();
			m_pAddBtn->setGeometry(rc.adjusted(-1, -1, -1, -1));
			emit newnote(NORMAL_NOTE);
		}
	}
	else if (watched == m_pMore)
	{
		if (event->type() == QEvent::MouseButtonPress)
		{
            QRect rc = m_pMore->geometry();
			m_pMore->setGeometry(rc.adjusted(1, 1, 1, 1));
		}
        if (event->type() == QEvent::Enter)
        {
            m_pMore->setPixmap(QIcon(":/icons/newnotemenu_hover.png").pixmap(
                MyStyle::dpiScaledSize(QSize(16, 16))));
        }
        if (event->type() == QEvent::Leave)
        {
            m_pMore->setPixmap(QIcon(":/icons/newnotemenu.png").pixmap(
                MyStyle::dpiScaledSize(QSize(16, 16))));
        }
		if (event->type() == QEvent::MouseButtonRelease)
		{
            QRect rc = m_pMore->geometry();
			m_pMore->setGeometry(rc.adjusted(-1, -1, -1, -1));

            PopupWidget* popup = new PopupWidget(this);
            NewNoteMenu* pNewMenu = new NewNoteMenu;
            connect(pNewMenu, SIGNAL(newnote(NOTE_TYPE)), this, SIGNAL(newnote(NOTE_TYPE)));
            popup->setContentWidget(pNewMenu);
			QPoint pGlobal = mapToGlobal(rc.bottomRight());
            popup->exec(pGlobal.x(), pGlobal.y(), NEW_NOTE_WIDGET_WIDTH, NEW_NOTE_MENU_ITEM_HEIGHT * 3);
            delete popup;
		}
	}
	return QWidget::eventFilter(watched, event);
}


////////////////////////////////////////////////////////
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
	static const int xoffset = 30;
	if (pos.x() < MyStyle::dpiScaled(xoffset) && this->itemsExpandable())
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
	static int xrange_expand = MyStyle::dpiScaled(30);
	static int xrange_addbtn = MyStyle::dpiScaled(35);

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


NewNoteMenu::NewNoteMenu(QWidget* parent)
	: QListView(parent)
{
	connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onIndexClicked(const QModelIndex&)));

	QStandardItemModel* pModel = new QStandardItemModel;
 
    QIcon iconNote;
    iconNote.addFile(":/icons/menu_newnote.png", QSize(), QIcon::Normal);
    iconNote.addFile(":/icons/menu_newnote_on.png", QSize(), QIcon::Active);
	QStandardItem* pNormalNote = new QStandardItem(iconNote, u8"笔记");
	pModel->appendRow(pNormalNote);

	QIcon iconmap;
	iconmap.addFile(":/icons/menu_mindmap.png", QSize(), QIcon::Normal);
	iconmap.addFile(":/icons/menu_mindmap_on.png", QSize(), QIcon::Active);
	QStandardItem* pMindMap = new QStandardItem(iconmap, u8"思维导图");
	pModel->appendRow(pMindMap);

	QIcon iconschedule;
	iconschedule.addFile(":/icons/menu_schedule.png", QSize(), QIcon::Normal);
	iconschedule.addFile(":/icons/menu_schedule_on.png", QSize(), QIcon::Active);
	QStandardItem* pSchedule = new QStandardItem(iconschedule, u8"进度图");
	pModel->appendRow(pSchedule);

	setLineWidth(1);
	int style = this->frameStyle();
	QFrame::Shape shape = this->frameShape();

	setFrameShape(QFrame::Panel);
	setFrameShadow(QFrame::Sunken);

	setModel(pModel);
	viewport()->setAttribute(Qt::WA_Hover, true);
	setItemDelegate(new NewItemDelegate(this));
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

NewNoteMenu::~NewNoteMenu()
{
}

QSize NewNoteMenu::sizeHint() const
{
	return QSize(sizeHintForColumn(0), 3 * sizeHintForRow(0));
}

void NewNoteMenu::paintEvent(QPaintEvent* e)
{
	QListView::paintEvent(e);
}

void NewNoteMenu::onIndexClicked(const QModelIndex& index)
{
	if (index.row() == 0)
	{
		emit newnote(NORMAL_NOTE);
	}
	else if (index.row() == 1)
	{
		emit newnote(MINDMAP);
	}
	else if (index.row() == 2)
	{
		emit newnote(SCHEDULE);
	}
}


NavigationPanel::NavigationPanel(QWidget* parent)
	: QWidget(parent)
	, m_model(NULL)
	, m_treeview(NULL)
	, m_pCustomMenu(NULL)
{
	m_spNotifier.reset(new NavigationPanelNotifier(this));

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

	connect(m_newnote, SIGNAL(newnote(NOTE_TYPE)), this, SIGNAL(newnote(NOTE_TYPE)));
	connect(m_treeview, SIGNAL(clickObj(const QModelIndex&, MOUSE_HINT)), this,
		SLOT(onObjClick(const QModelIndex&, MOUSE_HINT)));

	m_treeview->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_treeview, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onCustomContextMenu(const QPoint&)));
}

NavigationPanel::~NavigationPanel()
{
}

QSize NavigationPanel::sizeHint() const
{
	QSize sz = QWidget::sizeHint();
	return MyStyle::dpiScaledSize(QSize(256, sz.height()));
}

void NavigationPanel::registerToNote(INote* pNote)
{
	if (pNote)
		pNote->addWatcher(m_spNotifier);
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
		u8"素材集"
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
	/*
	m_model->appendRow(pMaterialItem);
	m_model->appendRow(pFragmentItem);
	m_model->appendRow(pDiaryItem);
	m_model->appendRow(pDraftItem);
	*/
	initSchedule();
	m_model->appendRow(pTrashItem);

	m_treeview->setModel(m_model);
	m_treeview->setItemDelegate(new LeftSideItemDelegate(m_treeview));

    connect(m_treeview->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
        this, SLOT(onTreeItemSelected(const QModelIndex&, const QModelIndex&)));
}

void NavigationPanel::onCustomContextMenu(const QPoint& point)
{
	QModelIndex index = m_treeview->indexAt(point);

	if (m_pCustomMenu == NULL)
	{
		m_pCustomMenu = new QMenu(this);
		connect(m_pCustomMenu, SIGNAL(triggered(QAction*)), this, SLOT(MenuActionSlot(QAction*)));
	}

	ITEM_CONTENT_TYPE type = index.data(LEFT_SIDE_ROLE::ItemContentTypeRole).value<ITEM_CONTENT_TYPE>();
	if (type == ITEM_CONTENT_TYPE::ITEM_NOTEBOOKITEM)
	{
		m_pCustomMenu->clear();

		QAction* pDelete = new QAction(u8"删除笔记本", m_pCustomMenu);
		pDelete->setData((int)NavigationPanel::DELETE_NOTEBOOK);
		m_pCustomMenu->addAction(pDelete);
	
		QAction* pNewNote = new QAction(u8"新建笔记", m_pCustomMenu);
		pNewNote->setData((int)NavigationPanel::NEW_NORMALNOTE);
		m_pCustomMenu->addAction(pNewNote);

		m_pCustomMenu->popup(QCursor::pos());
	}
	else if (type == ITEM_CONTENT_TYPE::ITEM_SCHEDULEITEM)
	{
		m_pCustomMenu->clear();

		QAction* pDelete = new QAction(u8"删除进度表", m_pCustomMenu);
		pDelete->setData((int)NavigationPanel::DELETE_SCHEDULE);
		m_pCustomMenu->addAction(pDelete);
		m_pCustomMenu->popup(QCursor::pos());
	}
}

void NavigationPanel::MenuActionSlot(QAction* action)
{
	if (action == NULL)
		return;

	MENU_ITEM nIndex = (MENU_ITEM)action->data().toInt();
	if (nIndex == DELETE_NOTEBOOK || nIndex == NEW_NORMALNOTE)
	{
		QModelIndex index = m_treeview->currentIndex();
		QString bookid = index.data(ItemCoreObjIdRole).toString();

		com_sptr<INotebook> spNotebook;
		AppHelper::GetNotebookById(bookid, &spNotebook);

		if(nIndex == DELETE_NOTEBOOK)
		{
#ifdef USE_RPC
		bool bRet = RPCService::GetInstance().RemoveNotebook(AppHelper::coreApp(), spNotebook);
#else
		bool bRet = DbService::GetInstance().RemoveNotebook(AppHelper::coreApp(), spNotebook);
#endif	
		}
		else
		{
			NewNoteWindow* pNewNoteWindow = new NewNoteWindow(NULL, NORMAL_NOTE);
			if (pNewNoteWindow)
				pNewNoteWindow->init(bookid);
			pNewNoteWindow->show();
		}
	}
	else if (nIndex == DELETE_SCHEDULE)
	{
		QModelIndex index = m_treeview->currentIndex();
		QString noteid = index.data(ItemCoreObjIdRole).toString();

		com_sptr<ISchedules> spSchedules;
		AppHelper::coreApp()->GetSchedules(&spSchedules);

		com_sptr<INote> spNote;
		AppHelper::GetNote(spSchedules, noteid, &spNote);

		DbService::GetInstance().RemoveSchedule(AppHelper::coreApp(), spNote);
	}
}

HRESULT NavigationPanel::onCoreNotify(INoteCoreObj* pCoreObj, NotifyArg arg)
{
	if (com_sptr<INotebooks>(pCoreObj))
	{
		com_sptr<INotebooks> spNotebooks = pCoreObj;
		QModelIndex booksIdx = m_model->index((int)ITEM_CONTENT_TYPE::ITEM_NOTEBOOK, 0);
		QStandardItem* pNoteBookItem = m_model->item((int)ITEM_CONTENT_TYPE::ITEM_NOTEBOOK);
		
		switch (arg.ope)
		{
			case NotifyOperator::Add:
			{
				//Notebook::AddNotebook
				com_sptr<INotebook> spNotebook = arg.pObj;
				Q_ASSERT(spNotebook);
				QString bookName = AppHelper::GetNotebookName(spNotebook);
				QString bookId = AppHelper::GetNotebookId(spNotebook);

				QStandardItem* pItem = new QStandardItem(bookName);
				pNoteBookItem->appendRow(pItem);

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
					emit currentChanged(newIndex);
				}
			}
		}
	}
	else if (com_sptr<ISchedules>(pCoreObj))
	{
		com_sptr<ISchedules> spSchedules = pCoreObj;
		QStandardItem* pSchedulesItem = m_model->item((int)ITEM_CONTENT_TYPE::ITEM_SCHEDULE);

		com_sptr<INote> spNote = arg.pObj;
		Q_ASSERT(spNote);
		QString title = AppHelper::GetNoteTitle(spNote);
		QString noteid = AppHelper::GetNoteId(spNote);

		switch (arg.ope)
		{
			case NotifyOperator::Add:
			{
				QStandardItem* pItem = new QStandardItem(title);
				pSchedulesItem->appendRow(pItem);

				QString showContent = title;
				pItem->setText(showContent);
				pItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(
					ITEM_CONTENT_TYPE::ITEM_SCHEDULEITEM), ItemContentTypeRole);
				pItem->setData(QVariant::fromValue<ITEM_WIDGET_TYPE>(
					ITEM_WIDGET_TYPE::ITEM_CHILDLEVEL), ItemWidgetTypeRole);
				pItem->setData(QVariant(noteid), ItemCoreObjIdRole);
				pItem->setSelectable(true);
				pItem->setEditable(false);
				break;
			}
			case NotifyOperator::Delete:
			{
				QModelIndexList indexs = m_model->match(m_model->index(0, 0),
					ItemCoreObjIdRole,
					QVariant(noteid),
					1,		//hits
					Qt::MatchRecursive);
				if (!indexs.isEmpty())
				{
					Q_ASSERT(indexs.size() == 1);
					QModelIndex index = indexs.at(0);
					pSchedulesItem->removeRow(index.row());
					QItemSelectionModel* pModel = m_treeview->selectionModel();
					QModelIndex newIndex = pModel->currentIndex();
					emit clicked(newIndex);
				}
				break;
			}
		}
	}
	else if (com_sptr<INote>(pCoreObj))
	{
		com_sptr<INote> spNote = pCoreObj;
		NOTE_TYPE type = NORMAL_NOTE;
		spNote->GetType(&type);
		if (type == SCHEDULE)
		{
            QString noteid = AppHelper::GetNoteId(spNote);
            QModelIndexList indexs = m_model->match(
                m_model->index(0, 0),
                ItemCoreObjIdRole,
                QVariant(noteid),
                1,		//hits
                Qt::MatchRecursive);
			
			if (!indexs.isEmpty())
			{
				QModelIndex index = indexs.at(0);
				QStandardItem* pItem = m_model->itemFromIndex(index);
				if (pItem)
				{
                    QString text = pItem->text();
                    pItem->setText(AppHelper::GetNoteTitle(spNote));
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
	else if (type == ITEM_CONTENT_TYPE::ITEM_SCHEDULE)
	{
		if (hint == MOUSE_IN_ADD)
			emit newnote(SCHEDULE);
	}
}

void NavigationPanel::onTreeItemSelected(const QModelIndex& curr, const QModelIndex& prev)
{
	emit currentChanged(curr);
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

	com_sptr<INoteApplication> spApp = AppHelper::coreApp();
	com_sptr<INotebooks> spNotebooks;
	spApp->GetNotebooks(&spNotebooks);

	spNotebooks->addWatcher(m_spNotifier);

	int count = 0;
	spNotebooks->GetCount(&count);
	for (int i = 0; i < count; i++)
	{
		com_sptr<INotebook> spNotebook;
		spNotebooks->Item(i, &spNotebook);

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
	}
	m_model->appendRow(pNoteBookItem);
}

void NavigationPanel::initSchedule()
{
	QStandardItem* pScheduleItem = new QStandardItem(
		QIcon(":/icons/schedules.png"),
		u8"进度表"
	);
	pScheduleItem->setEditable(false);
	pScheduleItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(
		ITEM_CONTENT_TYPE::ITEM_SCHEDULE), ItemContentTypeRole);
	pScheduleItem->setData(QVariant::fromValue<ITEM_WIDGET_TYPE>(
		ITEM_WIDGET_TYPE::ITEM_TOPLEVEL), ItemWidgetTypeRole);

	com_sptr<INoteApplication> spApp = AppHelper::coreApp();
	com_sptr<ISchedules> spSchedules;
	spApp->GetSchedules(&spSchedules);

	spSchedules->addWatcher(m_spNotifier);

	int count = 0;
	spSchedules->GetCount(&count);
	for (int i = 0; i < count; i++)
	{
		com_sptr<INote> spNote;
		spSchedules->Item(i, &spNote);

		spNote->addWatcher(m_spNotifier);

		QString title = AppHelper::GetNoteTitle(spNote);
		QString id = AppHelper::GetNoteId(spNote);

		QStandardItem* pItem = new QStandardItem(title);
		pItem->setEditable(false);
		pItem->setData(QVariant::fromValue<ITEM_CONTENT_TYPE>(
			ITEM_CONTENT_TYPE::ITEM_SCHEDULEITEM), ItemContentTypeRole);
		pItem->setData(QVariant::fromValue<ITEM_WIDGET_TYPE>(
			ITEM_WIDGET_TYPE::ITEM_CHILDLEVEL), ItemWidgetTypeRole);
		pItem->setData(QVariant(id), ItemCoreObjIdRole);

		pScheduleItem->appendRow(pItem);
	}
	m_model->appendRow(pScheduleItem);
}
