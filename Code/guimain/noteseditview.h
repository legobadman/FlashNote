#ifndef __NOTES_EDIT_VIEW_H__
#define __NOTES_EDIT_VIEW_H__

#include <QtWidgets/QSplitter>
#include <QtWidgets/QStackedWidget>
#include "common_types.h"
#include "bookviewmodel.h"

class NoteEditWindow;
class BookListView;

class NotesEditView : public QSplitter
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

	void setNotebook(BOOKVIEW_TYPE type, INoteCollection* pNotebook);

protected:
	bool eventFilter(QObject* watched, QEvent* event) override;

public slots:
	void onNoteItemSelected(const QModelIndex& index);

private:
	void onShowNotesView(QString noteid);

private:
	QWidget* m_pNoView;

	NoteEditWindow* m_pEditView;
	QStackedWidget* m_pStackedEdit;

	QMap<QString, BookViewModel*> m_models;
	AllNotesModel* m_pAllNotesModel;
	BookViewModel* m_pTrashModel;
	BookListView* m_pListView;
	BOOKVIEW_TYPE m_type;
};

#endif