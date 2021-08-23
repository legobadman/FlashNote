#ifndef __NOTE_EDIT_WINDOW_H__
#define __NOTE_EDIT_WINDOW_H__

#include "notecore2.h"
#include "ui_note_editwindow.h"

namespace Ui
{
	class NoteEditWindow;
}

class NoteEditWindow : public QWidget
{
	Q_OBJECT
public:
	NoteEditWindow(QWidget* parent = nullptr);
	~NoteEditWindow();
	void init();
	void initCustomWidget();
	void initContent();
	void updateNoteInfo(INotebook* pNotebook, INote* pNote, bool edittable);
	INote* GetNote();
	void setBookName(const QString& name);

public slots:
	void onTitleChanged();
	void onTextChanged(bool delay);
	void onMindMapChanged(bool bEditChange);
	void saveNote();
	void saveMindMap();
	void saveSchedule();
	void switchtobook(int bookidx);
	void onNotebookMoved(INotebook* pNewbook);

private:
	void initSlots();
	void updateBookMenu(INotebook* pNotebook);

signals:
	void noteCommited(const QString&);

private:
	Ui::NoteEditWindow* m_ui;
	com_sptr<INote> m_pNote;
	com_sptr<INotebook> m_pNotebook;
	NOTE_TYPE m_type;
	bool m_bEdittable;
};

#endif
