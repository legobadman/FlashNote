#ifndef __FONTCOLOR_COMBOBOX_H__
#define __FONTCOLOR_COMBOBOX_H__

#include "menubutton.h"
#include "colorgallery.h"

class FontColorComboBox : public MenuButton
{
	Q_OBJECT
public:
	FontColorComboBox(QWidget* parent = nullptr);
	~FontColorComboBox();
	void initStyleOption(QStyleOptionComboBox* option) const;
	void updateColor(const QColor& color);

protected:
	void paintEvent(QPaintEvent* event);
	void updateIcon() override;

protected slots:
	void popupChildWidget();
	void onFontColorItemClicked(const QColor& newColor);

signals:
	void colorChanged(const QColor& newColor);

private:
	QColor m_clr;
	QPointer<ColorGallery> m_pGallery;
	bool m_bUpdated;
};

#endif