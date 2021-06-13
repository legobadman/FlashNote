#include "stdafx.h"
#include "mindnode.h"
#include "mindlink.h"
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QInputDialog>
#include <QLineEdit>
#include <QTextFrame>


MindNode::MindNode(const QString& text)
	: m_level(0)
	, myText(text)
{
	myTextColor = QColor(255, 255, 255);
	myOutlineColor = QColor(0, 181, 72);
	myBackgroundColor = QColor(0, 181, 72);

	setFlags(ItemSendsGeometryChanges | ItemIsSelectable);
}

MindNode::~MindNode()
{
	foreach(MindLink * link, myLinks)
		delete link;
}

void MindNode::setText(const QString& text)
{
	prepareGeometryChange();
	myText = text;
	update();
}

QString MindNode::text() const
{
	return myText;
}

void MindNode::setTextColor(const QColor& color)
{
	myTextColor = color;
	update();
}

QColor MindNode::textColor() const
{
	return myTextColor;
}

void MindNode::setOutlineColor(const QColor& color)
{
	myOutlineColor = color;
	update();
}

QColor MindNode::outlineColor() const
{
	return myOutlineColor;
}

void MindNode::setBackgroundColor(const QColor& color)
{
	myBackgroundColor = color;
}

QColor MindNode::backgroundColor() const
{
	return myBackgroundColor;
}

void MindNode::addLink(MindLink* link)
{
	myLinks.insert(link);
}

void MindNode::removeLink(MindLink* link)
{
	myLinks.remove(link);
}

QRectF MindNode::boundingRect() const
{
	const int Margin = 1;
	return outlineRect().adjusted(-Margin, -Margin, Margin, Margin);
}

QPainterPath MindNode::shape() const
{
	QRectF rect = outlineRect();
	QPainterPath path;
	path.addRoundRect(rect, roundness(rect.width()), roundness(rect.height()));
	return path;
}

void MindNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QPen pen(myOutlineColor);
	if (option->state & QStyle::State_Selected) {
		pen.setStyle(Qt::SolidLine);
		pen.setWidth(2);
	}
	painter->setPen(pen);
	painter->setBrush(myBackgroundColor);

	QRectF rect = outlineRect();
	painter->drawRoundRect(rect, roundness(rect.width()), roundness(rect.height()));

	painter->setPen(myTextColor);
	QFont font(QString::fromUtf16((char16_t*)L"微软雅黑"), pointSize(m_level));
	painter->setFont(font);
	painter->drawText(rect, Qt::AlignCenter, myText);
}

void MindNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	QString text = QInputDialog::getText(event->widget(),
					tr("Edit Text"), tr("Enter new text:"),
					QLineEdit::Normal, myText);
	if (!text.isEmpty())
		setText(text);
}

QVariant MindNode::itemChange(GraphicsItemChange change, const QVariant& value)
{
	if (change == ItemPositionHasChanged)
	{
		foreach(MindLink * link, myLinks)
			link->trackNodes();
	}
	return QGraphicsItem::itemChange(change, value);
}

QRectF MindNode::outlineRect() const
{
	const int Padding = 8;
	QFont font(QString::fromUtf16((char16_t*)L"微软雅黑"), pointSize(m_level));
	QFontMetricsF metrics(font);
	QRectF rect = metrics.boundingRect(myText);
	rect.adjust(-Padding, -Padding, Padding, Padding);
	rect.translate(-rect.center());
	return rect;
}

int MindNode::roundness(double size) const
{
	const int Diameter = 12;
	return 100 * Diameter / int(size);
}

int MindNode::pointSize(int level) const
{
	switch (level)
	{
	case 0: return 15;
	case 1: return 14;
	case 2: return 12;
	case 3: return 10;
	default:
		return 9;
	}
}


/////////////////////////////////////////////////////////////
MindTextNode::MindTextNode(const QString& text)
	: m_level(0)
	, myText(text)
	, m_bHovered(false)
	, m_borderWidth(1)
	, m_cornerRadius(10)
{
	init();
}

MindTextNode::~MindTextNode()
{

}

void MindTextNode::init()
{
	myTextColor = QColor(255, 255, 255);
	myOutlineColor = QColor(0, 181, 72);
	myBackgroundColor = QColor(0, 181, 72);

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
	frameFormat.setPadding(15);

	frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
	frameFormat.setBorderBrush(QColor(23, 157, 235));
	frameFormat.setBorder(m_borderWidth);
	rootFrame->setFrameFormat(frameFormat);
}

bool MindTextNode::sceneEvent(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::GraphicsSceneHoverEnter: m_bHovered = true; break;
	case QEvent::GraphicsSceneHoverLeave: m_bHovered = false; break;
	default:
		break;
	}
	return QGraphicsTextItem::sceneEvent(event);
}

void MindTextNode::udpateBorderFormat(const QStyleOptionGraphicsItem* option)
{
	QTextFrame* rootFrame = document()->rootFrame();
	QTextFrameFormat frameFormat = rootFrame->frameFormat();
	if (option->state & (QStyle::State_Selected | QStyle::State_HasFocus))
	{
		frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
		frameFormat.setBorderBrush(QColor(23, 157, 235));
		frameFormat.setBorder(m_borderWidth);
	}
	else if (m_bHovered)
	{
		frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
		frameFormat.setBorderBrush(QColor(23, 157, 235));
		frameFormat.setBorder(m_borderWidth);
	}
	else
	{
		//由于边框是画在背景边缘，因此为了隐藏边框需要设成背景的颜色。
		frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
		frameFormat.setBorderBrush(myBackgroundColor);
		frameFormat.setBorder(m_borderWidth);
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
	case 1: return 14;
	case 2: return 12;
	case 3: return 10;
	default:
		return 9;
	}
}