#ifndef __BOOK_LIST_VIEW_H__
#define __BOOK_LIST_VIEW_H__

#include <QMenu>
#include "LeftSideItemDelegate.h"
#include "noteseditview.h"

namespace Ui
{
	class BookListView;
}

class BookListView : public QWidget
				   , public ICoreNotify
{
	Q_OBJECT

	enum MENU_ITEM
	{
		REMOVE_NOTE = 0,	//½«±Ê¼Ç±¾ÖÐµÄ±Ê¼ÇÒÆ³ýÖÁ·ÏÖ½Â¨
		RECOVER_NOTE,		//»Ö¸´·ÏÖ½Â¨ÖÐµÄ±Ê¼Ç¡£
		DELETE_NOTE,		//³¹µ×É¾³ý±Ê¼Ç¡£
	};

public:
	BookListView(NotesEditView* parent);
	~BookListView();

	void init();
	void initNoteContainer(BOOKVIEW_TYPE type, INoteCollection* pNotebook);
	void initAllNotes();
	HRESULT STDMETHODCALLTYPE onCoreNotify(
		INoteCoreObj* pCoreObj,
		NotifyArg arg);
	QString getCurrentNoteId();

signals:
	void noteitemselected(const QModelIndex&);

public slots:
	void onCustomContextMenu(const QPoint& point);
	void MenuActionSlot(QAction *action);

private:
	HRESULT onNotebookNotify(INotebook* pCoreObj, NotifyArg arg);
	HRESULT onTrashNotify(ITrash* pCoreObj, NotifyArg arg);
	HRESULT onNoteNotify(INoteCoreObj* pCoreObj, NotifyArg arg);
	HRESULT updateView(NotifyArg arg, INote* pNote);
	QString GetShowContent(INote* pNote);
	QString GetDefaultNoteId(INoteCollection* pNoteCollection);
	ITEM_CONTENT_TYPE getItemContentType(INoteCollection* pNotebook);
	void appendNotes(ITEM_CONTENT_TYPE itemType, INoteCollection* pNoteCollection);

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, _COM_Outptr_ void __RPC_FAR* __RPC_FAR*) { return E_NOTIMPL; }
	ULONG STDMETHODCALLTYPE AddRef(void) { return 1; }
	ULONG STDMETHODCALLTYPE Release(void) { return 1; }

private:
	Ui::BookListView* m_ui;
	QStandardItemModel* m_model;
	QItemSelectionModel* m_selectionModel;
	QMenu* m_pCustomMenu;
	NotesEditView* m_pNotesView;
	BOOKVIEW_TYPE m_type;
};

#endif
