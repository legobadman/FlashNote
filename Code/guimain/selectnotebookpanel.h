#ifndef __SELECTNOTEBOOK_PANEL_H__
#define __SELECTNOTEBOOK_PANEL_H__


class BooksListView : public QListView
{
	Q_OBJECT
public:
	BooksListView(QWidget* parent = NULL);
	~BooksListView();
	QSize sizeHint() const override;
	void calcWidthByItems();

private:
	
	int m_cachedWidth;
};

class BookItemDelegate : public QAbstractItemDelegate
{
	Q_OBJECT
public:
	BookItemDelegate(QObject* parent = NULL);
	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
};


class SelectNotebookPanel : public QWidget
{
	Q_OBJECT
public:
	SelectNotebookPanel(QWidget* parent = NULL);
	~SelectNotebookPanel();

signals:
	void notebookMoved(INotebook* pNotebook);

public slots:
	void onIndexClicked(const QModelIndex&);

private:
	BooksListView* m_pListView;
};

#endif
