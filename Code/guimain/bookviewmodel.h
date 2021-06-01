#ifndef __BOOKVIEW_MODEL_H__
#define __BOOKVIEW_MODEL_H__

#include <QAbstractItemModel>
#include "LeftSideItemDelegate.h"
#include "common_types.h"

class NoteItem
{
public:
	NoteItem() {}
	NoteItem(const QString& id, const QString& title, const QString& textAbbre)
		: m_id(id)
		, m_title(title)
		, m_textAbbre(textAbbre)
	{
	}

	QString GetTitle() { return m_title; }
	QString GetId() { return m_id; }
	QString GetContent() { return m_content; }

public:
	QString m_id;
	QString m_title;
	QString m_textAbbre;
	QString m_content;
	com_sptr<INote> m_spNote;
};
Q_DECLARE_METATYPE(NoteItem)


class BookViewModel : public QAbstractItemModel
					, public ICoreNotify
{
	Q_OBJECT

public:
	BookViewModel(QObject* parent = nullptr);
	~BookViewModel();
	void initFromCollection(INoteCollection* pNoteCollection);
	QModelIndex findIdOf(const QString& objid);
	void getNote(const QString& objId, INote** ppNote);
	void removeRows(const QSet<QString>& objSet);

	virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	virtual QModelIndex parent(const QModelIndex& child) const override;

	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const { return 1; }
	virtual bool hasChildren(const QModelIndex& parent = QModelIndex()) { return false; }

	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
	virtual bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	void clear();

	HRESULT STDMETHODCALLTYPE onCoreNotify(
		INoteCoreObj* pCoreObj,
		NotifyArg arg);

	bool insertRow(INote* pNote);

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, _COM_Outptr_ void __RPC_FAR* __RPC_FAR*) { return E_NOTIMPL; }
	ULONG STDMETHODCALLTYPE AddRef(void) { return 1; }
	ULONG STDMETHODCALLTYPE Release(void) { return 1; }

protected:
	void GetShowContent(INote* pNote, QString& noteid, QString& title, QString& content, QString& text_abbr);
	void AddBookItems(INoteCollection* pNoteCollection);
	virtual HRESULT onNotebookNotify(INoteCollection* pCoreObj, NotifyArg arg);
	virtual HRESULT onNotebooksNotify(INotebooks* pNotebooks, NotifyArg arg);
	virtual HRESULT onTrashNotify(ITrash* pCoreObj, NotifyArg arg);
	virtual HRESULT onNoteNotify(INoteCoreObj* pCoreObj, NotifyArg arg);

protected:
	QMap<QString, NoteItem*> m_mapper;
	QVector<NoteItem*> m_vec;
	BOOKVIEW_TYPE m_type;

private:
	com_sptr<INoteCollection> m_spNotebook;
};

class AllNotesModel : public BookViewModel
{
public:
	AllNotesModel(QObject* parent = nullptr);
	~AllNotesModel();
	void initAllNotes();

protected:
	HRESULT onNotebooksNotify(INotebooks* pNotebooks, NotifyArg arg) override;
};

#endif
