#include "stdafx.h"
#include "listpane.h"

NavigationPanel::NavigationPanel(QWidget* parent)
	: QTreeView(parent)
{

}

NavigationPanel::~NavigationPanel()
{

}

void NavigationPanel::paintEvent(QPaintEvent* event)
{
	QTreeView::paintEvent(event);
}