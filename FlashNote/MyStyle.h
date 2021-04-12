#ifndef __MYSTYLE_H__
#define __MYSTYLE_H__

#include <QtWidgets/QCommonStyle>
#include <QtWidgets/QProxyStyle>
#include <QtWidgets/qstyle.h>

class MyStyle : public QProxyStyle
{
	Q_OBJECT

public:
	MyStyle();
	~MyStyle();

	void drawPrimitive(PrimitiveElement pe, const QStyleOption* opt, QPainter* p,
		const QWidget* w = nullptr) const override;
	int styleHint(StyleHint sh, const QStyleOption* opt = nullptr, const QWidget* w = nullptr,
		QStyleHintReturn* shret = nullptr) const override;
	int pixelMetric(PixelMetric m, const QStyleOption* opt = nullptr, const QWidget* widget = nullptr) const override;
	QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const override;
};

#endif
