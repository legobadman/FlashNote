#ifndef __NOTE_TRAYICON_H__
#define __NOTE_TRAYICON_H__

class NoteMainWindow;

class NoteTrayIcon : public QSystemTrayIcon
{
	Q_OBJECT
public:
	NoteTrayIcon(QObject* parent = nullptr);
	~NoteTrayIcon();

	void setMainWindow(NoteMainWindow* pWin);

public slots:
	void onActivated(QSystemTrayIcon::ActivationReason reason);

private:
	NoteMainWindow* m_pWin;
};

#endif
