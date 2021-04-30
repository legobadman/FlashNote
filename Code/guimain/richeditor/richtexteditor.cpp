#include "stdafx.h"
#include <QMimeData>
#include <QBuffer>
#include <QFileInfo>
#include <QList>
#include <QByteArray>
#include <QtGui/QImageReader>
#include "richtexteditor.h"
#include "moc_richtexteditor.cpp"


RichTextEditor::RichTextEditor(QWidget* parent /* = nullptr */)
	: QTextEdit(parent)
{
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void RichTextEditor::dropImage(const QUrl& url, const QImage& image)
{
	if (!image.isNull())
	{
		document()->addResource(QTextDocument::ImageResource, url, image);
		textCursor().insertImage(url.toString());
	}
}

void RichTextEditor::dropTextFile(const QUrl& url)
{
	QFile file(url.toLocalFile());
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		textCursor().insertText(file.readAll());
}

bool RichTextEditor::canInsertFromMimeData(const QMimeData* source) const
{
	return source->hasImage() || source->hasUrls() || QTextEdit::canInsertFromMimeData(source);
}

void RichTextEditor::insertFromMimeData(const QMimeData* source)
{
	if (source->hasImage())
	{
		static int i = 1;
		QUrl url(QString("dropped_image_%1").arg(i++));
		dropImage(url, qvariant_cast<QImage>(source->imageData()));
	}
	else if (source->hasUrls())
	{
		foreach(QUrl url, source->urls())
		{
			QFileInfo info(url.toLocalFile());
			if (QImageReader::supportedImageFormats().contains(info.suffix().toLower().toLatin1()))
				dropImage(url, QImage(info.filePath()));
			else
				dropTextFile(url);
		}
	}
	else
	{
		QTextEdit::insertFromMimeData(source);
	}
}

QMimeData* RichTextEditor::createMimeDataFromSelection() const
{
	return QTextEdit::createMimeDataFromSelection();
}
