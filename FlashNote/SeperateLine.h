#ifndef __SEPERATE_LINE_H__
#define __SEPERATE_LINE_H__

class SeperateHLine : public QFrame
{
public:
	SeperateHLine(QWidget* parent = nullptr);

protected:
	void paintEvent(QPaintEvent*) override;
};

class SeperateVLine : public QFrame
{
public:
	SeperateVLine(QWidget* parent = nullptr);

protected:
	void paintEvent(QPaintEvent*) override;
};

#endif
