#include "stdafx.h"

#include "note_editwindow.h"
#include "rpcservice.h"
#include "note_types.h"
#include "guihelper.h"
#include "moc_note_editwindow.cpp"
#include "MyStyle.h"
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
#include <QMenu>
#include <QDialog>


NoteEditWindow::NoteEditWindow(QWidget* parent)
    : QWidget(parent)
	, m_bEnableBase64(true)
	, m_bEdittable(false)
{
    init();
    initContent();
}

NoteEditWindow::~NoteEditWindow()
{
}

INote* NoteEditWindow::GetNote()
{
    return m_pNote;
}

void NoteEditWindow::updateNoteInfo(INoteCollection* pNoteCollection, INote* pNote)
{
	m_pNoteColl = pNoteCollection;
	m_pNote = pNote;

	com_sptr<INotebook> spNotebook;
	if (com_sptr<ITrash>(pNoteCollection))
	{
		m_bEdittable = false;
		BSTR bstrBookid;
		pNote->GetBookId(&bstrBookid);
		std::wstring bookid(bstrBookid);
		AppHelper::GetNotebookById(QString::fromStdWString(bstrBookid), &spNotebook);
	}
	else
	{
		m_bEdittable = true;
		spNotebook = pNoteCollection;
	}

	QString bookName = AppHelper::GetNotebookName(spNotebook);
	m_ui->bookmenu->blockSignals(true);
	m_ui->bookmenu->setText(bookName);
	m_ui->bookmenu->blockSignals(false);

	QString title = AppHelper::GetNoteTitle(m_pNote);
	QString content = AppHelper::GetNoteContent(m_pNote);

	m_ui->editTitle->blockSignals(true);
	m_ui->editTitle->setText(title);
	m_ui->editTitle->blockSignals(false);

	m_ui->textEdit->blockSignals(true);
	m_ui->textEdit->setText(content);
	m_ui->textEdit->setReadOnly(!m_bEdittable);
	m_ui->editTitle->setReadOnly(!m_bEdittable);
	m_ui->textEdit->blockSignals(false);
	
	update();
}

void NoteEditWindow::init()
{
    m_ui = new Ui::NoteEditWindow;
    QPalette palette(this->palette());
    palette.setColor(QPalette::Window, QColor(255, 255, 255));
    setAutoFillBackground(true);
    setPalette(palette);

    m_ui->setupUi(this);

    initCustomWidget();
	initSlots();
}

void NoteEditWindow::setBookName(const QString& name)
{
	m_ui->bookmenu->setText(name);
	update();
}

void NoteEditWindow::initSlots()
{
	m_lastBlockList = nullptr;

	connect(m_ui->editTitle, SIGNAL(textChanged(const QString&)), this, SLOT(onTitleChanged()));

	connect(m_ui->textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
		this, SLOT(slotCurrentCharFormatChanged(QTextCharFormat)));
	connect(m_ui->textEdit, SIGNAL(cursorPositionChanged()),
		this, SLOT(slotCursorPositionChanged()));
	connect(m_ui->textEdit, SIGNAL(textChanged()), this, SLOT(onTextChanged()));

	m_fontsize_h1 = 18;
	m_fontsize_h2 = 16;
	m_fontsize_h3 = 14;
	m_fontsize_h4 = 12;

	fontChanged(m_ui->textEdit->font());
	bgColorChanged(m_ui->textEdit->textColor());

	m_paragraphItems << tr("Standard")
		<< tr("Heading 1")
		<< tr("Heading 2")
		<< tr("Heading 3")
		<< tr("Heading 4")
		<< tr("Monospace");
	//f_paragraph->addItems(m_paragraphItems);
	//connect(f_paragraph, SIGNAL(activated(int)), this, SLOT(textStyle(int)));

	connect(m_ui->fontsize_comboBox, SIGNAL(activated(QString)),
		this, SLOT(textSize(QString)));
	m_ui->fontsize_comboBox->setCurrentIndex(m_ui->fontsize_comboBox->findText(QString::number(QApplication::font()
		.pointSize())));

	connect(m_ui->font_comboBox, SIGNAL(activated(QString)), this, SLOT(onFontChanged(QString)));

	connect(m_ui->fontcolor, SIGNAL(colorChanged(const QColor&)), this, SLOT(textFgColor(const QColor&)));
	connect(m_ui->highlight, SIGNAL(clicked()), this, SLOT(textBgColor()));

	//m_ui->attachment->setShortcut(Qt::CTRL + Qt::Key_L);
	connect(m_ui->attachment, SIGNAL(clicked(bool)), this, SLOT(textLink(bool)));

	//m_ui->bold->setShortcut(Qt::CTRL + Qt::Key_B);
	//m_ui->italic->setShortcut(Qt::CTRL + Qt::Key_I);
	//m_ui->underline->setShortcut(Qt::CTRL + Qt::Key_U);

	connect(m_ui->bold, SIGNAL(clicked()), this, SLOT(textBold()));
	connect(m_ui->italic, SIGNAL(clicked()), this, SLOT(textItalic()));
	connect(m_ui->underline, SIGNAL(clicked()), this, SLOT(textUnderline()));
	connect(m_ui->strikeout, SIGNAL(clicked()), this, SLOT(textStrikeout()));

	//m_ui->item_symbol->setShortcut(Qt::CTRL + Qt::Key_Minus);
	//m_ui->item_id->setShortcut(Qt::CTRL + Qt::Key_Equal);

	connect(m_ui->item_symbol, SIGNAL(clicked()), this, SLOT(listBullet()));
	connect(m_ui->item_id, SIGNAL(clicked()), this, SLOT(listOrdered()));
	connect(m_ui->photo, SIGNAL(clicked()), this, SLOT(insertImage()));
	connect(m_ui->attachment, SIGNAL(clicked()), this, SLOT(checkDocument()));
}

