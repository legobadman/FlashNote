#ifndef __BOOK_LIST_VIEW_H__
#define __BOOK_LIST_VIEW_H__

#include <QMenu>
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
	};

public:
	BookListView(NotesEditView* parent);
	~BookListView();

	void init();
	void resetModel(BookViewModel* pModel, BOOKVIEW_TYPE type, INoteCollection* pNoteCollection);

signals:
	void noteitemselected(const QModelIndex&);

public slots:
	void onCustomContextMenu(const QPoint& point);
	void MenuActionSlot(QAction *action);
	void onRowRemoved(int);
	void onRowInserted(int);

private:
	Ui::BookListView* m_ui;
	QMenu* m_pCustomMenu;
	NotesEditView* m_pNotesView;
};

#endif
