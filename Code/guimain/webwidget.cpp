#include "stdafx.h"
#include "webwidget.h"

webWidget::webWidget(const QString& url, QWidget* parent, const QRectF& rc)
{
	m_webView = wkeCreateWebWindow(WKE_WINDOW_TYPE_CONTROL, HWND(parent->winId()), rc.x(), rc.y(), rc.width(), rc.height());
	wkeShowWindow(m_webView, TRUE);
	wkeLoadURL(m_webView, url.toLocal8Bit().data());
}

webWidget::~webWidget()
{

}

void webWidget::loadUrl(const QString& url)
{
	wkeLoadURL(m_webView, url.toLocal8Bit().data());
}

void webWidget::loadHtmlW(const QString& html)
{
	wkeLoadHTMLW(m_webView, html.toStdWString().c_str());
}

void webWidget::resizeEvent(QResizeEvent* event)
{
	QSize sz = event->size();
	wkeResizeWindow(m_webView, sz.width(), sz.height());
}