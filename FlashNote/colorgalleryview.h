#ifndef __STANDARD_COLOR_VIEW_H__
#define __STANDARD_COLOR_VIEW_H__

class ColorGalleryView : public QAbstractItemView
{
	Q_OBJECT
public:
	ColorGalleryView(QWidget* parent = nullptr);
	~ColorGalleryView();
	void init();
	void initModel();
	QRect visualRect(const QModelIndex& index) const override;
	void scrollTo(const QModelIndex& index, ScrollHint hint = EnsureVisible) override;
	QModelIndex indexAt(const QPoint& point) const override;

protected:
	QModelIndex moveCursor(CursorAction cursorAction,
		Qt::KeyboardModifiers modifiers) override;
	int horizontalOffset() const override;
	int verticalOffset() const override;
	bool isIndexHidden(const QModelIndex& index) const override;
	void setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command) override;
	QRegion visualRegionForSelection(const QItemSelection& selection) const override;

	void paintEvent(QPaintEvent* e) override;

private:
	QStandardItemModel* model;

};

#endif
