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

public slots:
	void onLeftTreeClicked(const QModelIndex& index);

protected:
	void closeEvent(QCloseEvent* event) override;

private:
	Ui::NoteMainWindow* m_ui;
};

#endif
