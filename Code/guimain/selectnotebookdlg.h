#ifndef __SELECT_NOTEBOOK_DLG_H__
#define __SELECT_NOTEBOOK_DLG_H__

namespace Ui
{
	class SelectNotebookDlg;
}

class SelectNotebookDlg : public QDialog
{
	Q_OBJECT
public:
	SelectNotebookDlg(QDialog* parent = NULL);
	QString getBookId();
	void setButtonText(const QString& okText, const QString& cancelText);

private:
	Ui::SelectNotebookDlg* m_ui;
};


#endif
