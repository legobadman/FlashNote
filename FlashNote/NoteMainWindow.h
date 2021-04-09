#ifndef __NOTE_MAIN_WINDOW_H__
#define __NOTE_MAIN_WINDOW_H__

namespace Ui
{
	class NoteMainWindow;
}

class LeftSideItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	LeftSideItemDelegate(QWidget* parent);
	void setModelData(QWidget* editor,
		QAbstractItemModel* model,
		const QModelIndex& index) const;
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:
	void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const;
};

class NoteMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	NoteMainWindow(QWidget* parent);
	~NoteMainWindow();

	void init();

private:
	Ui::NoteMainWindow* m_ui;
	QStandardItemModel* leftsidemodel;
};

#endif
