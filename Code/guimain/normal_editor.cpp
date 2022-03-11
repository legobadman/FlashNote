#include "stdafx.h"
#include "common_types.h"
#include "normal_editor.h"
#include "MyStyle.h"
#include "screenshot/screenshotwindow.h"
#include <QtGui/QClipboard>
#include <QtGui/QTextDocumentWriter>
#include <QMimeData>
#include <QtGui/QFontDatabase>
#include <QInputDialog>
#include <QColorDialog>
#include <QtGui/QTextList>
#include <QtDebug>
#include <QFileDialog>
#include <QtGui/QImageReader>
#include <QSettings>
#include <QBuffer>
#include <QUrl>
#include <QPlainTextEdit>
#include <QWindow>
#include <QMenu>
#include <QDialog>
#include "guihelper.h"
#include "uiapplication.h"
#ifdef Q_OS_WIN
#include "notehook.h"
#endif
#include "floatingmenubutton.h"


NormalEditor::NormalEditor(QWidget* parent)
	: QWidget(parent)
	, bold(NULL)
	, italic(NULL)
    , underline(NULL)
	, strikeout(NULL)
	, highlight(NULL)
	, codeblock(NULL)
	, vline(NULL)
	, item_symbol(NULL)
	, item_id(NULL)
	, alignment2(NULL)
	, vline2(NULL)
	, attachment(NULL)
	, photo(NULL)
	, font_comboBox(NULL)
	, fontsize_comboBox(NULL)
	, fontcolor(NULL)
	, textEdit(NULL)
{
	init();
	initSlots();
}

void NormalEditor::initContent(QString contentUrl, bool readOnly)
{
	textEdit->blockSignals(true);
	textEdit->loadUrl(contentUrl);
	//textEdit->setReadOnly(readOnly);//donglm_temp_code
	//textEdit->updateFrameFormat();
	textEdit->blockSignals(false);
}

void NormalEditor::init()
{
	QVBoxLayout* pMainLayout = new QVBoxLayout;
	pMainLayout->setContentsMargins(0, 0, 0, 0);

	QHBoxLayout* pToolLayout = initToolButtons();
	pMainLayout->addLayout(pToolLayout);

	QFrame* frame2 = new QFrame;
	frame2->setMinimumSize(QSize(0, 0));
	frame2->setMaximumSize(QSize(16777215, 16777215));
	frame2->setBaseSize(QSize(10, 1));
	frame2->setFrameShape(QFrame::HLine);
	frame2->setFrameShadow(QFrame::Plain);
    QPalette palette = frame2->palette();
    palette.setColor(QPalette::WindowText, QColor(204, 204, 204));
	frame2->setPalette(palette);

	frame2->setLineWidth(1);
	pMainLayout->addWidget(frame2);

	QSize toolSz = pToolLayout->sizeHint();
	QSize fram2Sz = frame2->sizeHint();
	QRectF frameRc = frame2->frameRect();
	textEdit = new webWidget("https://www.baidu.com", this, QRectF(0, toolSz.height() + fram2Sz.height() +10, frameRc.width(), frameRc.height()));
	pMainLayout->addWidget(textEdit);

	setLayout(pMainLayout);

	textEdit->installEventFilter(this);
}

