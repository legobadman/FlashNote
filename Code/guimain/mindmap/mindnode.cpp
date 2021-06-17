#include "stdafx.h"
#include "mindnode.h"
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QInputDialog>
#include <QLineEdit>
#include <QTextFrame>


/////////////////////////////////////////////////////////////
MindTextNode::MindTextNode(const QString& text, MindTextNode* parent)
	: m_level(0)
	, myText(text)
	, m_mouseState(MS_FOCUSOUT)
	, m_bHovered(false)
	, m_borderWidth(2)
	, m_cornerRadius(7)
	, m_parent(parent)
	, m_progress(0)
	, m_counter(0)
	, m_bProgress(false)
{
}

MindTextNode::~MindTextNode()
{
}

void MindTextNode::setup()
{
	if (m_level == 0) {
		myTextColor = QColor(255, 255, 255);
		m_highlightedBorder = QColor(23, 157, 235);
		m_selectedBorder = QColor(23, 157, 235);
		myBackgroundColor = QColor(0, 181, 72);
		m_borderFocusout = QColor(myBackgroundColor);	//由于文本框的绘制策略，只能将同色的边框视为无边框。
	}
	else if (m_level == 1) {
		myTextColor = QColor(0, 0, 0);
		m_highlightedBorder = QColor(136, 203, 242);
		m_selectedBorder = QColor(23, 157, 235);
		myBackgroundColor = QColor(242, 242, 242);
		m_borderFocusout = QColor(myBackgroundColor);	//由于文本框的绘制策略，只能将同色的边框视为无边框。
	}
	else if (m_level == 2) {
		myTextColor = QColor(0, 0, 0);
		m_highlightedBorder = QColor(136, 203, 242);
		m_selectedBorder = QColor(23, 157, 235);
		myBackgroundColor = QColor(255, 255, 255);
		m_borderFocusout = QColor(myBackgroundColor);	//由于文本框的绘制策略，只能将同色的边框视为无边框。
	}

	init();

	//须初始化文档结构后，才能写入control
	if (m_bProgress)
	{
		myTextColor = QColor(0, 0, 0);	//由于有绿白色，需要将文本设置为黑色。
		QGraphicsTextItem::setProgress(m_progress);
	}
}

void MindTextNode::init()
{
	initDocFormat(myText);
	setFlags(ItemIsMovable | ItemSendsGeometryChanges | ItemIsSelectable);
	setTextInteractionFlags(Qt::TextEditorInteraction);

	QPalette pal = palette();
	//处理选中文本的情况。
	pal.setBrush(QPalette::Active, QPalette::Highlight, QColor(0, 129, 218));
	pal.setBrush(QPalette::Active, QPalette::HighlightedText, QColor(255, 255, 255));
	pal.setBrush(QPalette::Inactive, QPalette::Highlight, myBackgroundColor);
	pal.setBrush(QPalette::Inactive, QPalette::HighlightedText, myTextColor);

	setPalette(pal);
	setCornerRadius(m_cornerRadius);
}

void MindTextNode::onDocumentContentsChanged(int from, int charsRemoved, int charsAdded)
{
	if (m_counter == 0)
	{
		QGraphicsScene* pScene = scene();
		emit contentsChange();
	}
}

void MindTextNode::initDocFormat(const QString& text)
{
	QTextDocument* doc = document();

	QTextCursor cursor = textCursor();
	cursor.movePosition(QTextCursor::Start);

	QTextFrame::iterator it;
	QTextFrame* rootFrame = doc->rootFrame();
	for (it = rootFrame->begin(); !(it.atEnd()); ++it)
	{
		QTextFrame* childFrame = it.currentFrame();
		QTextBlock childBlock = it.currentBlock();
		if (childBlock.isValid())
		{
			QTextBlockFormat format = childBlock.blockFormat();
			format.setBackground(myBackgroundColor);
			format.setLeftMargin(10);
			format.setRightMargin(10);
			cursor.setBlockFormat(format);

			QTextCharFormat chrFormat = childBlock.charFormat();
			chrFormat.setFont(QFont(QString::fromUtf16((char16_t*)L"微软雅黑"), pointSize(m_level)));
			chrFormat.setForeground(myTextColor);
			cursor.setBlockCharFormat(chrFormat);
		}
	}
	cursor.insertText(text);

	QTextFrameFormat frameFormat = rootFrame->frameFormat();
	frameFormat.setBackground(myBackgroundColor);
	frameFormat.setMargin(0);
	frameFormat.setPadding(10);

	frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
	frameFormat.setBorderBrush(QColor(23, 157, 235));
	frameFormat.setBorder(m_borderWidth);
	rootFrame->setFrameFormat(frameFormat);

	connect(doc, SIGNAL(contentsChange(int,int,int)), this, SLOT(onDocumentContentsChanged(int, int, int)));
}

void MindTextNode::focusOutEvent(QFocusEvent* event)
{
	QGraphicsTextItem::focusOutEvent(event);
	clearSelection();
}

bool MindTextNode::sceneEvent(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::GraphicsSceneHoverEnter:
		m_bHovered = true;
		update();
		break;
	case QEvent::GraphicsSceneHoverLeave:
		m_bHovered = false;
		update();
		break;
	default:
		break;
	}
	return QGraphicsTextItem::sceneEvent(event);
}

void MindTextNode::SetContent(const std::wstring& content)
{
	myText = QString::fromStdWString(content);
}

std::wstring MindTextNode::GetContent() const
{
	return myText.toStdWString();
}

void MindTextNode::SetProgress(float progress)
{
	m_progress = progress;
	m_bProgress = true;
}

float MindTextNode::GetProgress() const
{
	return m_progress;
}

bool MindTextNode::IsProgress() const
{
	return m_bProgress;
}

void MindTextNode::append(MindTextNode* pNode)
{
	pNode->setParent(this);
	m_children.append(pNode);
}

void MindTextNode::udpateBorderFormat(const QStyleOptionGraphicsItem* option)
{
	UpdateBatch batch(&m_counter);
	QTextFrame* rootFrame = document()->rootFrame();
	QTextFrameFormat frameFormat = rootFrame->frameFormat();
	if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus))
	{
		if (m_mouseState == MS_SELECTED)
			return;
		frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
		frameFormat.setBorderBrush(m_selectedBorder);
		frameFormat.setBorder(m_borderWidth);
		m_mouseState = MS_SELECTED;
	}
	else if (m_bHovered)
	{
		if (m_mouseState == MS_HOVERED)
			return;
		frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
		frameFormat.setBorderBrush(m_highlightedBorder);
		frameFormat.setBorder(m_borderWidth);
		m_mouseState = MS_HOVERED;
	}
	else
	{
		//由于边框是画在背景边缘，因此为了隐藏边框需要设成背景的颜色。
		if (m_mouseState == MS_FOCUSOUT)
			return;
		frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
		frameFormat.setBorderBrush(m_borderFocusout);
		frameFormat.setBorder(m_borderWidth);
		m_mouseState = MS_FOCUSOUT;
	}
	rootFrame->setFrameFormat(frameFormat);
}

void MindTextNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	udpateBorderFormat(option);
	QGraphicsTextItem::paint(painter, option, widget);
}

int MindTextNode::pointSize(int level) const
{
	switch (level)
	{
	case 0: return 15;
	case 1: return 12;
	case 2: return 10;
	case 3: return 10;
	default:
		return 10;
	}
}