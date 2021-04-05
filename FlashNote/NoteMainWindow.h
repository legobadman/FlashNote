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

private:
	Ui::NoteMainWindow* m_ui;
};

#endif
