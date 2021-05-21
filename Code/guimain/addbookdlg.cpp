#include "stdafx.h"
#include "addbookdlg.h"
#include "ui_addbookdlg.h"


AddBookDlg::AddBookDlg(QWidget* parent)
	: QDialog(parent)
{
	//setWindowFlag(Qt::WindowCloseButtonHint, true);
	//setWindowFlag(Qt::WindowSystemMenuHint, true);
	//setWindowFlag(Qt::WindowTitleHint, true);
	setWindowFlag(Qt::WindowContextHelpButtonHint, false);

	m_ui = new Ui::AddBookDlg();
	m_ui->setupUi(this);
	m_ui->lineEdit->setPlaceholderText(u8"������ʼǱ�����");
	m_ui->buttonBox->button(QDialogButtonBox::Ok)->setText(u8"ȷ��");
	m_ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(u8"ȡ��");
}

QString AddBookDlg::getName()
{
	return m_ui->lineEdit->text();
}