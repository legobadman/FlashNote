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
	BookListView(QWidget* parent = nullptr);
	~BookListView();

	void init();
	void resetNotebook(INoteCollection* pNotebook, QString noteid);
	HRESULT STDMETHODCALLTYPE onCoreNotify(
		INoteCoreObj* pCoreObj,
		NotifyArg arg);

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
	ITEM_CONTENT_TYPE getItemContentType(INoteCollection* pNotebook);

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, _COM_Outptr_ void __RPC_FAR* __RPC_FAR*) { return E_NOTIMPL; }
	ULONG STDMETHODCALLTYPE AddRef(void) { return 1; }
	ULONG STDMETHODCALLTYPE Release(void) { return 1; }

private:
	Ui::BookListView* m_ui;
	QStandardItemModel* m_model;
	QItemSelectionModel* m_selectionModel;
	QMenu* m_pCustomMenu;

	QString m_bookid;
	BOOKVIEW_TYPE m_type;
};

#endif
