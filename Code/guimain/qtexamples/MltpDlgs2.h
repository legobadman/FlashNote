#ifndef MLTPDLGS2_H
#define MLTPDLGS2_H

#include <QWidget>

class QPushButton;
class QLabel;

class MltpDlgs2 : public QWidget
{
	Q_OBJECT
public:
	explicit MltpDlgs2(QWidget* parent = nullptr);

private:
	QPushButton* clse = nullptr;
	QLabel* label = nullptr;
};

#endif // MLTPDLGS2_H