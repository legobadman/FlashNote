#ifndef __NOTE_EDIT_WINDOW_H__
#define __NOTE_EDIT_WINDOW_H__

#include "notecore.h"
#include "ui_note_editwindow.h"

namespace Ui
{
	class NoteEditWindow;
}

class NoteEditWindow : public QWidget
{
	Q_OBJECT
public:
	NoteEditWindow(QWidget* parent = nullptr);
	~NoteEditWindow();
	void init();
	void initCustomWidget();
	void initContent();
	INote* GetNote();
	bool isTitleEmpty() const;

	QString toPlainText() const { return m_ui->textEdit->toPlainText(); }
	QString toHtml() const;
	QTextDocument* document() { return m_ui->textEdit->document(); }
	QTextCursor textCursor() const { return m_ui->textEdit->textCursor(); }
	void setTextCursor(const QTextCursor& cursor) { m_ui->textEdit->setTextCursor(cursor); }

public slots:
	void setText(const QString& text);
	void clearSource();

protected slots:
	void setPlainText(const QString& text) { m_ui->textEdit->setPlainText(text); }
	void setHtml(const QString& text) { m_ui->textEdit->setHtml(text); }
	void textRemoveFormat();
	void textRemoveAllFormat();
	void textBold();
	void textUnderline();
	void textStrikeout();
	void textItalic();
	void textSize(const QString& p);
	void textLink(bool checked);
	void textStyle(int index);
	void textFgColor();
	void textBgColor();
	void listBullet(bool checked);
	void listOrdered(bool checked);
	void slotCurrentCharFormatChanged(const QTextCharFormat& format);
	void slotCursorPositionChanged();
	void slotClipboardDataChanged();
	void increaseIndentation();
	void decreaseIndentation();
	void insertImage();
	void textSource();

protected:
	void mergeFormatOnWordOrSelection(const QTextCharFormat& format);
	void fontChanged(const QFont& f);
	void fgColorChanged(const QColor& c);
	void bgColorChanged(const QColor& c);
	void list(bool checked, QTextListFormat::Style style);
	void indent(int delta);
	void focusInEvent(QFocusEvent* event);

private:
	void initSlots();

private:
	Ui::NoteEditWindow* m_ui;
	INote* m_pNote;

	QStringList m_paragraphItems;
	int m_fontsize_h1;
	int m_fontsize_h2;
	int m_fontsize_h3;
	int m_fontsize_h4;

	enum ParagraphItems {
		ParagraphStandard = 0,
		ParagraphHeading1,
		ParagraphHeading2,
		ParagraphHeading3,
		ParagraphHeading4,
		ParagraphMonospace
	};

	QPointer<QTextList> m_lastBlockList;
};

#endif
