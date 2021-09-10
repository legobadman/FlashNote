#include "stdafx.h"
#include "bookviewmodel.h"
#include "guihelper.h"
#include "uiapplication.h"


BookViewModel::BookViewModel(QObject* parent /* = nullptr */)
	: QAbstractItemModel(parent)
	, m_type(VIEW_NOTEBOOK)
{
	m_spNotifier.reset(new BookViewModelNotifier(this));
}

BookViewModel::~BookViewModel()
{
	clear();
}

void BookViewModel::initFromCollection(INoteCollection* pNoteCollection)
{
	if (com_sptr<INotebook>(pNoteCollection))
		m_type = VIEW_NOTEBOOK;
	else if (com_sptr<ITrash>(pNoteCollection))
		m_type = VIEW_TRASH;
	m_spNotebook = pNoteCollection;

	AddBookItems(m_spNotebook);
}

QModelIndex BookViewModel::findIdOf(const QString& objid)
{
	for (int r = 0; r < m_vec.size(); r++)
	{
		if (m_vec[r]->m_id == objid)
		{
			return index(r, 0);
		}
	}
	return QModelIndex();
}

QModelIndex BookViewModel::index(int row, int column, const QModelIndex& parent) const
{
	if (row >= 0 && row < m_vec.size())
	{
		NoteItem* item = m_vec[row];
		return createIndex(row, column, item);
	}
	else
	{
		return QModelIndex();
	}
}

void BookViewModel::GetShowContent(
			INote* pNote,
			const QString& time_format,
			QString& noteid,
			QString& title,
			QString& htmlContent,
			QString& full_plain_text,
			QString& text_abbre,
			QString& create_time,
			QString& modify_time)
{
	int nContentLimit = 74;
	title = AppHelper::GetNoteTitle(pNote);
	htmlContent = AppHelper::GetNoteContent(pNote);
	QString showContent;

	noteid = AppHelper::GetNoteId(pNote);
	create_time = AppHelper::GetCreateTime(pNote, time_format);
	modify_time = AppHelper::GetModifyTime(pNote, time_format);

	QTextDocument html;
	html.setHtml(htmlContent);
	full_plain_text = html.toPlainText();
	text_abbre = full_plain_text.replace('\n', ' ');
	text_abbre = text_abbre.mid(0, nContentLimit);
}

void BookViewModel::AddBookItems(INoteCollection* pNoteCollection)
{
	if (!pNoteCollection)
		return;

	int nNoteCount = 0;
	pNoteCollection->GetCount(&nNoteCount);
	for (int j = 0; j < nNoteCount; j++)
	{
		com_sptr<INote> spNote;
		AppHelper::GetNote(pNoteCollection, j, &spNote);
		QString id, title, html, content, textabbre, create_time, modify_time;
		GetShowContent(spNote, "yyyy/MM/dd", id, title, html, content, textabbre, create_time, modify_time);
		NoteItem* pItem = new NoteItem(id, title, textabbre);
		pItem->m_html = html;
		pItem->m_content = content;
		pItem->m_spNote = spNote;
		pItem->m_create_time = create_time;
		pItem->m_modify_time = modify_time;
		m_vec.push_back(pItem);

		spNote->addWatcher(m_spNotifier);
	}
	pNoteCollection->addWatcher(m_spNotifier);
}

HRESULT BookViewModel::onCoreNotify(INoteCoreObj* pCoreObj, NotifyArg arg)
{
	if (com_sptr<INotebook>(pCoreObj))
	{
		com_sptr<INotebook> spNotebook(pCoreObj);
		return onNotebookNotify(spNotebook, arg);
	}
	else if (com_sptr<ITrash>(pCoreObj))
	{
		com_sptr<ITrash> spTrash(pCoreObj);
		return onNotebookNotify(spTrash, arg);
	}
	else if (com_sptr<INote>(pCoreObj))
	{
		return onNoteNotify(pCoreObj, arg);
	}
	else if (com_sptr<INotebooks>(pCoreObj))
	{
		com_sptr<INotebooks> spNotebooks(pCoreObj);
		return onNotebooksNotify(spNotebooks, arg);
	}
	return S_OK;
}

void BookViewModel::clear()
{
	for (int i = 0; i < m_vec.size(); i++)
	{
		delete m_vec[i];
	}
	m_vec.clear();
}

