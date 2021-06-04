#ifndef __NOTES_LISTVIEW_H__
#define __NOTES_LISTVIEW_H__

class NotesListView : public QListView
{
	Q_OBJECT
public:
	NotesListView(QWidget* parent = nullptr);
	~NotesListView();
	QModelIndex getHoverIndex() const;

protected:
	bool viewportEvent(QEvent* event) override;
	void paintEvent(QPaintEvent* e) override;

private:
	QVector<QModelIndex> getRenderedIndice(const QRect r);

private:
	QModelIndex m_hoverIndex;
};

#endif
