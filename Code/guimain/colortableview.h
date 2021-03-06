#ifndef __STANDARD_COLOR_VIEW_H__
#define __STANDARD_COLOR_VIEW_H__

class ColorTableView : public QAbstractItemView
{
	Q_OBJECT
public:
	ColorTableView(QWidget* parent = nullptr);
	~ColorTableView();
	void init();
	void initModel();
	QRect visualRect(const QModelIndex& index) const override;
	void scrollTo(const QModelIndex& index, ScrollHint hint = EnsureVisible) override;
	QModelIndex indexAt(const QPoint& point) const override;
	bool isHoverIndex(const QModelIndex& index) const;

protected:
	QModelIndex moveCursor(CursorAction cursorAction,
		Qt::KeyboardModifiers modifiers) override;
	int horizontalOffset() const override;
	int verticalOffset() const override;
	bool isIndexHidden(const QModelIndex& index) const override;
	void setSelection(const QRect& rect, QItemSelectionModel::SelectionFlags command) override;
	QRegion visualRegionForSelection(const QItemSelection& selection) const override;
	//void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* e) override;
	void enterEvent(QEvent* e) override;
	void leaveEvent(QEvent* e) override;

signals:
	void colorSelected(const QColor& color);

private:
	QStandardItemModel* model;
	QModelIndex m_hoverIdx;
};

#endif
