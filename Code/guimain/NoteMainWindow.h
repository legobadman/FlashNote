#ifndef __NOTE_MAIN_WINDOW_H__
#define __NOTE_MAIN_WINDOW_H__

namespace Ui
{
	class NoteMainWindow;
}

class NoteMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	NoteMainWindow(QWidget* parent);
	~NoteMainWindow();
	void init();
	void showNavigationPane(bool show);
	void setApplication(QApplication* pApp);

public slots:
	void onLeftTreeClicked(const QModelIndex& index);

signals:
	void newnote();

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	Ui::NoteMainWindow* m_ui;
	QApplication* m_pApp;
};

#endif
