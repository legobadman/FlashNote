#ifndef __NOTE_WIN_SERVICE_H__
#define __NOTE_WIN_SERVICE_H__

#include <QObject>

#include "NoteMainWindow.h"
#include "notetrayicon.h"

typedef NoteMainWindow NewNoteWindow;

class NoteWinService : public QWidget
{
	Q_OBJECT
public:
	NoteWinService(QWidget* parent);
	~NoteWinService();
	void startup();

public slots:
	void onNewNote();
	void onQuickApp();
	void onTrigger();

private:
	QVector<QSharedPointer<NewNoteWindow>> m_subWindows;
	QSharedPointer<NoteMainWindow> m_mainWindow;
	NoteTrayIcon m_trayIcon;
};

#endif
