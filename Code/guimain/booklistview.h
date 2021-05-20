#ifndef __BOOK_LIST_VIEW_H__
#define __BOOK_LIST_VIEW_H__

namespace Ui
{
	class BookListView;
}

class BookListView : public QWidget
{
	Q_OBJECT
public:
	BookListView(QWidget* parent = nullptr);
	~BookListView();

	void init();
	void updateNotebook(INotebook* pNotebook, int idxNote);

signals:
	void noteitemclicked(const QModelIndex&);

private:
	Ui::BookListView* m_ui;
	QStandardItemModel* m_model;
	com_sptr<INotebook> m_spNotebook;
	int m_idxNote;
};

#endif
