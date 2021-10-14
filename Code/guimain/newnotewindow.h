#ifndef __NEWNOTEWINDOW_H__
#define __NEWNOTEWINDOW_H__

namespace Ui
{
	class NewNoteWindow;
}

class NewNoteWindow : public QMainWindow
{
	Q_OBJECT
public:
	NewNoteWindow(QWidget* parent, NOTE_TYPE type);
	~NewNoteWindow();
	void init(QString bookid);
	INote* initSchedule();
	void open(INotebook* pNotebook, INote* pNote);
	void getNote(INote** ppNote);

protected:
	void closeEvent(QCloseEvent* event) override;

signals:
	void closeWin();
	void noteCommited(const QString&);
	void newnote(const QString& noteid);

private:
	Ui::NewNoteWindow* m_ui;
	NOTE_TYPE m_type;
	com_sptr<INote> m_pNote;
};

#endif
