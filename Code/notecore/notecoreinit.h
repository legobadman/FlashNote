#ifndef __NOTECORE_INIT_H__
#define __NOTECORE_INIT_H__

#include "notecore2.h"

HRESULT CreateNote(NOTE_TYPE type, INote** ppNote);
HRESULT CreateNotebook(INotebook** ppNotebook);
HRESULT CreateApplication(INoteApplication** pApp);
HRESULT CreateNotebooks(INotebooks** ppNotebooks);
HRESULT CreateTrash(ITrash** ppTrash);
HRESULT CreateSchedules(ISchedules** ppSchedules);

#endif
