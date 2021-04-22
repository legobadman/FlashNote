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

private:
	ColorTableView* m_clrTable;
	ToolButton* m_btnDefaultClr;
	ToolButton* m_btnMoreClr;
};

#endif
