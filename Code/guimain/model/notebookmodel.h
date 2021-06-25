#ifndef __NOTEBOOK_MODEL_H__
#define __NOTEBOOK_MODEL_H__


class NotebookModel : public QStandardItemModel
{
	Q_OBJECT
public:
	NotebookModel(QObject* parent = NULL);
	~NotebookModel();
};


#endif