void NoteEditWindow::initCustomWidget()
{
	m_ui->font_comboBox->setFixedSize(MyStyle::dpiScaledSize(QSize(150, 28)));
	m_ui->fontsize_comboBox->setFixedSize(MyStyle::dpiScaledSize(QSize(50, 28)));

	m_ui->bookmenu->setFixedSize(MyStyle::dpiScaledSize(QSize(50, 22)));

	m_ui->bold->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->bold->setIcon(QIcon(":/icons/16x16/Bold.png"));
	m_ui->bold->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->bold->setButtonStyle(ToolButton::ButtonIcon | ToolButton::ButtonCheckable);
	m_ui->italic->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->italic->setIcon(QIcon(":/icons/16x16/Italic.png"));
	m_ui->italic->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->italic->setButtonStyle(ToolButton::ButtonIcon | ToolButton::ButtonCheckable);
	m_ui->underline->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->underline->setIcon(QIcon(":/icons/16x16/Underline.png"));
	m_ui->underline->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->underline->setButtonStyle(ToolButton::ButtonIcon | ToolButton::ButtonCheckable);
	m_ui->strikeout->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->strikeout->setIcon(QIcon(":/icons/16x16/strikeout.png"));
	m_ui->strikeout->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->strikeout->setButtonStyle(ToolButton::ButtonIcon | ToolButton::ButtonCheckable);
	m_ui->highlight->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->highlight->setIcon(QIcon(":/icons/16x16/highlighted.png"));
	m_ui->highlight->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->highlight->setButtonStyle(ToolButton::ButtonIcon | ToolButton::ButtonCheckable);
	m_ui->fontcolor->setFixedSize(MyStyle::dpiScaledSize(QSize(45, 30)));
	m_ui->fontcolor->setIconSize(MyStyle::dpiScaledSize(QSize(24, 24)));
	m_ui->codeblock->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->codeblock->setIcon(QIcon(":/icons/16x16/codeblock.png"));
	m_ui->codeblock->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->item_symbol->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->item_symbol->setIcon(QIcon(":/icons/16x16/item_symbol.png"));
	m_ui->item_symbol->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->item_symbol->setButtonStyle(ToolButton::ButtonIcon | ToolButton::ButtonCheckable);
	m_ui->item_id->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->item_id->setIcon(QIcon(":/icons/16x16/item_id.png"));
	m_ui->item_id->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->item_id->setButtonStyle(ToolButton::ButtonIcon | ToolButton::ButtonCheckable);
	m_ui->attachment->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->attachment->setIcon(QIcon(":/icons/16x16/attachment.png"));
	m_ui->attachment->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
	m_ui->photo->setFixedSize(MyStyle::dpiScaledSize(QSize(30, 30)));
	m_ui->photo->setIcon(QIcon(":/icons/16x16/photo.png"));
	m_ui->photo->setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
}

