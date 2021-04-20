#include <QtWidgets/qstyle.h>

class StyleOptionToolButton : public QStyleOptionToolButton
{
public:
	QString text2;
	int buttonStyle;
	int roundCorner;
	Qt::Orientation orientation;
	bool hideText;
	bool largeMargins;
	bool triggerAble;
	bool buttonEnabled;
	bool bDown;
	bool bHasDisabledBound;
	bool bEmbeded;
	bool isRemark;
	QColor textColor;
	QColor textHoverColor;
	QColor textDownColor;
	QColor borderColor;
	QColor borderInColor;
	QBrush bgBrush;
	QString themeName;
	QString status;
	bool isTouchScreenMode;
	bool isSplitButtonLarge;
	bool isRecommend;
	QPixmap recommendIcon;

	StyleOptionToolButton();
	StyleOptionToolButton(const StyleOptionToolButton& other) { *this = other; }
};