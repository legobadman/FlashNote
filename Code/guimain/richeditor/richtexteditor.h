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
	void updateFrameFormat();

public slots:
	void resizeImages();

protected:
	bool canInsertFromMimeData(const QMimeData* source) const;
	void insertFromMimeData(const QMimeData* source);
	QMimeData* createMimeDataFromSelection() const;
	void resizeEvent(QResizeEvent* e) override;
	void focusOutEvent(QFocusEvent* e) override;

private:
	CppSyntaxHighlighter* highlighter;
	const qreal mtabStopDist = 36;	//以Consola字体11号计算的tab大小。
};

#endif
