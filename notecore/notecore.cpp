#include "notecore.h"
#include "notebase.h"

INote* CreateNote(NOTE_TYPE type)
{
	NoteBase* pNote = new NoteBase;
	return pNote;
}