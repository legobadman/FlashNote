#ifndef __TOOLBUTTON_STYLE_H__
#define __TOOLBUTTON_STYLE_H__

class StyleOptionToolButton;

class ToolButtonStyle
{
public:
	//static int getMax2013LooseBtnHeight(const QFontMetrics& fm);
	//static int getMax2013NomrlnHeight(const QFontMetrics& fm);
public:
	ToolButtonStyle();
	virtual ~ToolButtonStyle();

	void initFromStyleOption(const StyleOptionToolButton* option);
	virtual void paint(QPainter* painter, const QWidget* widget);
	QSize contentSizeHint(Qt::Orientation o) const;
	//QSize calcSizeHint(Qt::Orientation o, const QWidget* w);

	bool needDrawIcon() const;
	bool needDrawText() const;
	bool needDrawArrow() const;
	bool needShowHotkey() const;
	bool needDrawSperatorLine() const;

	bool isTextUnderIcon() const;
	bool isEnabled() const;
	bool isDown() const;
	bool isPressed() const;
	bool isChecked() const;
	bool isHover() const;
	bool isAutoRaise() const;
	bool isNoButtonBackGround() const;
	//bool isIconColorFollowTheme() const;

	QColor textColor() const;
	QColor borderColor() const;
	QBrush bgBrush();

	void getDrawArea(QRect& iconArea, QRect& textArea, QRect& arrowArea) const;
	QStringList getDrawText() const;
	QPainterPath getHoverPath(const QWidget* widget) const;
	QRect getHoverRect(const QWidget* widget) const;
	void getButtonArea(QRect& btnArea, QRect& popupArea) const;

	void setOngmaniStyle(bool value);

protected:
	//QString themeName() const;
	//void paintBtnOnKRbTabWidget(QPainter* painter, const QWidget* widget);
	//void paintRemark(QPainter* painter, const QWidget* widget, const QRect& iconRect);
	void paintBackgrgOngmani(QPainter* painter, const QWidget*);
	void paintBackgrgNormal(QPainter* painter, const QWidget*);

	//virtual void paintKRbTabBtnBackground(QPainter* painter, const QRect& hoverRect,
	//	const StyleOptionToolButton* option);
	virtual void paintOngmaniBtnBackground(QPainter* painter, const QRect& hoverRect,
		const StyleOptionToolButton* option);
	virtual QRect adjustedArrowRect(const QRect& rcArrowDrawArea,
		const StyleOptionToolButton* option);

	void drawIcon(QPainter* painter, QRect& rcIcon);
	//void drawRecommendIcon(QPainter* painter, QRect& rcWidget);
	void drawText(QPainter* painter, QRect& rcText);
	void drawArrow(QPainter* painter, const QRect& rcArrow, const QPen& c, const QBrush& b);
	void drawArrow(QPainter* painter, const QRect& rcArrow, bool enabled);

protected:
	QSize _getIconDrawSize() const;
	QSize _getArrowDrawSize() const;
	QSize _getTextDrawSize(Qt::Orientation o) const;
	//QString _getProp() const;
	//QColor _getTabbarTextColor(const QString& str = "") const;
	//QColor _getThemeTextColor(bool b, bool bNormarl = true) const;
	//QPainterPath _getRectPath(const QRect& rect, int nRadius = 0, bool bSplit = false) const;
	virtual QMargins _getIconMargin() const;
	//QStringList _getDrawText_Ongmani() const;
	//QStringList _getDrawText_2012() const;
	//QPen _cachedPen(const QColor& col);
	//QBrush _cachedBrush(const QColor& col);
	//QBrush _cachedBrush(const QGradient& grad);

protected:
	const StyleOptionToolButton* m_option;

	QSize m_iconSz;
	QSize m_textSz;
	QSize m_arrowSz;
	bool m_needDrawText;
	bool m_needDrawIcon;
	bool m_needDrawRecommendIcon;
	bool m_needDrawArrow;
	bool m_bOngmaniStyle;
	QString m_themeName;
	bool m_isRemark;
	bool m_bArrowAside;
};

#endif
