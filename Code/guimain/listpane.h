#ifndef __LIST_PANE__H__
#define __LIST_PANE__H__


class NewNoteItem : public QWidget
{
	Q_OBJECT
public:
	NewNoteItem(QWidget* parent = nullptr);
	~NewNoteItem();

protected:
	void initStyleOption(QStyleOptionViewItem* option) const;
	void paintEvent(QPaintEvent* event) override;
	void enterEvent(QEvent* e) override;
	void leaveEvent(QEvent* e) override;
	void mousePressEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);

signals:
	void LButtonPressed();
	void clicked();

private:
	void setPressed(bool bPressed);
	bool isDown() const { return m_bPressed; }

private:
	bool m_bPressed;
};

class NoteItemTreeView : public QTreeView
{
	Q_OBJECT
public:
	NoteItemTreeView(QWidget* parent = nullptr);

protected:
	void mousePressEvent(QMouseEvent* e) override;
};

class NavigationPanel : public QWidget
{
	Q_OBJECT
public:
	NavigationPanel(QWidget* parent = nullptr);
	~NavigationPanel();
	QTreeView* treeview() { return m_treeview; }

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	void initNotebookItem();
	void initModel();

signals:
	void newnote();
	void clicked(const QModelIndex&);

private:
	QStandardItemModel* leftsidemodel;
	NoteItemTreeView* m_treeview;
	NewNoteItem* m_newnote;
};

#endif
