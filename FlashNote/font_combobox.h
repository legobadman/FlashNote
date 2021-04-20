#ifndef __FONT_COMBOBOX_H__
#define __FONT_COMBOBOX_H__

class FontComboBox : public QFontComboBox
{
	Q_OBJECT
public:
	FontComboBox(QWidget* parent = nullptr);
	~FontComboBox();

protected:
	void paintEvent(QPaintEvent* event);

private:
	void init();
};

#endif