HRESULT BookViewModel::onNotebookNotify(INoteCollection* pCoreObj, NotifyArg arg)
{
	Q_ASSERT(arg.pObj);
	com_sptr<INote> spNote(arg.pObj);
	Q_ASSERT(spNote);

	QString noteid = AppHelper::GetNoteId(spNote);
	
	switch (arg.ope)
	{
		case NotifyOperator::Add:
		{
			insertRow(spNote);
			spNote->addWatcher(m_spNotifier);
			break;
		}
		case NotifyOperator::Delete:
		{
			int idx = -1;
			for (int i = 0; i < m_vec.size(); i++)
			{
				if (m_vec[i]->m_id == noteid)
				{
					idx = i;
					break;
				}
			}
			removeRows(idx, 1);
			break;
		}
		case NotifyOperator::Update:
		{
			//TODO: 更新bookName
			break;
		}
	}
	return S_OK;
}

HRESULT BookViewModel::onNotebooksNotify(INotebooks* pNotebooks, NotifyArg arg)
{
	return E_NOTIMPL;
}

HRESULT BookViewModel::onNoteNotify(INoteCoreObj* pCoreObj, NotifyArg arg)
{
	com_sptr<INote> pNote = pCoreObj;
	switch (arg.ope)
	{
	case NotifyOperator::Delete:
	case NotifyOperator::Add:
	{
		//notebook已经做了。
		break;
	}
	case NotifyOperator::Update:
	{
		QString noteid = AppHelper::GetNoteId(pNote);
		for (int i = 0; i < m_vec.size(); i++)
		{
			if (m_vec[i]->m_id == noteid)
			{
				QString noteid, title, html, content, textAbbre, create_time, modify_time;
				GetShowContent(pNote, "yyyy/MM/dd", noteid, title, html, content, textAbbre, create_time, modify_time);

				m_vec[i]->m_id = noteid;
				m_vec[i]->m_title = title;
				m_vec[i]->m_textAbbre = textAbbre;
				m_vec[i]->m_html = html;
				m_vec[i]->m_content = content;
				m_vec[i]->m_create_time = create_time;
				m_vec[i]->m_modify_time = modify_time;

				QModelIndex idx = index(i, 0);
				emit dataChanged(idx, idx);
				break;
			}
		}
	}
	}
	return S_OK;
}

HRESULT BookViewModel::onTrashNotify(ITrash* pCoreObj, NotifyArg arg)
{
	return E_NOTIMPL;
}

bool BookViewModel::insertRow(INote* pNote)
{
	beginInsertRows(QModelIndex(), m_vec.size(), m_vec.size());

	QString noteid, title, textAbbre, html, content, create_time, modify_time;
	GetShowContent(pNote, "yyyy/MM/dd", noteid, title, html, content, textAbbre, create_time, modify_time);
	NoteItem* pItem = new NoteItem(noteid, title, textAbbre);
	pItem->m_html = html;
	pItem->m_content = content;
	pItem->m_spNote = pNote;
	pItem->m_create_time = create_time;
	pItem->m_modify_time = modify_time;
	m_vec.push_back(pItem);
	emit rowInserted(m_vec.size() - 1);

	endInsertRows();
	return false;
}

QModelIndex BookViewModel::parent(const QModelIndex& child) const
{
	return QModelIndex();
}

int BookViewModel::rowCount(const QModelIndex& parent) const
{
	return m_vec.size();
}

int BookViewModel::columnCount(const QModelIndex& parent) const
{
	return 3;
}

void BookViewModel::getNote(const QString& objId, INote** ppNote)
{
	if (!ppNote)
		return;
	for (int i = 0; i < m_vec.size(); i++)
	{
		if (m_vec[i]->m_id == objId)
		{
			*ppNote = m_vec[i]->m_spNote;
			(*ppNote)->AddRef();
			break;
		}
	}
}

bool BookViewModel::removeRows(int row, int count, const QModelIndex& parent)
{
	beginRemoveRows(parent, row, row + count - 1);
	m_vec.erase(m_vec.begin() + row, m_vec.begin() + row + count);
	endRemoveRows();
	return true;
}

