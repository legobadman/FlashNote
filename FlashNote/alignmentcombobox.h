#ifndef __ALIGNMENT_COMBOBOX_H__
#define __ALIGNMENT_COMBOBOX_H__


class PopupWidget;

class AlignItemDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	AlignItemDelegate(QWidget* parent);
	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
	QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

protected:
	void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const;
};

class AlignmentComboBox : public MenuButton
{
    Q_OBJECT
public:
    AlignmentComboBox(QWidget* parent = nullptr);
    ~AlignmentComboBox();

signals:
    void alignmentChanged(int);

public slots:
    void popup();
    void onItemClicked(const QModelIndex& index);

private:
    PopupWidget* m_popup;
    QListView* m_pView;
    QStandardItemModel* m_pModel;
};

#endif
