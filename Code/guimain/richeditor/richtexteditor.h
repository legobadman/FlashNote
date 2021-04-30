#ifndef __RICHTEXTEDITOR_H__
#define __RICHTEXTEDITOR_H__

#include <QPointer>

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
};

#endif
