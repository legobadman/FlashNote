#ifndef __NOTE_MAIN_WINDOW_H__
#define __NOTE_MAIN_WINDOW_H__

namespace Ui
{
	class NoteMainWindow;
}

#include "LeftSideItemDelegate.h"

class NoteMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	NoteMainWindow(QWidget* parent);
	~NoteMainWindow();
	void init();
	void initSelected();

public slots:
	void onLeftTreeClicked(const QModelIndex& index);
	void onNoteItemClicked(const QModelIndex& index);
	void onNewNote();
	void showNotesInfo(int idxNotebook, int idxNote);

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	int getActiveBookIndex();
	int getActiveNoteInBook(int bookidx);
	void onShowNote(int idxBook, INote* pNote);

private:
	Ui::NoteMainWindow* m_ui;
	com_sptr<INotebook> m_pNotebook;
};

#endif
