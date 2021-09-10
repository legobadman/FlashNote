#ifndef __GUI_HELPER_H__
#define __GUI_HELPER_H__

#include <QtGui/QTextLayout>

class UiApplication;

class AppHelper
{
public:
	static UiApplication* uiApp();
	static INoteApplication* coreApp();
	static void GetNotebook(int idx, INotebook** ppNotebook);
	static void GetNote(INoteCollection* pNotebook, int idxNote, INote** ppNote);
	static void GetNote(INoteCollection* pNotebook, QString noteid, INote** ppNote);
	static void GetNoteAndBookById(QString noteid, INotebook** ppNotebook, INote** ppNote);
	static QString GetNotebookName(INoteCollection* pNotebook);
	static QString GetNotebookId(INotebook* pNotebook);
	static void GetNotebookById(const QString& id, INotebook** ppNotebook);
	static void GetNotebookByNote(INote* pNote, INotebook** ppNotebook);
	static void GetNotebookByName(const QString& name, INotebook** ppNotebook);

	static QString GetNoteContent(INote* pNote);
	static QString GetNoteAbbre(INote* pNote);
	static QString GetNoteTitle(INote* pNote);
	static QString GetNoteId(INote* pNote);
	static QString GetCreateTime(INote* pNote, const QString& dateTimeFormat);
	static QString GetModifyTime(INote* pNote, const QString& dateTimeFormat);
	static NOTE_TYPE GetNoteType(INote* pNote);
	static QStringList GetNotes(INoteCollection* pNoteColl);

	static QString GenerateGUID();

	static int GetNoteCounts(INoteCollection* pNotebook);
	static QSizeF viewItemTextLayout(QTextLayout& textLayout, int lineWidth, int maxHeight = -1, int* lastVisibleLine = nullptr);
	static void openNoteInIsoWindow(const QString& noteid);
	static QColor colorBlue();
};

#endif
