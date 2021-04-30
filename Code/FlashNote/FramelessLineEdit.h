#ifndef __FRAMELESS_LINE_EDIT_H__
#define __FRAMELESS_LINE_EDIT_H__

#include <QLineEdit>

class FramelessLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	FramelessLineEdit(QWidget* parent = nullptr);
	~FramelessLineEdit();

protected:
	void initStyleOption(QStyleOptionFrame* option) const;
	void paintEvent(QPaintEvent*) override;
};

#endif
