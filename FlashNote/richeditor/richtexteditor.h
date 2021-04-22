#ifndef __RICHTEXTEDITOR_H__
#define __RICHTEXTEDITOR_H__

#include <QPointer>

class RichTextEditor : public QTextEdit
{
	Q_OBJECT
public:
	RichTextEditor(QWidget* parent = nullptr);
	void dropImage(const QImage& image, const QString& format);

protected:
	bool canInsertFromMimeData(const QMimeData* source) const;
	void insertFromMimeData(const QMimeData* source);
	QMimeData* createMimeDataFromSelection() const;
};

#endif
