#include "stdafx.h"
#include "selectnotebookdlg.h"
#include "guihelper.h"
#include "ui_selectnotebookdlg.h"
#include "LeftSideItemDelegate.h"


SelectNotebookDlg::SelectNotebookDlg(QDialog* parent)
	: QDialog(parent)
{
	m_ui = new Ui::SelectNotebookDlg();

	m_ui->setupUi(this);

	setWindowFlags(Qt::SubWindow);

	connect(m_ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(m_ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

QString SelectNotebookDlg::getBookId()
{
	return m_ui->bookpanel->currentBookId();
}

void SelectNotebookDlg::setButtonText(const QString& okText, const QString& cancelText)
{
    QPushButton* btnOk = m_ui->buttonBox->button(QDialogButtonBox::Ok);
    QPushButton* btnCancel = m_ui->buttonBox->button(QDialogButtonBox::Cancel);
    btnOk->setText(okText);
    btnCancel->setText(cancelText);
}