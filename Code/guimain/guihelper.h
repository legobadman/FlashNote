#ifndef __GUI_HELPER_H__
#define __GUI_HELPER_H__

class AppHelper
{
public:
	static void GetNotebook(int idx, INotebook** ppNotebook);
	static void GetNote(INoteCollection* pNotebook, int idxNote, INote** ppNote);
	static QString GetNotebookName(INoteCollection* pNotebook);
	static QString GetNotebookId(INotebook* pNotebook);
	static void GetNotebookById(const QString& id, INotebook** ppNotebook);

	static QString GetNoteContent(INote* pNote);
	static QString GetNoteAbbre(INote* pNote);
	static QString GetNoteTitle(INote* pNote);
	static QString GetNoteId(INote* pNote);

	static int GetNoteCounts(INoteCollection* pNotebook);
};

#endif
