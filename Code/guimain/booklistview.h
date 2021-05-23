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
		DELETE_NOTE = 0,
	};

public:
	BookListView(QWidget* parent = nullptr);
	~BookListView();

	void init();
	void updateNotebook(INotebook* pNotebook, int idxNote);
	HRESULT STDMETHODCALLTYPE onCoreNotify(
		INoteCoreObj* pCoreObj,
		NotifyArg arg);

signals:
	void noteitemclicked(const QModelIndex&);

public slots:
	void onCustomContextMenu(const QPoint& point);
	void MenuActionSlot(QAction *action);

public:
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, _COM_Outptr_ void __RPC_FAR* __RPC_FAR*) { return E_NOTIMPL; }
	ULONG STDMETHODCALLTYPE AddRef(void) { return 1; }
	ULONG STDMETHODCALLTYPE Release(void) { return 1; }

private:
	Ui::BookListView* m_ui;
	QStandardItemModel* m_model;
	QItemSelectionModel* m_selectionModel;
	com_sptr<INotebook> m_spNotebook;
	QMenu* m_pCustomMenu;
};

#endif
