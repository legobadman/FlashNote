#include "mltpdlgs2.h"

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

MltpDlgs2::MltpDlgs2(QWidget* parent) : QWidget(parent, Qt::Window)
{
	label = new QLabel(tr("This is dialog 2"));
	clse = new QPushButton(tr("Close"));

	auto mainLayout = new QVBoxLayout;
	mainLayout->addWidget(label);
	mainLayout->addStretch();
	mainLayout->addWidget(clse);
	setLayout(mainLayout);

	connect(clse, &QPushButton::clicked, this, &MltpDlgs2::close);
}