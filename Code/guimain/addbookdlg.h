#ifndef __ADD_NOTEBOOK_DLG__
#define __ADD_NOTEBOOK_DLG__

namespace Ui
{
	class AddBookDlg;
}

class AddBookDlg : public QDialog
{
public:
	AddBookDlg(QWidget* parent = NULL);
	QString getName();
	
private:
	Ui::AddBookDlg* m_ui;
};

#endif
