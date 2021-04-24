#ifndef __POPUP_WIDGET_H__
#define __POPUP_WIDGET_H__

class PopupWidget : public QWidget
{
	Q_OBJECT
public:
	PopupWidget(QWidget* parent = nullptr);
	~PopupWidget();

	void setContentWidget(QWidget* contentWidget);
	void exec(int top, int left, int width, int height);

protected:
	void hideEvent(QHideEvent* event) override;
	void closeEvent(QCloseEvent* event) override;

signals:
	void aboutToHide();

private:
	QWidget* m_pContentWidget;
};

#endif
