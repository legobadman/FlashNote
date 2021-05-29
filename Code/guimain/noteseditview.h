#ifndef __NOTES_EDIT_VIEW_H__
#define __NOTES_EDIT_VIEW_H__

#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>

class NoteEditWindow;
class BookListView;


class MySplitter : public QSplitter
{
	Q_OBJECT
public:
	MySplitter(QWidget* parent = nullptr);

protected:
	void paintEvent(QPaintEvent* event);
};

//以后再研究QWidget的布局。
#define SPLITTER_BASE

#ifdef SPLITTER_BASE
class NotesEditView : public QSplitter
#else
class NotesEditView : public QWidget
#endif
{
	Q_OBJECT

	enum PAGE_IDX
	{
		PAGE_EDITVIEW,
		PAGE_NOEDIT,
	};

public:
	NotesEditView(QWidget* parent = NULL);
	~NotesEditView();

	void setNotebook(INoteCollection* pNotebook);

#ifndef SPLITTER_BASE
	QSize sizeHint() const override;
#endif

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;

public slots:
	void onNoteItemSelected(const QModelIndex& index);

private:
	void onShowNotesView(QString noteid);

private:
	QWidget* m_pNoView;
	QString m_bookid;
	BookListView* m_pBookView;
	NoteEditWindow* m_pEditView;
	QStackedWidget* m_pStackedWidget;
	com_sptr<INoteCollection> m_pNotebook;
};

#endif