void NoteEditWindow::initContent()
{
    m_ui->editTitle->setPlaceholderText(u8"标题");
}

void NoteEditWindow::textSource() {
	QDialog* dialog = new QDialog(this);
	QPlainTextEdit* pte = new QPlainTextEdit(dialog);
	pte->setPlainText(m_ui->textEdit->toHtml());
	QGridLayout* gl = new QGridLayout(dialog);
	gl->addWidget(pte, 0, 0, 1, 1);
	dialog->setWindowTitle(tr("Document source"));
	dialog->setMinimumWidth(400);
	dialog->setMinimumHeight(600);
	dialog->exec();

	m_ui->textEdit->setHtml(pte->toPlainText());

	delete dialog;
}

void NoteEditWindow::clearSource() {
	m_ui->textEdit->clear();
}


void NoteEditWindow::textRemoveFormat() {
	QTextCharFormat fmt;
	fmt.setFontWeight(QFont::Normal);
	fmt.setFontUnderline(false);
	fmt.setFontStrikeOut(false);
	fmt.setFontItalic(false);
	fmt.setFontPointSize(9);
	//  fmt.setFontFamily     ("Helvetica");
	//  fmt.setFontStyleHint  (QFont::SansSerif);
	//  fmt.setFontFixedPitch (true);

	m_ui->bold->setChecked(false);
	m_ui->underline->setChecked(false);
	m_ui->italic->setChecked(false);

	m_ui->fontsize_comboBox->setCurrentIndex(m_ui->fontsize_comboBox->findText("9"));

	//  QTextBlockFormat bfmt = cursor.blockFormat();
	//  bfmt->setIndent(0);

	fmt.clearBackground();

	mergeFormatOnWordOrSelection(fmt);
}


void NoteEditWindow::textRemoveAllFormat() {
	m_ui->bold->setChecked(false);
	m_ui->underline->setChecked(false);
	m_ui->italic->setChecked(false);
	m_ui->strikeout->setChecked(false);
	m_ui->fontsize_comboBox->setCurrentIndex(m_ui->fontsize_comboBox->findText("9"));
	QString text = m_ui->textEdit->toPlainText();
	m_ui->textEdit->setPlainText(text);
}


void NoteEditWindow::textBold() {
	QTextCharFormat fmt;
	fmt.setFontWeight(m_ui->bold->isChecked() ? QFont::Bold : QFont::Normal);
	mergeFormatOnWordOrSelection(fmt);
}


void NoteEditWindow::focusInEvent(QFocusEvent*) {
	m_ui->textEdit->setFocus(Qt::TabFocusReason);
}


