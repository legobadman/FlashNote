#ifndef __GLOBAL_SEARCH_EDITOR_H__
#define __GLOBAL_SEARCH_EDITOR_H__

#include <QSortFilterProxyModel>
#include <QCompleter>
#include "searcheditor.h"

class NoteCompleter;
class PopupWidget;
class AllNotesModel;

class GlobalSearchEditor : public QWidget
{
	Q_OBJECT
public:
	GlobalSearchEditor(QWidget* parent = NULL);
	~GlobalSearchEditor();

protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);

private slots:
	void quit();
	void onItemActivated(const QModelIndex& index);

private:
	QPoint m_origin_pos;
	QListView* m_searchView;
	SearchEditor* m_pEditor;
	AllNotesModel* m_model;
	QSortFilterProxyModel* m_pFilterModel;
	NoteCompleter* m_completer;

	const int nItemHeight = 25;
	const int nInitHeight = 36;
	const int nMaxItemRows = 10;
};

#endif