QHBoxLayout* NormalEditor::initToolButtons()
{
	QHBoxLayout* pLayout = new QHBoxLayout;
	pLayout->setContentsMargins(MyStyle::dpiScaled(8), 0, MyStyle::dpiScaled(8), 0);
	pLayout->setSpacing(1);

	qreal dpi = MyStyle::dpiScaled(1);
	
	font_comboBox = new FontComboBox;
	QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(font_comboBox->sizePolicy().hasHeightForWidth());
	font_comboBox->setSizePolicy(sizePolicy);
	font_comboBox->setFixedSize(MyStyle::dpiScaledSize(QSize(150, 28)));
	pLayout->addWidget(font_comboBox);

	fontsize_comboBox = new FontComboSizeBox;
	sizePolicy.setHeightForWidth(fontsize_comboBox->sizePolicy().hasHeightForWidth());
	fontsize_comboBox->setSizePolicy(sizePolicy);
	fontsize_comboBox->setFixedSize(MyStyle::dpiScaledSize(QSize(50, 28)));
	pLayout->addWidget(fontsize_comboBox);

	fontcolor = new FontColorComboBox;
	fontcolor->setFixedSize(MyStyle::dpiScaledSize(QSize(45, 30)));
	fontcolor->setIconSize(MyStyle::dpiScaledSize(QSize(24, 24)));
	pLayout->addWidget(fontcolor);

	bold = new ToolButton;
	bold->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	bold->setIcon(dpi < 1.5 ? QIcon(":/icons/16x16/Bold.png") : QIcon(":/icons/Bold.png"));
	bold->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	bold->setButtonStyle(ToolButton::ButtonIcon | ToolButton::ButtonCheckable);
	pLayout->addWidget(bold);

	italic = new ToolButton;
	italic->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	italic->setIcon(dpi < 1.5 ? QIcon(":/icons/16x16/Italic.png") : QIcon(":/icons/Italic.png"));
	italic->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	italic->setButtonStyle(ToolButton::ButtonIcon | ToolButton::ButtonCheckable);
	pLayout->addWidget(italic);

	underline = new ToolButton;
	underline->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	underline->setIcon(QIcon(":/icons/underline.png"));
	underline->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	underline->setButtonStyle(ToolButton::ButtonIcon | ToolButton::ButtonCheckable);
	pLayout->addWidget(underline);

	strikeout = new ToolButton;
	strikeout->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	strikeout->setIcon(dpi < 1.5 ? QIcon(":/icons/16x16/strikeout.png") : QIcon(":/icons/strikeout.png"));
	strikeout->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	strikeout->setButtonStyle(ToolButton::ButtonIcon | ToolButton::ButtonCheckable);
	pLayout->addWidget(strikeout);

	highlight = new ToolButton;
	highlight->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	highlight->setIcon(dpi < 1.5 ? QIcon(":/icons/16x16/highlighted.png") : QIcon(":/icons/highlighted.png"));
	highlight->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	highlight->setButtonStyle(ToolButton::ButtonIcon | ToolButton::ButtonCheckable);
	pLayout->addWidget(highlight);

	codeblock = new ToolButton;
	codeblock->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	codeblock->setIcon(dpi < 1.5 ? QIcon(":/icons/16x16/codeblock.png") : QIcon(":/icons/codeblock.png"));
	codeblock->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	pLayout->addWidget(codeblock);

	item_symbol = new ToolButton;
	item_symbol->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	item_symbol->setIcon(dpi < 1.5 ? QIcon(":/icons/16x16/item_symbol.png") : QIcon(":/icons/item_symbol.png"));
	item_symbol->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	item_symbol->setButtonStyle(ToolButton::ButtonIcon | ToolButton::ButtonCheckable);
	pLayout->addWidget(item_symbol);

	item_id = new ToolButton;
	item_id->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	item_id->setIcon(dpi < 1.5 ? QIcon(":/icons/16x16/item_id.png") : QIcon(":/icons/item_id.png"));
	item_id->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	item_id->setButtonStyle(ToolButton::ButtonIcon | ToolButton::ButtonCheckable);
	pLayout->addWidget(item_id);

	alignment2 = new AlignmentComboBox;
	pLayout->addWidget(alignment2);

	vline2 = new SeperateVLine;
	pLayout->addWidget(vline2);

	photo = new ToolButton;
	photo->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	photo->setIcon(QIcon(":/icons/camera.png"));
	photo->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	pLayout->addWidget(photo);

    attachment = new ToolButton;
    attachment->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
    attachment->setIcon(QIcon(":/icons/16x16/attachment.png"));
    attachment->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
    pLayout->addWidget(attachment);

	QSpacerItem* horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	pLayout->addItem(horizontalSpacer);

	return pLayout;
}

