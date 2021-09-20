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
	void updateNoteInfo(INotebook* pNotebook, INote* pNote, bool bTrash);
	INote* GetNote();
	void setBookName(const QString& name);

public slots:
	void onTitleChanged();
	void onTextChanged(bool delay);
	void onMindMapChanged(bool bEditChange);
	void trashNote();
	void saveNote();
	void saveMindMap();
	void saveSchedule();
	void switchtobook(int bookidx);
	void onNotebookMoved(INotebook* pNewbook);

private:
	void initSlots();
	void updateBookMenu(INotebook* pNotebook);
	void updateEditContent();

    HRESULT onCoreNotify(INoteCoreObj* pCoreObj, NotifyArg arg);
    struct NoteEditWindowNotifier : public ICoreNotify
    {
        NoteEditWindowNotifier(NoteEditWindow* pWidget) : m_pWidget(pWidget) {}
        HRESULT onCoreNotify(INoteCoreObj* pCoreObj, NotifyArg arg) {
            if (m_pWidget)
                return m_pWidget->onCoreNotify(pCoreObj, arg);
            else
                return E_NOTIMPL;
        }
        NoteEditWindow* m_pWidget;
    };
    shared_ptr<NoteEditWindowNotifier> m_spNotifier;

	struct RAII_CheckEditting
	{
		RAII_CheckEditting(bool* bEditting) : m_bEditting(bEditting) {
			*m_bEditting = true;
		}
		~RAII_CheckEditting() {
			*m_bEditting = false;
		}

		bool* m_bEditting;
	};
	bool m_bEditting;

signals:
	void noteCommited(const QString&);
	void noteDeleted();

private:
	Ui::NoteEditWindow* m_ui;
	com_sptr<INote> m_pNote;
	com_sptr<INotebook> m_pNotebook;
	NOTE_TYPE m_type;
	bool m_bTrash;
};

#endif