QVariant BookViewModel::data(const QModelIndex& index, int role) const
{
	QVariant var;
	if (role == Qt::DisplayRole)
	{
		NoteItem* pItem = static_cast<NoteItem*>(index.internalPointer());
		switch (index.column())
		{
		case 0: return QVariant(pItem->m_title);
		case 1: return QVariant(pItem->m_create_time);
		case 2: return QVariant(pItem->m_modify_time);
		}
		QString displayText = pItem->m_title + "\n" + pItem->m_textAbbre;
		return QVariant(displayText);
	}
	else if (role == ItemCoreObjIdRole)
	{
		NoteItem* pItem = static_cast<NoteItem*>(index.internalPointer());
		QString id = pItem->m_id;
		return QVariant(id);
	}
	else if (role == ItemWidgetTypeRole)
	{
		return QVariant::fromValue<ITEM_WIDGET_TYPE>(ITEM_WIDGET_TYPE::ITEM_CHILDLEVEL);
	}
	else if (role == ItemContentTypeRole)
	{
		if (m_type == VIEW_TRASH)
			return QVariant::fromValue<ITEM_CONTENT_TYPE>(ITEM_CONTENT_TYPE::ITEM_TRASHITEM);
		else
			return QVariant::fromValue<ITEM_CONTENT_TYPE>(ITEM_CONTENT_TYPE::ITEM_NOTEBOOKITEM);
	}
	else if (role == ItemCoreObjRole)
	{
		NoteItem* pItem = static_cast<NoteItem*>(index.internalPointer());
		return QVariant::fromValue<com_sptr<INote>>(pItem->m_spNote);
	}
	else if (role == ItemNoteTitle)
	{
        NoteItem* pItem = static_cast<NoteItem*>(index.internalPointer());
        return QVariant(pItem->m_title);
	}
	else if (role == ItemNoteContent)
	{
		NoteItem* pItem = static_cast<NoteItem*>(index.internalPointer());
		return QVariant(pItem->m_textAbbre);
	}
    else if (role == ItemNoteAllContent)
    {
        NoteItem* pItem = static_cast<NoteItem*>(index.internalPointer());
        QString displayText = pItem->m_title + "\n" + pItem->m_content;
        return QVariant(displayText);
    }
	else if (role == ItemNoteShowDataTime)
	{
		NoteItem* pItem = static_cast<NoteItem*>(index.internalPointer());
		return QVariant(pItem->m_modify_time);
	}
	else if (role == ItemNoteCreateTime)
	{
        NoteItem* pItem = static_cast<NoteItem*>(index.internalPointer());
        return QVariant(pItem->m_create_time);
	}
	else if (role == ItemNoteModifyTime)
	{
        NoteItem* pItem = static_cast<NoteItem*>(index.internalPointer());
        return QVariant(pItem->m_modify_time);
	}
	else
	{
		return var;
	}
}

bool BookViewModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	//场景?
	NoteItem* pItem = static_cast<NoteItem*>(index.internalPointer());
	if (role == ItemCoreObjIdRole)
	{
	
	}
	return true;
}

QVariant BookViewModel::headerData(int section, Qt::Orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		switch (section)
		{
		case 0: return QVariant(u8"标题");
		case 1: return QVariant(u8"创建时间");
		case 2: return QVariant(u8"标题时间");
		}
	}
    return QVariant();
}


AllNotesModel::AllNotesModel(QObject* parent)
	: BookViewModel(parent)
{
}

AllNotesModel::~AllNotesModel()
{
}

void AllNotesModel::initAllNotes()
{
	//遍历所有笔记本
	m_type = VIEW_ALLNOTES;

	com_sptr<INotebooks> spNotebooks;
	AppHelper::coreApp()->GetNotebooks(&spNotebooks);
	spNotebooks->addWatcher(m_spNotifier);

	int nCount = 0;
	spNotebooks->GetCount(&nCount);
	for (int i = 0; i < nCount; i++)
	{
		com_sptr<INotebook> spNotebook;
		AppHelper::GetNotebook(i, &spNotebook);
		AddBookItems(spNotebook);
	}
}

HRESULT AllNotesModel::onNotebooksNotify(INotebooks* pNotebooks, NotifyArg arg)
{
	com_sptr<INotebook> spNotebook = arg.pObj;
	switch (arg.ope)
	{
		case NotifyOperator::Delete:
		{
			//TODO: DeleteNotebook的时候就分别release了每个note，每次release都会notify，
			//这样删除性能会有问题，如果在这里统一移除，需要改内核，暂且搁置。
			break;
		}
		case NotifyOperator::Add:
		{
			AddBookItems(spNotebook);
			break;
		}
	}
	return S_OK;
}


QuickSearchModel::QuickSearchModel(QObject* parent)
	: AllNotesModel(parent)
{
	initAllNotes();
}

QuickSearchModel::~QuickSearchModel()
{
}


QVariant QuickSearchModel::data(const QModelIndex& index, int role) const
{
    QVariant var;
    if (role == Qt::DisplayRole)
    {
        NoteItem* pItem = static_cast<NoteItem*>(index.internalPointer());
        return QVariant(pItem->m_title);
    }
    else if (role == Qt::EditRole)
    {
        NoteItem* pItem = static_cast<NoteItem*>(index.internalPointer());
        return QVariant(pItem->m_title);
    }
	else if (role == ItemNoteAllContent)
	{
        NoteItem* pItem = static_cast<NoteItem*>(index.internalPointer());
        QString displayText = pItem->m_title + "\n" + pItem->m_content;
		return QVariant(displayText);
	}
	return AllNotesModel::data(index, role);
}