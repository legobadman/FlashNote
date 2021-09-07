#ifndef __SEARCH_EDITOR_H__
#define __SEARCH_EDITOR_H__

#include <QLineEdit>

class SearchEditor : public QLineEdit
{
	Q_OBJECT
public:
	SearchEditor(QWidget* parent = nullptr);
	~SearchEditor();

protected:
	void paintEvent(QPaintEvent* event);

};

#endif
