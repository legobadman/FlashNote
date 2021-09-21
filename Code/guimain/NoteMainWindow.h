#ifndef __NOTE_MAIN_WINDOW_H__
#define __NOTE_MAIN_WINDOW_H__

namespace Ui
{
	class NoteMainWindow;
}

#include "LeftSideItemDelegate.h"
#include "floatingmenubutton.h"

class NoteMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	NoteMainWindow(QWidget* parent);
	~NoteMainWindow();
	void init();

public slots:
	void onLeftTreeClicked(const QModelIndex& index);
	void onNewNote(NOTE_TYPE);
	void onAddNotebook();
	void initNotesView(int idxNotebook, int idxNote);
	void initMenuSignal();
	void _temp_hide_floatWin();

protected:
	void closeEvent(QCloseEvent* event) override;
	bool nativeEvent(const QByteArray& eventType, void* message, long* result) override;

private:
	QString getActiveBookId();
	int getActiveNoteInBook(int bookidx);
	INote* GetCurrentActiveNote();
	INoteCollection* GetCurrentNoteCollection();
	void _enableNoteMenu(bool enable);

private:
	Ui::NoteMainWindow* m_ui;
	FloatingMenuButton* m_pMenuButton;
};

#endif
