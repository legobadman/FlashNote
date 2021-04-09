#include "stdafx.h"
#include "listpane.h"
#include "moc_listpane.cpp"

NavigationPanel::NavigationPanel(QWidget* parent)
	: QTreeView(parent)
{
	header()->setSectionResizeMode(QHeaderView::ResizeToContents);
	setAlternatingRowColors(true);
}

NavigationPanel::~NavigationPanel()
{

}

void NavigationPanel::paintEvent(QPaintEvent* event)
{
	QTreeView::paintEvent(event);
}

void NavigationPanel::resizeEvent(QResizeEvent* event)
{
	QTreeView::resizeEvent(event);
}