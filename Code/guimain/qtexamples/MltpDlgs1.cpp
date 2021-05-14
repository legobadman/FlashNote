#include "mltpdlgs1.h"

#include <QPushButton>
#include <QVBoxLayout>

MltpDlgs1::MltpDlgs1(QWidget* parent)
	: QDialog(parent)
{
	withParent = new QPushButton(tr("With Parent"));
	withoutParent = new QPushButton(tr("Without Parent"));

	auto mainLayout = new QVBoxLayout;
	mainLayout->addWidget(withParent);
	mainLayout->addStretch();
	mainLayout->addWidget(withoutParent);

	setLayout(mainLayout);

	connect(withParent, &QPushButton::clicked, this, &MltpDlgs1::on_withParent);
	connect(withoutParent, &QPushButton::clicked, this, &MltpDlgs1::on_withoutParent);
}

void MltpDlgs1::on_withParent()
{
	// With Parent
	auto dialog2 = new MltpDlgs2(this);
	Qt::WindowFlags flags = dialog2->windowFlags();
	dialog2->setWindowFlags(flags | Qt::CustomizeWindowHint |
		Qt::WindowStaysOnTopHint | Qt::Tool);
	dialog2->setMinimumSize(100, 100);
	dialog2->show();
}

void MltpDlgs1::on_withoutParent()
{
	// Without Parent
	auto dialog2 = new MltpDlgs2;
	dialog2->setMinimumSize(100, 100);
	dialog2->show();
}