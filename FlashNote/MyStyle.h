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
	//void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget = nullptr) const override;
	int pixelMetric(PixelMetric m, const QStyleOption* opt = nullptr, const QWidget* widget = nullptr) const override;
	QRect subElementRect(SubElement element, const QStyleOption* option, const QWidget* widget) const override;

private:
	void viewItemDrawText(QPainter* p, const QStyleOptionViewItem* option, const QRect& rect) const;
	QString calculateElidedText(const QString& text, const QTextOption& textOption,
		const QFont& font, const QRect& textRect, const Qt::Alignment valign,
		Qt::TextElideMode textElideMode, int flags,
		bool lastVisibleLineShouldBeElided, QPointF* paintStartPosition) const;

};

#endif
