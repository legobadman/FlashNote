#ifndef __COLOR_GALLERY_H__
#define __COLOR_GALLERY_H__

#include "colortableview.h"
#include "toolbutton.h"

class ColorGallery : public QFrame
{
	Q_OBJECT
	typedef QFrame _base;
public:
	ColorGallery(QWidget* parent = nullptr);
	~ColorGallery();
	void init();

protected:
	void closeEvent(QCloseEvent* event) override;
	void hideEvent(QHideEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	QColor getCurrentColor(const QColor& color);

signals:
	void fontColorChanged(const QColor& newColor);

public slots:
	void onItemActivated(const QModelIndex& index);
	void onDefaultColorClicked();
	void onMoreClrClicked();

private:
	ColorTableView* m_clrTable;
	ToolButton* m_btnDefaultClr;
	ToolButton* m_btnMoreClr;
	QColor m_color;
};

#endif
