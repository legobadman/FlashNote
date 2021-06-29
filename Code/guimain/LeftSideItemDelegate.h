#ifndef __LEFTSIDE_ITEM_DELEGATE_H__
#define __LEFTSIDE_ITEM_DELEGATE_H__

enum class ITEM_CONTENT_TYPE
{
	ITEM_ALLNOTE = 0,
	ITEM_NOTEBOOK,
	ITEM_SCHEDULE,
	ITEM_MATERIAL,
	ITEM_SOCIALMEDIA,
	ITEM_DIARY,	
	ITEM_DRAFT,
	ITEM_TRASH,
	ITEM_NOTEBOOKITEM,
	ITEM_SCHEDULEITEM,
	ITEM_TRASHITEM,
	ITEM_UNKNOWN,
};
Q_DECLARE_METATYPE(ITEM_CONTENT_TYPE)

enum class ITEM_WIDGET_TYPE
{
	ITEM_TOPLEVEL,	//左边最顶层项
	ITEM_CHILDLEVEL, //展开的子项
};
Q_DECLARE_METATYPE(ITEM_WIDGET_TYPE)

enum LEFT_SIDE_ROLE
{
	ItemWidgetTypeRole = Qt::UserRole + 1,
	ItemContentTypeRole,
	ItemCoreObjIdRole,
	ItemCoreObjRole,
	ItemNoteShowContent,
};

enum CONTENT_MAIN_VIEW
{
	NOTES_VIEW,
	NOTEBOOKS_VIEW,
	SCHEDULE_VIEW,
	TRASH_VIEW,
};

class LeftSideItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	LeftSideItemDelegate(QWidget* parent);
	void setModelData(QWidget* editor,
		QAbstractItemModel* model,
		const QModelIndex& index) const;
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:
	void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const;

private:
	void drawExpandArrow(QPainter* painter, const QStyleOptionViewItem& option) const;
};

#endif