void NormalEditor::initSlots()
{
	connect(textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)), this, SLOT(slotCurrentCharFormatChanged(QTextCharFormat)));
	connect(textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(slotCursorPositionChanged()));
	connect(textEdit, SIGNAL(textChanged()), this, SLOT(onEditting()));

	fontChanged(textEdit->font());

	connect(fontsize_comboBox, SIGNAL(activated(QString)),
		this, SLOT(textSize(QString)));
	fontsize_comboBox->setCurrentIndex(fontsize_comboBox->findText(QString::number(QApplication::font()
		.pointSize())));

	connect(font_comboBox, SIGNAL(activated(QString)), this, SLOT(onFontChanged(QString)));
	connect(fontcolor, SIGNAL(colorChanged(const QColor&)), this, SLOT(textFgColor(const QColor&)));
	connect(highlight, SIGNAL(clicked()), this, SLOT(textBgColor()));

	connect(bold, SIGNAL(clicked()), this, SLOT(textBold()));
	connect(italic, SIGNAL(clicked()), this, SLOT(textItalic()));
	connect(underline, SIGNAL(clicked()), this, SLOT(textUnderline()));
	connect(strikeout, SIGNAL(clicked()), this, SLOT(textStrikeout()));
	connect(codeblock, SIGNAL(clicked()), this, SLOT(addCodeBlock()));

	connect(item_symbol, SIGNAL(clicked()), this, SLOT(listBullet()));
	connect(item_id, SIGNAL(clicked()), this, SLOT(listOrdered()));
	connect(photo, SIGNAL(clicked()), this, SLOT(screenShot()));
	if (attachment)
		connect(attachment, SIGNAL(clicked()), this, SLOT(checkDocument()));
}

void NormalEditor::slotCurrentCharFormatChanged(const QTextCharFormat& format)
{
	fontChanged(format.font());
	QColor color = (format.foreground().isOpaque()) ? format.foreground().color() : QColor();
	QColor bgColor = format.background().color();
	bool bHighlighted = format.background().isOpaque();
	fontcolor->updateColor(color);
	highlight->setChecked(bHighlighted);
	if (attachment)
		attachment->setChecked(format.isAnchor());
}

void NormalEditor::fontChanged(const QFont& f)
{
	font_comboBox->setCurrentIndex(font_comboBox->findText(f.family()));
	fontsize_comboBox->setCurrentIndex(fontsize_comboBox->findText(QString::number(f.pointSize())));
	bold->setChecked(f.bold());
	italic->setChecked(f.italic());
	underline->setChecked(f.underline());
	strikeout->setChecked(f.strikeOut());
	/*if (textEdit->textCursor().currentList()) {//donglm_temp_code
		QTextListFormat lfmt = textEdit->textCursor().currentList()->format();
		if (lfmt.style() == QTextListFormat::ListDisc) {
			item_symbol->setChecked(true);
			item_id->setChecked(false);
		}
		else if (lfmt.style() == QTextListFormat::ListDecimal) {
			item_symbol->setChecked(false);
			item_id->setChecked(true);
		}
		else {
			item_symbol->setChecked(false);
			item_id->setChecked(false);
		}
	}
	else {
		item_symbol->setChecked(false);
		item_id->setChecked(false);
	}*/
}

void NormalEditor::textBold()
{
	QTextCharFormat fmt;
	fmt.setFontWeight(bold->isChecked() ? QFont::Bold : QFont::Normal);
	mergeFormatOnWordOrSelection(fmt);
}

