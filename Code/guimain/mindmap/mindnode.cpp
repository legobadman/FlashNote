#include "stdafx.h"
#include "mindnode.h"
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMenu>
#include <QLineEdit>
#include <QTextFrame>


/////////////////////////////////////////////////////////////
MindTextNode::MindTextNode(const QString& text, MindTextNode* parent)
	: m_level(0)
	, myText(text)
	, m_mouseState(MS_UNKNOWN)
	, m_bHovered(false)
	, m_borderWidth(2)
	, m_cornerRadius(7)
	, m_parent(parent)
	, m_progress(0)
	, m_counter(0)
	, m_bProgress(false)
	, m_pBtn(NULL)
{
}

MindTextNode::~MindTextNode()
{
}

void MindTextNode::setup()
{
	// setup��Ҫ�����һ���������ܻ�ȡ������level�Լ��Ƿ������������
	if (m_level == 0)
	{
		if (m_bProgress)
		{
			myTextColor = QColor(0, 0, 0);
			myBackgroundColor = QColor(242, 242, 242);
		}
		else
		{
			myTextColor = QColor(255, 255, 255);
			myBackgroundColor = QColor(0, 181, 72);
		}
		m_highlightedBorder = QColor(23, 157, 235);
		m_selectedBorder = QColor(23, 157, 235);
		m_borderFocusout = QColor(myBackgroundColor);	//�����ı���Ļ��Ʋ��ԣ�ֻ�ܽ�ͬɫ�ı߿���Ϊ�ޱ߿�
	}
	else if (m_level == 1) {
		myTextColor = QColor(0, 0, 0);
		m_highlightedBorder = QColor(136, 203, 242);
		m_selectedBorder = QColor(23, 157, 235);
		myBackgroundColor = QColor(242, 242, 242);
		m_borderFocusout = QColor(myBackgroundColor);	//�����ı���Ļ��Ʋ��ԣ�ֻ�ܽ�ͬɫ�ı߿���Ϊ�ޱ߿�
	}
	else if (m_level >= 2) {
		myTextColor = QColor(0, 0, 0);
		m_highlightedBorder = QColor(136, 203, 242);
		m_selectedBorder = QColor(23, 157, 235);
		myBackgroundColor = QColor(255, 255, 255);
		m_borderFocusout = QColor(myBackgroundColor);	//�����ı���Ļ��Ʋ��ԣ�ֻ�ܽ�ͬɫ�ı߿���Ϊ�ޱ߿�
	}

	init();

	//���ʼ���ĵ��ṹ�󣬲���д��control
	if (m_bProgress)
	{
		myTextColor = QColor(0, 0, 0);	//�������̰�ɫ����Ҫ���ı�����Ϊ��ɫ��
		QGraphicsTextItem::setProgress(m_progress);
	}
}

void MindTextNode::init()
{
	initDocFormat(myText);
	setFlags(ItemIsMovable | ItemSendsGeometryChanges | ItemIsSelectable);

	QPalette pal = palette();
	//����ѡ���ı��������
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
		myText = document()->toPlainText();
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
			chrFormat.setFont(QFont(QString::fromUtf16((char16_t*)L"΢���ź�"), pointSize(m_level)));
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
	setTextInteractionFlags(Qt::NoTextInteraction);
}

void MindTextNode::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsTextItem::mousePressEvent(event);
}

void MindTextNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void MindTextNode::onCreateChildNode()
{
	emit childNodeCreate(this);
}

void MindTextNode::onCreateSliblingNode()
{
	emit silibingNodeCreate(this);
}

void MindTextNode::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		QMenu* menu = new QMenu(NULL);
		menu->addAction(QString(u8"�����Ӽ��ڵ�"), this, SLOT(onCreateChildNode()));
		if (m_parent != NULL)
			menu->addAction(QString(u8"����ͬ���ڵ�"), this, SLOT(onCreateSliblingNode()));

		QGraphicsView* v = scene()->views().first();
		QPointF sceneP = mapToScene(event->pos());
		QPoint viewP = v->mapFromScene(sceneP);
		QPoint sendMenuEventPos = v->viewport()->mapToGlobal(viewP);
		menu->popup(sendMenuEventPos);
	}
	QGraphicsTextItem::mouseReleaseEvent(event);
}

bool MindTextNode::sceneEvent(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::GraphicsSceneHoverEnter:
		//��ť�����Ѿ��ھ��������ˣ�����ҪһЩ�жϡ�
		m_bHovered = true;
		if (m_pBtn)
			m_pBtn->setVisible(true);
		update();
		break;
	case QEvent::GraphicsSceneHoverLeave:
		m_bHovered = false;
		if (m_pBtn)
			m_pBtn->setVisible(false);
		update();
		break;
	case QEvent::GraphicsSceneMouseDoubleClick:
		{
			setTextInteractionFlags(Qt::TextEditorInteraction);
			bool ret = QGraphicsTextItem::sceneEvent(event);
			setFocus(Qt::MouseFocusReason);
			return ret;
		}
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
		//���ڱ߿��ǻ��ڱ�����Ե�����Ϊ�����ر߿���Ҫ��ɱ�������ɫ��
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