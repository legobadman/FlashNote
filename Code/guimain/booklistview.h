#ifndef __BOOK_LIST_VIEW_H__
#define __BOOK_LIST_VIEW_H__

#include <QMenu>
#include <QSortFilterProxyModel>
#include "LeftSideItemDelegate.h"
#include "noteseditview.h"
#include "bookviewmodel.h"

namespace Ui
{
	class BookListView;
}

class BookListView : public QWidget
{
	Q_OBJECT

	enum MENU_ITEM
	{
		REMOVE_NOTE = 0,	//���ʼǱ��еıʼ��Ƴ�����ֽ¨
		RECOVER_NOTE,		//�ָ���ֽ¨�еıʼǡ�
		DELETE_NOTE,		//����ɾ���ʼǡ�
		OPEN_NOTE,			//�����򿪱ʼǡ�
		SORT_MODIFY_TIME,
		SORT_CREATE_TIME,
		SORT_TITLE,
		SORT_ASCEND,
	};

	enum SORT_OPTION
	{
		SOP_MODIFY_TIME,
		SOP_CREATE_TIME,
		SOP_TITLE,
	};

public:
	BookListView(NotesEditView* parent);
	~BookListView();

	void init();
	void resetModel(QSortFilterProxyModel* pModel, BOOKVIEW_TYPE type, INoteCollection* pNoteCollection);

signals:
	void searchTriggered(const QString&);

public slots:
	void onCustomContextMenu(const QPoint& point);
	void MenuActionSlot(QAction *action);
	void onSearchTextChanged(const QString&);
	void onSortBtnClicked();
	void onViewBtnClicked();

private:
	void _sort();

	Ui::BookListView* m_ui;
	QMenu* m_pCustomMenu;
	NotesEditView* m_pNotesView;
	QSortFilterProxyModel* m_model;
	SORT_OPTION m_sortOption;
	bool m_bAscent;
};

#endif