void NormalEditor::slotCursorPositionChanged()
{
	////QTextCursor cursor = textEdit->textCursor();//donglm_temp_code
	//QTextFrame* pFrame = cursor.currentFrame();

	//QTextList* l = cursor.currentList();
	//if (l)
	//{
	//	QTextListFormat lfmt = l->format();
	//	if (lfmt.style() == QTextListFormat::ListDisc)
	//	{
	//		item_symbol->setChecked(true);
	//		item_id->setChecked(false);
	//	}
	//	else if (lfmt.style() == QTextListFormat::ListDecimal)
	//	{
	//		item_symbol->setChecked(false);
	//		item_id->setChecked(true);
	//	}
	//	else
	//	{
	//		item_symbol->setChecked(false);
	//		item_id->setChecked(false);
	//	}
	//}
	//else
	//{
	//	item_symbol->setChecked(false);
	//	item_id->setChecked(false);
	//}*/
}

void NormalEditor::focusInEvent(QFocusEvent*)
{
	textEdit->setFocus(Qt::TabFocusReason);
}

void NormalEditor::resizeEvent(QResizeEvent* event)
{
	QSize sz = event->size();
	QSize oldSz = event->oldSize();
	QWidget::resizeEvent(event);
}

void NormalEditor::textUnderline()
{
	QTextCharFormat fmt;
	fmt.setFontUnderline(underline->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

void NormalEditor::textItalic()
{
	QTextCharFormat fmt;
	fmt.setFontItalic(italic->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

void NormalEditor::textStrikeout()
{
	QTextCharFormat fmt;
	fmt.setFontStrikeOut(strikeout->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

void NormalEditor::addCodeBlock()
{
	/*QTextFrameFormat frameFormat;//donglm_temp_code

	frameFormat.setBackground(QColor(30, 30, 30));
	frameFormat.setTopMargin(5);
	frameFormat.setBottomMargin(5);
	frameFormat.setPadding(15);

	QTextCursor cursor = textEdit->textCursor();
	QTextFrame* pTextFrame = cursor.insertFrame(frameFormat);
	QTextFrame::Iterator it = pTextFrame->begin();
	QTextBlock block = it.currentBlock();

	QTextBlockFormat format = block.blockFormat();
	cursor.setBlockFormat(format);

	QTextCharFormat charFormat = block.charFormat();
	charFormat.setForeground(QColor(213, 221, 227));
	QFont fontClr("Consolas", 12);
	charFormat.setFont(fontClr);
	cursor.setBlockCharFormat(charFormat);*/
}

void NormalEditor::checkDocument()
{
	//QTextDocument* p = textEdit->document();//donglm_temp_code

	//QTextCursor cursor = textEdit->textCursor();

	//QTextFrame::iterator it;
	//QTextFrame* rootFrame = p->rootFrame();
	//for (it = rootFrame->begin(); !(it.atEnd()); ++it)
	//{
	//	QTextFrame* childFrame = it.currentFrame();
	//	QTextBlock childBlock = it.currentBlock();
	//	if (childFrame)
	//	{
	//		QTextFrame::iterator it2;
	//		for (it2 = childFrame->begin(); !(it2.atEnd()); ++it2)
	//		{
	//			QTextFrame* childFrame2 = it2.currentFrame();
	//			QTextBlock block2 = it2.currentBlock();
	//			if (childFrame2)
	//			{

	//			}
	//			else if (block2.isValid())
	//			{
	//				QTextBlockFormat format = block2.blockFormat();
	//				//format.setLeftMargin(10);
	//				//format.setRightMargin(10);
	//				//format.setTopMargin(10);
	//				//format.setBottomMargin(10);
	//				//cursor.setBlockFormat(format);
	//			}
	//		}

	//		//reset
	//		if (false)
	//		{
	//			QTextFrameFormat frameFormat = childFrame->frameFormat();
	//			int wtf = frameFormat.padding();
	//			frameFormat.setPadding(10);
	//			//frameFormat.setMargin(10);
	//			childFrame->setFrameFormat(frameFormat);
	//		}
	//	}
	//	else if (childBlock.isValid())
	//	{

	//	}
	//}

	//QString html = p->toHtml();
	//QFile f("wtf.html");
	//f.open(QIODevice::WriteOnly);
	//f.write(html.toUtf8());
	//f.close();
}

void NormalEditor::textSize(const QString& p)
{
	qreal pointSize = p.toDouble();
	if (p.toFloat() > 0) {
		QTextCharFormat fmt;
		fmt.setFontPointSize(pointSize);
		mergeFormatOnWordOrSelection(fmt);
	}
}

void NormalEditor::onFontChanged(const QString& font)
{
	QTextCharFormat fmt;
	fmt.setFont(QFont(font));
	mergeFormatOnWordOrSelection(fmt);
}

void NormalEditor::textFgColor(const QColor& color)
{
	/*QTextCursor cursor = textEdit->textCursor();//donglm_temp_code
	QTextCharFormat fmt = cursor.charFormat();
	if (color.isValid()) {
		fmt.setForeground(color);
	}
	else {
		fmt.clearForeground();
	}
	cursor.setCharFormat(fmt);
	textEdit->setCurrentCharFormat(fmt);*/
}

void NormalEditor::textBgColor()
{
	/*QColor col = highlight->isChecked() ? QColor("#FFFAA5") : QColor("#FFFFFF");//donglm_temp_code
	QTextCursor cursor = textEdit->textCursor();
	QTextCharFormat fmt = cursor.charFormat();
	if (col.isValid()) {
		fmt.setBackground(col);
	}
	else {
		fmt.clearBackground();
	}
	cursor.setCharFormat(fmt);
	textEdit->setCurrentCharFormat(fmt);*/
}

void NormalEditor::listBullet()
{
	bool checked = item_symbol->isChecked();
	if (checked)
		item_id->setChecked(false);
	list(checked, QTextListFormat::ListDisc);
}

void NormalEditor::listOrdered()
{
	bool checked = item_id->isChecked();
	if (checked)
		item_symbol->setChecked(false);
	list(checked, QTextListFormat::ListDecimal);
}

void NormalEditor::list(bool checked, QTextListFormat::Style style)
{
	/*QTextCursor cursor = textEdit->textCursor();//donglm_temp_code
	cursor.beginEditBlock();
	if (!checked) {
		QTextBlockFormat obfmt = cursor.blockFormat();
		QTextBlockFormat bfmt;
		bfmt.setIndent(obfmt.indent());
		cursor.setBlockFormat(bfmt);
	}
	else {
		QTextListFormat listFmt;
		if (cursor.currentList()) {
			listFmt = cursor.currentList()->format();
		}
		listFmt.setStyle(style);
		cursor.createList(listFmt);
	}
	cursor.endEditBlock();*/
}

void NormalEditor::mergeFormatOnWordOrSelection(const QTextCharFormat& format)
{
	/*QTextCursor cursor = textEdit->textCursor();//donglm_temp_code
	cursor.mergeCharFormat(format);
	textEdit->mergeCurrentCharFormat(format);
	textEdit->setFocus(Qt::TabFocusReason);*/
}

void NormalEditor::screenShot()
{
	AppHelper::uiApp()->screenshot();
}

QTextDocument* NormalEditor::document()
{
	/*return textEdit->document();*///donglm_temp_code
	return nullptr;
}

void NormalEditor::insertImage()
{
	/*QString original = QFileDialog::getOpenFileName(this, tr("Select an image"),//donglm_temp_code
		".", "JPEG (*.jpg *jpeg)\n"
			"GIF (*.gif)\n"
			"PNG (*.png)\n"
			"Bitmap Files (*.bmp)\n");

	if (original.isEmpty())
		return;

	QImage image = QImageReader(original).read();
	textEdit->dropImage(QUrl(original), image);*/
}

bool NormalEditor::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == textEdit && event->type() == QEvent::FocusOut)
	{
		emit textChanged(false);
	}
	return QWidget::eventFilter(watched, event);
}

void NormalEditor::onEditting()
{
	emit textChanged(true);
}