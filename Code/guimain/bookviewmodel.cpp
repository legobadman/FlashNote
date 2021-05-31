#include "stdafx.h"
#include "bookviewmodel.h"
#include "guihelper.h"



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
	int nCount = 0;
	m_spNotebook->GetCount(&nCount);
	for (int i = 0; i < nCount; i++)
	{
		com_sptr<INote> spNote;
		AppHelper::GetNote(pNoteCollection, i, &spNote);
		QString id, title, content, textabbre;
		GetShowContent(spNote, id, title, content, textabbre);
		NoteItem* pItem = new NoteItem(id, title, textabbre);
		pItem->m_content = content;
		m_vec.push_back(pItem);
		spNote->addWatcher(this);
	}
	m_spNotebook->addWatcher(this);
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

void BookViewModel::GetShowContent(INote* pNote, QString& noteid, QString& title, QString& content, QString& text_abbre)
{
	int nContentLimit = 74;
	title = AppHelper::GetNoteTitle(pNote);
	content = AppHelper::GetNoteContent(pNote);
	QString showContent;

	noteid = AppHelper::GetNoteId(pNote);

	QTextDocument html;
	html.setHtml(content);
	text_abbre = html.toPlainText();
	text_abbre = text_abbre.mid(0, nContentLimit);
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
			//暂时用遍历的方式去掉
			for (int i = 0; i < m_vec.size(); i++)
			{
				if (m_vec[i]->m_id == noteid)
				{
					m_vec.removeAt(i);
					break;
				}
			}
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
				QString noteid, title, content, textAbbre;
				GetShowContent(pNote, noteid, title, content, textAbbre);

				m_vec[i]->m_id = noteid;
				m_vec[i]->m_title = title;
				m_vec[i]->m_textAbbre = textAbbre;
				m_vec[i]->m_content = content;

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

	QString noteid, title, textAbbre, content;
	GetShowContent(pNote, noteid, title, content, textAbbre);
	NoteItem* pItem = new NoteItem(noteid, title, textAbbre);
	pItem->m_content = content;
	m_vec.push_back(pItem);

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
	coreApp->GetNotebooks(&spNotebooks);

	int nCount = 0;
	spNotebooks->GetCount(&nCount);
	for (int i = 0; i < nCount; i++)
	{
		com_sptr<INotebook> spNotebook;
		AppHelper::GetNotebook(i, &spNotebook);
		spNotebook->addWatcher(this);

		int nNoteCount = 0;
		spNotebook->GetCount(&nNoteCount);
		for (int j = 0; j < nNoteCount; j++)
		{
			com_sptr<INote> spNote;
			AppHelper::GetNote(spNotebook, j, &spNote);
			QString id, title, content, textabbre;
			GetShowContent(spNote, id, title, content, textabbre);
			NoteItem* pItem = new NoteItem(id, title, textabbre);
			pItem->m_content = content;
			m_vec.push_back(pItem);
			spNote->addWatcher(this);
		}
		spNotebook->addWatcher(this);
	}
}

HRESULT AllNotesModel::onNotebooksNotify(INotebooks* pNotebooks, NotifyArg arg)
{
	com_sptr<INotebook> spNotebook = arg.pObj;
	switch (arg.ope)
	{
	case NotifyOperator::Delete:
	{
		//TODO：delete掉所有notebook下的note。
	}
	case NotifyOperator::Add:
	{
		//TODO: 将所有notebook下的note加到模型中。
	}
	}
	return S_OK;
}