void NoteEditWindow::textUnderline() {
	QTextCharFormat fmt;
	fmt.setFontUnderline(m_ui->underline->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

void NoteEditWindow::textItalic() {
	QTextCharFormat fmt;
	fmt.setFontItalic(m_ui->italic->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

void NoteEditWindow::textStrikeout() {
	QTextCharFormat fmt;
	fmt.setFontStrikeOut(m_ui->strikeout->isChecked());
	mergeFormatOnWordOrSelection(fmt);
}

void NoteEditWindow::textSize(const QString& p) {
	qreal pointSize = p.toDouble();
	if (p.toFloat() > 0) {
		QTextCharFormat fmt;
		fmt.setFontPointSize(pointSize);
		mergeFormatOnWordOrSelection(fmt);
	}
}

void NoteEditWindow::onFontChanged(const QString& font)
{
	QTextCharFormat fmt;
	fmt.setFont(QFont(font));
	mergeFormatOnWordOrSelection(fmt);
}

void NoteEditWindow::textLink(bool checked) {
	bool unlink = false;
	QTextCharFormat fmt;
	if (checked) {
		QString url = m_ui->textEdit->currentCharFormat().anchorHref();
		bool ok;
		QString newUrl = QInputDialog::getText(this, tr("Create a link"),
			tr("Link URL:"), QLineEdit::Normal,
			url,
			&ok);
		if (ok) {
			fmt.setAnchor(true);
			fmt.setAnchorHref(newUrl);
			fmt.setForeground(QApplication::palette().color(QPalette::Link));
			fmt.setFontUnderline(true);
		}
		else {
			unlink = true;
		}
	}
	else {
		unlink = true;
	}
	if (unlink) {
		fmt.setAnchor(false);
		fmt.setForeground(QApplication::palette().color(QPalette::Text));
		fmt.setFontUnderline(false);
	}
	mergeFormatOnWordOrSelection(fmt);
}

void NoteEditWindow::textStyle(int index) {
	QTextCursor cursor = m_ui->textEdit->textCursor();
	cursor.beginEditBlock();

	// standard
	if (!cursor.hasSelection()) {
		cursor.select(QTextCursor::BlockUnderCursor);
	}
	QTextCharFormat fmt;
	cursor.setCharFormat(fmt);
	m_ui->textEdit->setCurrentCharFormat(fmt);

	if (index == ParagraphHeading1
		|| index == ParagraphHeading2
		|| index == ParagraphHeading3
		|| index == ParagraphHeading4) {
		if (index == ParagraphHeading1) {
			fmt.setFontPointSize(m_fontsize_h1);
		}
		if (index == ParagraphHeading2) {
			fmt.setFontPointSize(m_fontsize_h2);
		}
		if (index == ParagraphHeading3) {
			fmt.setFontPointSize(m_fontsize_h3);
		}
		if (index == ParagraphHeading4) {
			fmt.setFontPointSize(m_fontsize_h4);
		}
		if (index == ParagraphHeading2 || index == ParagraphHeading4) {
			fmt.setFontItalic(true);
		}

		fmt.setFontWeight(QFont::Bold);
	}
	if (index == ParagraphMonospace) {
		fmt = cursor.charFormat();
		fmt.setFontFamily("Monospace");
		fmt.setFontStyleHint(QFont::Monospace);
		fmt.setFontFixedPitch(true);
	}
	cursor.setCharFormat(fmt);
	m_ui->textEdit->setCurrentCharFormat(fmt);

	cursor.endEditBlock();
}

void NoteEditWindow::textFgColor(const QColor& color)
{
	QTextCursor cursor = m_ui->textEdit->textCursor();
	QTextCharFormat fmt = cursor.charFormat();
	if (color.isValid()) {
		fmt.setForeground(color);
	}
	else {
		fmt.clearForeground();
	}
	cursor.setCharFormat(fmt);
	m_ui->textEdit->setCurrentCharFormat(fmt);
	fgColorChanged(color);
}

void NoteEditWindow::textBgColor() {
	QColor col = m_ui->highlight->isChecked() ? QColor("#FFFAA5") : QColor("#FFFFFF");
	QTextCursor cursor = m_ui->textEdit->textCursor();
	QTextCharFormat fmt = cursor.charFormat();
	if (col.isValid()) {
		fmt.setBackground(col);
	}
	else {
		fmt.clearBackground();
	}
	cursor.setCharFormat(fmt);
	m_ui->textEdit->setCurrentCharFormat(fmt);
}

void NoteEditWindow::listBullet()
{
	bool checked = m_ui->item_symbol->isChecked();
	if (checked)
		m_ui->item_id->setChecked(false);
	list(checked, QTextListFormat::ListDisc);
}

void NoteEditWindow::listOrdered()
{
	bool checked = m_ui->item_id->isChecked();
	if (checked)
		m_ui->item_symbol->setChecked(false);
	list(checked, QTextListFormat::ListDecimal);
}

void NoteEditWindow::list(bool checked, QTextListFormat::Style style) {
	QTextCursor cursor = m_ui->textEdit->textCursor();
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
	cursor.endEditBlock();
}

void NoteEditWindow::mergeFormatOnWordOrSelection(const QTextCharFormat& format) {
	QTextCursor cursor = m_ui->textEdit->textCursor();
	cursor.mergeCharFormat(format);
	m_ui->textEdit->mergeCurrentCharFormat(format);
	m_ui->textEdit->setFocus(Qt::TabFocusReason);
}

void NoteEditWindow::slotCursorPositionChanged() {
	QTextList* l = m_ui->textEdit->textCursor().currentList();
	//if (m_lastBlockList && (l == m_lastBlockList || (l != nullptr && m_lastBlockList != nullptr
	//	&& l->format().style() == m_lastBlockList->format().style()))) {
	//	return;
	//}
	//m_lastBlockList = l;
	if (l)
	{
		QTextListFormat lfmt = l->format();
		if (lfmt.style() == QTextListFormat::ListDisc)
		{
			m_ui->item_symbol->setChecked(true);
			m_ui->item_id->setChecked(false);
		}
		else if (lfmt.style() == QTextListFormat::ListDecimal)
		{
			m_ui->item_symbol->setChecked(false);
			m_ui->item_id->setChecked(true);
		}
		else
		{
			m_ui->item_symbol->setChecked(false);
			m_ui->item_id->setChecked(false);
		}
	}
	else
	{
		m_ui->item_symbol->setChecked(false);
		m_ui->item_id->setChecked(false);
	}
}

void NoteEditWindow::fontChanged(const QFont& f) {
	m_ui->font_comboBox->setCurrentIndex(m_ui->font_comboBox->findText(f.family()));
	m_ui->fontsize_comboBox->setCurrentIndex(m_ui->fontsize_comboBox->findText(QString::number(f.pointSize())));
	m_ui->bold->setChecked(f.bold());
	m_ui->italic->setChecked(f.italic());
	m_ui->underline->setChecked(f.underline());
	m_ui->strikeout->setChecked(f.strikeOut());
	if (m_ui->textEdit->textCursor().currentList()) {
		QTextListFormat lfmt = m_ui->textEdit->textCursor().currentList()->format();
		if (lfmt.style() == QTextListFormat::ListDisc) {
			m_ui->item_symbol->setChecked(true);
			m_ui->item_id->setChecked(false);
		}
		else if (lfmt.style() == QTextListFormat::ListDecimal) {
			m_ui->item_symbol->setChecked(false);
			m_ui->item_id->setChecked(true);
		}
		else {
			m_ui->item_symbol->setChecked(false);
			m_ui->item_id->setChecked(false);
		}
	}
	else {
		m_ui->item_symbol->setChecked(false);
		m_ui->item_id->setChecked(false);
	}
}

void NoteEditWindow::fgColorChanged(const QColor& c) {
	QPixmap pix(16, 16);
	if (c.isValid()) {
		pix.fill(c);
	}
	else {
		pix.fill(QApplication::palette().windowText().color());
	}
}

void NoteEditWindow::bgColorChanged(const QColor& c) {
	QPixmap pix(16, 16);
	if (c.isValid()) {
		pix.fill(c);
	}
	else {
		pix.fill(QApplication::palette().window().color());
	}
}

void NoteEditWindow::slotCurrentCharFormatChanged(const QTextCharFormat& format) {
	fontChanged(format.font());
	QColor color = (format.foreground().isOpaque()) ? format.foreground().color() : QColor();
	QColor bgColor = format.background().color();
	bool bHighlighted = format.background().isOpaque();
	m_ui->fontcolor->updateColor(color);
	m_ui->highlight->setChecked(bHighlighted);
	m_ui->attachment->setChecked(format.isAnchor());
}

void NoteEditWindow::slotClipboardDataChanged() {
#ifndef QT_NO_CLIPBOARD
	//if (const QMimeData* md = QApplication::clipboard()->mimeData())
	//	f_paste->setEnabled(md->hasText());
#endif
}

QString NoteEditWindow::toHtml() const {
	QString s = m_ui->textEdit->toHtml();
	// convert emails to links
	s = s.replace(QRegExp("(<[^a][^>]+>(?:<span[^>]+>)?|\\s)([a-zA-Z\\d]+@[a-zA-Z\\d]+\\.[a-zA-Z]+)"), "\\1<a href=\"mailto:\\2\">\\2</a>");
	// convert links
	s = s.replace(QRegExp("(<[^a][^>]+>(?:<span[^>]+>)?|\\s)((?:https?|ftp|file)://[^\\s'\"<>]+)"), "\\1<a href=\"\\2\">\\2</a>");
	// see also: Utils::linkify()
	return s;
}

void NoteEditWindow::saveNote()
{
	Q_ASSERT(m_pNote);
	QTextDocument* p = document();
	QString title = m_ui->editTitle->text();
	QString html = p->toHtml();
	QString plainText = p->toPlainText();

	BSTR bstrTitle = SysAllocString(title.toStdWString().c_str());
	m_pNote->SetTitle(bstrTitle);
	BSTR bstrContent = SysAllocString(html.toStdWString().c_str());
	m_pNote->SetContent(bstrContent);
	BSTR bstrPlainText = SysAllocString(plainText.toStdWString().c_str());
	m_pNote->SetPlainText(bstrPlainText);

	com_sptr<INotebook> spNotebook = m_pNoteColl;
	RPCService::GetInstance().SynchronizeNote(spNotebook, m_pNote);
}

void NoteEditWindow::switchtobook(int bookidx)
{
	//TODO
	VARIANT newindex;
	V_VT(&newindex) = VT_I4;
	V_I4(&newindex) = bookidx;
	com_sptr<INotebook> spNewbook;
	com_sptr<INotebooks> spNotebooks;
	coreApp->GetNotebooks(&spNotebooks);
	HRESULT hr = spNotebooks->Item(newindex, &spNewbook);
	if (FAILED(hr) || !spNewbook)
	{
		Q_ASSERT(FALSE);
	}

	hr = m_pNoteColl->RemoveNote(m_pNote);
	if (FAILED(hr))
		Q_ASSERT(FALSE);
	hr = spNewbook->AddNote(m_pNote);
	if (FAILED(hr))
		Q_ASSERT(FALSE);
	m_pNoteColl = spNewbook;
}

void NoteEditWindow::onTitleChanged()
{
	QTimer::singleShot(2000, this, SLOT(saveNote()));
}

void NoteEditWindow::onTextChanged()
{
	QTimer::singleShot(2000, this, SLOT(saveNote()));
}

void NoteEditWindow::increaseIndentation()
{
	indent(+1);
}

void NoteEditWindow::decreaseIndentation()
{
	indent(-1);
}

void NoteEditWindow::indent(int delta)
{
	QTextCursor cursor = m_ui->textEdit->textCursor();
	cursor.beginEditBlock();
	QTextBlockFormat bfmt = cursor.blockFormat();
	int ind = bfmt.indent();
	if (ind + delta >= 0) {
		bfmt.setIndent(ind + delta);
	}
	cursor.setBlockFormat(bfmt);
	cursor.endEditBlock();
}

void NoteEditWindow::setText(const QString& text)
{
	if (text.isEmpty()) {
		setPlainText(text);
		return;
	}
	if (text[0] == '<') {
		setHtml(text);
	}
	else {
		setPlainText(text);
	}
}

void NoteEditWindow::checkDocument()
{
	QTextDocument* p = document();
    QString html = p->toHtml();
    QFile f("wtf.html");
    f.open(QIODevice::WriteOnly);
    f.write(html.toUtf8());
    f.close();
}

#define USE_BASE_64

void NoteEditWindow::insertImage()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Select an image"),
		".", tr("JPEG (*.jpg *jpeg)\n"
			"GIF (*.gif)\n"
			"PNG (*.png)\n"
			"Bitmap Files (*.bmp)\n"));
	if (file.isEmpty())
		return;
	QUrl Uri(QString("file://%1").arg(file));
	QImage image = QImageReader(file).read();

	QTextDocument* textDocument = m_ui->textEdit->document();

	if (m_bEnableBase64)
	{
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        image.save(&buffer, "PNG");
        Uri = QString("data:image/png;base64,") + byteArray.toBase64();
        //Uri = QString("<img src=\"data:image/png;base64,") + byteArray.toBase64() + "\"/>";
        textDocument->addResource(QTextDocument::ImageResource, Uri, QVariant(image));
	}
	else
	{
		textDocument->addResource(QTextDocument::ImageResource, Uri, QVariant(image));
	}

	QTextCursor cursor = m_ui->textEdit->textCursor();
	QTextImageFormat imageFormat;

	//调整图片的宽度。
	//TODO: resize的时候自动调宽
	int w = m_ui->textEdit->width();
	float ratio = (float)image.width() / image.height();
	int h = w / ratio;

	imageFormat.setWidth(w);
	imageFormat.setHeight(h);
	imageFormat.setName(Uri.toString());
	cursor.insertImage(imageFormat);
}