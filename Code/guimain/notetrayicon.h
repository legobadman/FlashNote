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

signals:
	void contextActivated();
	void triggerActivated();
	void quickTriggerd();
	void hookTriggered(bool);

public slots:
	void onActivated(QSystemTrayIcon::ActivationReason reason);
	void onQuickTriggered();

private:
	NoteMainWindow* m_pWin;
	QApplication* m_pApp;
};

#endif
