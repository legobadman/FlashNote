#ifndef __BOOK_LIST_VIEW_H__
#define __BOOK_LIST_VIEW_H__

#include <QMenu>

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
	void updateNotebook(INoteCollection* pNotebook, QString noteid);
	HRESULT STDMETHODCALLTYPE onCoreNotify(
		INoteCoreObj* pCoreObj,
		NotifyArg arg);

signals:
	void noteitemselected(const QModelIndex&);

public slots:
	void onCustomContextMenu(const QPoint& point);
	void MenuActionSlot(QAction *action);

private:
	HRESULT onNotebookNotify(INoteCoreObj* pCoreObj, NotifyArg arg);
	HRESULT onNoteNotify(INoteCoreObj* pCoreObj, NotifyArg arg);
	QString GetShowContent(INote* pNote);

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, _COM_Outptr_ void __RPC_FAR* __RPC_FAR*) { return E_NOTIMPL; }
	ULONG STDMETHODCALLTYPE AddRef(void) { return 1; }
	ULONG STDMETHODCALLTYPE Release(void) { return 1; }

private:
	Ui::BookListView* m_ui;
	QStandardItemModel* m_model;
	QItemSelectionModel* m_selectionModel;
	com_sptr<INoteCollection> m_spNotebook;
	QMenu* m_pCustomMenu;
};

#endif
