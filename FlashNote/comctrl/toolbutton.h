#ifndef __TOOL_BUTTON_H__
#define __TOOL_BUTTON_H__

class StyleOptionToolButton;

class ToolButton : public QWidget
{
	Q_OBJECT
public:
	enum ButtonStyle
	{
		ButtonUndefined = 0x0000,
		ButtonIcon = 0x0001 << 0,
		ButtonText = 0x0001 << 1,
		ButtonDownArrow = 0x0001 << 2,
		ButtonRightArrow = 0x0001 << 3,
		ButtonTextUnderIcon = 0x0001 << 4,
		ButtonSingleLineText = 0x0001 << 10,			// 默认Rainbow显示两行文字空白。这个属性用于特殊情况：只想显示单行文字，并且不需要多余的一行空白。
		ButtonNoButtonBg = 0x0001 << 11,			//不画背景
	};
	ToolButton(QWidget* parent = NULL);
	~ToolButton();

public:
	QString text() const;
	QIcon icon() const;
	QSize iconSize() const;
	bool isChecked() const;
	bool isDown() const;
	bool isPressed() const;
	int buttonStyle() const;
	virtual QSize sizeHint() const;

public slots:
	void setText(const QString& text);
	void setIcon(const QIcon& icon);
	void setIconSize(const QSize& size);
	void showToolTip();
	void setChecked(bool bChecked);
	void setDown(bool bDown);
	void setPressed(bool bPressed);
	void setButtonStyle(int style);
	virtual void updateIcon();

signals:
	void clicked();
	void doubleClicked();
	void LButtonClicked();
	void LButtonPressed();

protected:
	virtual bool event(QEvent* e);
	virtual void mousePressEvent(QMouseEvent* e);
	virtual void mouseReleaseEvent(QMouseEvent* e);
	virtual void enterEvent(QEvent* e);
	virtual void leaveEvent(QEvent* e);
	virtual void paintEvent(QPaintEvent* event);
	virtual void initStyleOption(StyleOptionToolButton* option) const;

	void setCustomTip(QString tip);
	QString getCustomTip();

protected:
	QSize m_iconSize;
	QIcon m_icon;

private:
	int m_buttonStyle;
	bool m_bHideText;
	int m_roundCorner;
	bool m_bChecked;
	bool m_bDown;
	bool m_bPressed;

	QString m_text;
	QString m_text2;
	QString m_customTip;
};

#endif
