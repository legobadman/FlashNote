#ifndef __MYSTYLE_H__
#define __MYSTYLE_H__

#include <QtWidgets/QCommonStyle>
#include <QtWidgets/QProxyStyle>
#include <QtWidgets/qstyle.h>


class StyleOptionToolButton;

class MyStyle : public QProxyStyle
{
	Q_OBJECT

	typedef QProxyStyle base;

public:
	enum MyComplexControl
	{
		CC_MyToolButton = CC_CustomBase + 1,
		CC_MyComboBox,
	};

	enum MyPrimitiveElement
	{
		PE_ComboBoxDropdownButton = PE_CustomBase + 1,
		PE_ComboBoxLineEdit,
	};

	MyStyle();
	~MyStyle();

	static qreal dpiScaled(qreal value);
	static QSize dpiScaledSize(const QSize& value);

	void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget = nullptr) const override;
	void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p,
		const QWidget* w = nullptr) const override;
    void drawControl(ControlElement element, const QStyleOption* opt, QPainter* p,
        const QWidget* w = nullptr) const override;
	QRect subControlRect(ComplexControl cc, const QStyleOptionComplex* opt, SubControl sc, const QWidget* widget) const override;
	int styleHint(StyleHint sh, const QStyleOption* opt = nullptr, const QWidget* w = nullptr,
		QStyleHintReturn* shret = nullptr) const override;
	int pixelMetric(PixelMetric m, const QStyleOption* opt = nullptr, const QWidget* widget = nullptr) const override;
	QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const override;
    QSize sizeFromContents(ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* widget = nullptr) const override;

private:
	void drawMyLineEdit(PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* widget) const;
	void drawComplexControl_MyToolButton(const StyleOptionToolButton* option, QPainter* painter, const QWidget* widget) const;
	void drawDropdownArrow(QPainter* painter, QRect downArrowRect) const;
	void drawNewItemMenu(const QStyleOptionMenuItem* menuitem, QPainter* p, const QWidget* w) const;
};

#endif
