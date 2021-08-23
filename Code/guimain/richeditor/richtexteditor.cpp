#include "stdafx.h"
#include <QMimeData>
#include <QBuffer>
#include <QFileInfo>
#include <QList>
#include <QByteArray>
#include <QtGui/QImageReader>
#include <QDir>
#include "richtexteditor.h"
#include "pathservice.h"
#include "moc_richtexteditor.cpp"


RichTextEditor::RichTextEditor(QWidget* parent)
	: QTextEdit(parent)
	, highlighter(NULL)
{
	QString fontFamily = QString::fromUtf16((char16_t*)L"微软雅黑");
	QFont font(fontFamily, 11);
	setFont(font);

	setFrameShape(QFrame::NoFrame);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	QTextOption opt = document()->defaultTextOption();
	opt.setTabStopDistance(mtabStopDist);
	document()->setDefaultTextOption(opt);

	highlighter = new CppSyntaxHighlighter(document());
}

void RichTextEditor::updateFrameFormat()
{
	QTextFrame* rootFrame = document()->rootFrame();
	QTextFrameFormat format = rootFrame->frameFormat();
	format.setLeftMargin(45);
	format.setRightMargin(45);
	format.setTopMargin(10);
	format.setBottomMargin(10);
	rootFrame->setFrameFormat(format);
}

void RichTextEditor::resizeEvent(QResizeEvent* e)
{
	QTextEdit::resizeEvent(e);
	resizeImages();
}

void RichTextEditor::focusOutEvent(QFocusEvent* e)
{
	QTextEdit::focusOutEvent(e);
}

void RichTextEditor::resizeImages()
{
	QTextBlock currentBlock = textCursor().block();
	QTextBlock::iterator it;

	for (it = currentBlock.begin(); !(it.atEnd()); ++it)
	{
		QTextFragment fragment = it.fragment();
		if (fragment.isValid())
		{
			if (fragment.charFormat().isImageFormat())
			{
				QTextImageFormat newImageFormat = fragment.charFormat().toImageFormat();
				qreal editorWidth = newImageFormat.width();
				QString name = newImageFormat.name();
				QUrl url(name);
				QVariant varRes = document()->resource(QTextDocument::ImageResource, url);
				if (varRes.type() != QVariant::Image && varRes.type() != QVariant::Pixmap)
					continue;

				QImage image = varRes.value<QImage>();

				int W = viewport()->width() - 45 * 2;
				float ratio = 1;
				if (image.width() > W)
				{
					ratio = (float)image.width() / image.height();
				}
				else
				{
					continue;
				}
				newImageFormat.setWidth(W);
				newImageFormat.setHeight(W / ratio);
				if (newImageFormat.isValid())
				{
					QTextCursor helper = textCursor();

					helper.setPosition(fragment.position());
					helper.setPosition(fragment.position() + fragment.length(),
						QTextCursor::KeepAnchor);
					helper.setCharFormat(newImageFormat);
				}
			}
		}
	}
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

		QString assertPath = PathService::instance().GetAssetsPath();
		QDir dir(assertPath);
		//TODO: 暂不检测重复图片。
		QString fullpath = QString("%1/%2.%3").arg(assertPath).arg(prefix).arg(suffix);
		int i = 1;
		while (QFileInfo(fullpath).exists())
		{
			prefix += "_" + QString::number(i);
			fullpath = QString("%1/%2.%3").arg(assertPath).arg(prefix).arg(suffix);
		}

		bool bRet = image.save(fullpath, suffix.toUtf8(), 100);
		QUrl url_(fullpath);
		document()->addResource(QTextDocument::ImageResource, url_, image);
		QTextImageFormat imageFormat;
		imageFormat.setName(url_.toString());
		float W = viewport()->width() - 45 * 2;	//算上margin。
		float ratio = (float)image.width() / image.height();
		if (image.width() > W)
		{
			//ratio = (float)image.width() / image.height();
		}
		else
		{
			W = image.width();
		}
		imageFormat.setWidth(W);
		imageFormat.setHeight(W / ratio);
		textCursor().insertImage(imageFormat);
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
