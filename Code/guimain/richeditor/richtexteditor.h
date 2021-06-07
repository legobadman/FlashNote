#ifndef __RICHTEXTEDITOR_H__
#define __RICHTEXTEDITOR_H__

#include "highlighter/cppsyntaxhighlighter.h"

class RichTextEditor : public QTextEdit
{
	Q_OBJECT
public:
	RichTextEditor(QWidget* parent = nullptr);
	void dropImage(const QUrl& url, const QImage& image);
	void dropTextFile(const QUrl& url);

protected:
	bool canInsertFromMimeData(const QMimeData* source) const;
	void insertFromMimeData(const QMimeData* source);
	QMimeData* createMimeDataFromSelection() const;

private:
	CppSyntaxHighlighter* highlighter;
};

#endif
