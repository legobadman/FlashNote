#include "stdafx.h"
#include <QMimeData>
#include <QBuffer>
#include <QFileInfo>
#include <QList>
#include <QByteArray>
#include <QtGui/QImageReader>
#include <QDir>
#include "richtexteditor.h"
#include "moc_richtexteditor.cpp"


RichTextEditor::RichTextEditor(QWidget* parent)
	: QTextEdit(parent)
{
	QString fontFamily = QString::fromUtf16((char16_t*)L"微软雅黑");
	QFont font(fontFamily, 10);
	setFont(font);
	document()->setDocumentMargin(20);
	setFrameShape(QFrame::NoFrame);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void RichTextEditor::dropImage(const QUrl& url, const QImage& image)
{
	if (!image.isNull())
	{
		//需要复制图片到assets目录，并以assets目录下的图片作为url。
		QString strUrl = url.url();
		int idx = strUrl.lastIndexOf("/") + 1;

		QString filename = strUrl.mid(idx);
		idx = filename.lastIndexOf(".");
		QString prefix, suffix;
		if (idx >= 0)
		{
			prefix = filename.mid(0, idx);
			suffix = filename.mid(idx + 1);
		}
		else
		{
			prefix = filename;
			suffix = "jpg";
		}

		QString appPath = QCoreApplication::applicationDirPath();
		QString assertPath = appPath + "/" + "assets";

		QDir dir(assertPath);
		//TODO: 暂不检测重复图片。
		QString fullpath = QString("%1/%2.%3").arg(assertPath).arg(prefix).arg(suffix);
		int i = 1;
		while (QFileInfo(fullpath).exists())
		{
			prefix += "_" + QString::number(i);
			fullpath = QString("%1/%2.%3").arg(assertPath).arg(prefix).arg(suffix);
		}

		bool bRet = image.save(fullpath);
		QUrl url_(fullpath);
		document()->addResource(QTextDocument::ImageResource, url_, image);
		textCursor().insertImage(url_.toString());
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
