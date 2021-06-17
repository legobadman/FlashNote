#ifndef __NOTE_EDIT_WINDOW_H__
#define __NOTE_EDIT_WINDOW_H__

#include "notecore.h"
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
	void onTextChanged();
	void onMindMapChanged();
	void saveNote();
	void saveMindMap();
	void switchtobook(int bookidx);

private:
	void initSlots();

private:
	Ui::NoteEditWindow* m_ui;
	com_sptr<INote> m_pNote;
	com_sptr<INotebook> m_pNotebook;
	NOTE_TYPE m_type;
	bool m_bEdittable;
};

#endif
