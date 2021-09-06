#include "stdafx.h"
#include "bookviewmodel.h"
#include "guihelper.h"
#include "uiapplication.h"


BookViewModel::BookViewModel(QObject* parent /* = nullptr */)
	: QAbstractItemModel(parent)
	, m_type(VIEW_NOTEBOOK)
{
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
			QString& content,
			QString& text_abbre,
			QString& create_time,
			QString& modify_time)
{
	int nContentLimit = 74;
	title = AppHelper::GetNoteTitle(pNote);
	content = AppHelper::GetNoteContent(pNote);
	QString showContent;

	noteid = AppHelper::GetNoteId(pNote);
	create_time = AppHelper::GetCreateTime(pNote, time_format);
	modify_time = AppHelper::GetModifyTime(pNote, time_format);

	QTextDocument html;
	html.setHtml(content);
	text_abbre = html.toPlainText().replace('\n', ' ');
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
		QString id, title, content, textabbre, create_time, modify_time;
		GetShowContent(spNote, "yyyy/MM/dd", id, title, content, textabbre, create_time, modify_time);
		NoteItem* pItem = new NoteItem(id, title, textabbre);
		pItem->m_content = content;
		pItem->m_spNote = spNote;
		pItem->m_create_time = create_time;
		pItem->m_modify_time = modify_time;
		m_vec.push_back(pItem);
		m_mapper.insert(id, pItem);
		spNote->addWatcher(this);
	}
	pNoteCollection->addWatcher(this);
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
			spNote->addWatcher(this);
			break;
		}
		case NotifyOperator::Delete:
		{
			m_mapper.remove(noteid);
			//暂时用遍历的方式去掉
			int rowsRemoved = -1;
			for (int i = 0; i < m_vec.size(); i++)
			{
				if (m_vec[i]->m_id == noteid)
				{
					rowsRemoved = i;
					m_vec.removeAt(i);
					break;
				}
			}
			emit rowRemoved(rowsRemoved);
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
				QString noteid, title, content, textAbbre, create_time, modify_time;
				GetShowContent(pNote, "yyyy/MM/dd", noteid, title, content, textAbbre, create_time, modify_time);

				m_vec[i]->m_id = noteid;
				m_vec[i]->m_title = title;
				m_vec[i]->m_textAbbre = textAbbre;
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

	QString noteid, title, textAbbre, content, create_time, modify_time;
	GetShowContent(pNote, "yyyy/MM/dd", noteid, title, content, textAbbre, create_time, modify_time);
	NoteItem* pItem = new NoteItem(noteid, title, textAbbre);
	pItem->m_content = content;
	pItem->m_spNote = pNote;
	pItem->m_create_time = create_time;
	pItem->m_modify_time = modify_time;
	m_vec.push_back(pItem);
	m_mapper.insert(noteid, pItem);
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

void BookViewModel::removeRows(const QSet<QString>& objSet)
{
	QVector<NoteItem*> vec;
	for (int i = 0; i < m_vec.size(); i++)
	{
		NoteItem* pItem = m_vec[i];
		if (objSet.contains(pItem->m_id))
		{
			delete pItem;
			m_vec[i] = NULL;
		}
		else
		{
			vec.push_back(m_vec[i]);
		}
	}
	m_vec.clear();
	m_vec = vec;
}

QVariant BookViewModel::data(const QModelIndex& index, int role) const
{
	QVariant var;
	if (role == Qt::DisplayRole)
	{
		NoteItem* pItem = static_cast<NoteItem*>(index.internalPointer());
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
	else if (role == ItemNoteShowContent)
	{
		NoteItem* pItem = static_cast<NoteItem*>(index.internalPointer());
		return QVariant(pItem->m_textAbbre);
	}
	else if (role == ItemNoteShowDataTime)
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

	spNotebooks->addWatcher(this);

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