#ifndef __NORMAL_EDITOR_H__
#define __NORMAL_EDITOR_H__

#include "richeditor/richtexteditor.h"
#include "toolbutton.h"
#include "SeperateLine.h"
#include "alignmentcombobox.h"
#include "FontColorComboBox.h"
#include "font_combobox.h"
#include "fontsize_combobox.h"
#include "font_combobox.h"


class NormalEditor : public QWidget
{
	Q_OBJECT
public:
	NormalEditor(QWidget* parnet = NULL);
	void initContent(QString content, bool readOnly);
	QTextDocument* document();

Q_SIGNALS:
	void textChanged(bool delay);
	void currentCharFormatChanged(const QTextCharFormat& format);
	void cursorPositionChanged();

protected slots:
	void slotCurrentCharFormatChanged(const QTextCharFormat& format);
	void slotCursorPositionChanged();
	void textBold();
	void textUnderline();
	void textItalic();
	void textStrikeout();
	void addCodeBlock();
	void checkDocument();
	void textSize(const QString& p);
	void onFontChanged(const QString& font);
	void listOrdered();
	void listBullet();
	void textBgColor();
	void textFgColor(const QColor& color);
	void insertImage();
	void onEditting();
	void screenShot();

protected:
	void fontChanged(const QFont& f);
	void focusInEvent(QFocusEvent*);
	bool eventFilter(QObject* watched, QEvent* event);

private:
	void init();
	void initSlots();
	QHBoxLayout* initToolButtons();
	void list(bool checked, QTextListFormat::Style style);
	void mergeFormatOnWordOrSelection(const QTextCharFormat& format);

private:
	ToolButton* bold;
	ToolButton* italic;
	ToolButton* underline;
	ToolButton* strikeout;
	ToolButton* highlight;
	ToolButton* codeblock;
	SeperateVLine* vline;
	ToolButton* item_symbol;
	ToolButton* item_id;
	AlignmentComboBox* alignment2;
	SeperateVLine* vline2;
	ToolButton* attachment;
	ToolButton* photo;
	FontComboBox* font_comboBox;
	FontComboSizeBox* fontsize_comboBox;
	FontColorComboBox* fontcolor;

	RichTextEditor* textEdit;
};

#endif
