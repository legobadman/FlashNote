#ifndef __COLOR_GALLERY_H__
#define __COLOR_GALLERY_H__

#include "colortableview.h"
#include "toolbutton.h"

class ColorGallery : public QWidget
{
	Q_OBJECT
public:
	ColorGallery(QWidget* parent = nullptr);
	~ColorGallery();
	void init();

protected:
	void closeEvent(QCloseEvent* event) override;
	void hideEvent(QHideEvent* event) override;
	QColor getCurrentColor(const QColor& color);

signals:
	void fontColorChanged(const QColor& newColor);

public slots:
	void onItemActivated(const QModelIndex& index);
	void onDefaultColorClicked();

private:
	ColorTableView* m_clrTable;
	ToolButton* m_btnDefaultClr;
	ToolButton* m_btnMoreClr;
	QColor m_color;
};

#endif