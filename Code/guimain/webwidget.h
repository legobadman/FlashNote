#ifndef __WEB_WIDGET__
#define __WEB_WIDGET__

#include "wke.h"

class webWidget: public QWidget
{
public:
	webWidget(const QString& url="", QWidget* parent = 0, const QRectF& sz= QRectF(0, 0, 0, 0));
	~webWidget();
public:
	void loadUrl(const QString& url);
	void loadHtmlW(const QString& html);

protected:
	void resizeEvent(QResizeEvent* event);

private:
	wkeWebView m_webView;
};

#endif
