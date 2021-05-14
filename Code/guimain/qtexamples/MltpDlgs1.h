#ifndef MLTPDLGS1_H
#define MLTPDLGS1_H

#include <QDialog>
#include "mltpdlgs2.h"

class QPushButton;

class MltpDlgs1 : public QDialog
{
	Q_OBJECT

public:
	MltpDlgs1(QWidget* parent = nullptr);

private slots:
	void on_withParent();
	void on_withoutParent();

private:
	QPushButton* withParent = nullptr;
	QPushButton* withoutParent = nullptr;
};
#endif // MLTPDLGS1_H