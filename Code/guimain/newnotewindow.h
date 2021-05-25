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
	NewNoteWindow(QWidget* parent);
	~NewNoteWindow();
	void init(QString bookid);

protected:
	void closeEvent(QCloseEvent* event) override;

signals:
	void closeWin();

private:
	Ui::NewNoteWindow* m_ui;
	com_sptr<INote> m_pNote;
};

#endif
