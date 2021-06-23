#include "stdafx.h"
#include "mindnode.h"
#include "guihelper.h"
#include "newnotewindow.h"
#include "selectnotebookdlg.h"
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMenu>
#include <QLineEdit>
#include <QTextFrame>


/////////////////////////////////////////////////////////////
MindNode::MindNode(const QString& text, MindNode* parent)
	: m_level(0)
	, m_content(text)
	, m_mouseState(MS_UNKNOWN)
	, m_bHovered(false)
	, m_bToRight(true)
	, m_borderWidth(2)
	, m_cornerRadius(7)
	, m_parent(parent)
	, m_counter(0)
	, m_pBtn(NULL)
	, m_pMenu(NULL)
{
}

MindNode::~MindNode()
{
}

void MindNode::setup()
{
	// setup需要在最后一步做，才能获取真正的level以及是否包含进度条。
	if (m_level == 0)
	{
		m_textColor = QColor(255, 255, 255);
		m_backgroudColor = QColor(0, 181, 72);
		m_highlightedBorder = QColor(23, 157, 235);
		m_selectedBorder = QColor(23, 157, 235);
		m_borderFocusout = QColor(m_backgroudColor);	//由于文本框的绘制策略，只能将同色的边框视为无边框。
	}
	else if (m_level == 1) {
		m_textColor = QColor(0, 0, 0);
		m_highlightedBorder = QColor(136, 203, 242);
		m_selectedBorder = QColor(23, 157, 235);
		m_backgroudColor = QColor(242, 242, 242);
		m_borderFocusout = QColor(m_backgroudColor);	//由于文本框的绘制策略，只能将同色的边框视为无边框。
	}
	else if (m_level >= 2) {
		m_textColor = QColor(0, 0, 0);
		m_highlightedBorder = QColor(136, 203, 242);
		m_selectedBorder = QColor(23, 157, 235);
		m_backgroudColor = QColor(255, 255, 255);
		m_borderFocusout = QColor(m_backgroudColor);	//由于文本框的绘制策略，只能将同色的边框视为无边框。
	}

	init();
}

void MindNode::init()
{
	initDocFormat(m_content);
	setFlags(ItemIsMovable | ItemSendsGeometryChanges | ItemIsSelectable);

	QPalette pal = palette();
	//处理选中文本的情况。
	pal.setBrush(QPalette::Active, QPalette::Highlight, QColor(0, 129, 218));
	pal.setBrush(QPalette::Active, QPalette::HighlightedText, QColor(255, 255, 255));
	pal.setBrush(QPalette::Inactive, QPalette::Highlight, m_backgroudColor);
	pal.setBrush(QPalette::Inactive, QPalette::HighlightedText, m_textColor);

	setPalette(pal);
	setCornerRadius(m_cornerRadius);

	initDecoration();
	initDirection();
	initMenu();
}

void MindNode::initDirection()
{
	if (m_level > 1)
	{
		MindNode* p = m_parent;
		while (p->level() != 1)
			p = p->Parent();
		Q_ASSERT(p);
		setToRight(p->isToRight());
	}
}

void MindNode::initDecoration()
{
	if (needShowDecoration())
	{
		if (m_level == 0)
			setDecoration(2, QIcon(":/icons/16x16/link_note_white.png"));
		else
			setDecoration(2, QIcon(":/icons/16x16/link_note_black.png"));
	}
}

void MindNode::initMenu()
{
	m_pMenu = new QMenu(NULL);

	if (m_level == 0)
	{
		m_pMenu->addAction(QString(u8"增加子级节点(右)"), this, SLOT(onCreateChildNodeRight()));
		m_pMenu->addAction(QString(u8"增加子级节点(左)"), this, SLOT(onCreateChildNodeLeft()));
	}
	else
	{
		if (isToRight())
			m_pMenu->addAction(QString(u8"增加子级节点"), this, SLOT(onCreateChildNodeRight()));
		else
			m_pMenu->addAction(QString(u8"增加子级节点"), this, SLOT(onCreateChildNodeLeft()));
	}

	if (m_parent != NULL)
	{
		m_pMenu->addAction(QString(u8"增加同级节点"), this, SLOT(onCreateSliblingNode()));
		m_pMenu->addAction(QString(u8"删除节点"), this, SLOT(onDeleteNode()));
	}

	if (m_noteid.isEmpty())
	{
		m_pMenu->addAction(QString(u8"新建关联笔记"), this, SLOT(onCreateAssociateNote()));
	}
	else
	{
		com_sptr<INote> spNote;
		com_sptr<INotebook> spNotebook;
		AppHelper::GetNoteAndBookById(m_noteid, &spNotebook, &spNote);
		if (!spNote || !spNotebook)
		{
			m_noteid = "";
			m_pMenu->addAction(QString(u8"新建关联笔记"), this, SLOT(onCreateAssociateNote()));
		}
		else
		{
			m_pMenu->addAction(QString(u8"编辑关联笔记"), this, SLOT(onEditAssociateNote()));
		}
	}
}

void MindNode::onCreateAssociateNote()
{
	NewNoteWindow* pNewNoteWindow = new NewNoteWindow(NULL, NORMAL_NOTE);
	connect(pNewNoteWindow, SIGNAL(noteCommited(const QString&)), this, SLOT(onNewNote(const QString&)));

	SelectNotebookDlg dlg;
	if (QDialog::Accepted == dlg.exec())
	{
		QString bookId = dlg.getBookId();
		pNewNoteWindow->init(bookId);
		pNewNoteWindow->showMaximized();
	}
}

void MindNode::onNewNote(const QString& noteid)
{
	Q_ASSERT(!noteid.isEmpty());
	m_noteid = noteid;
	initMenu();
	emit dataChanged();
}

bool MindNode::needShowDecoration() const
{
	return !m_noteid.isEmpty();
}

void MindNode::onEditAssociateNote()
{
	NewNoteWindow* pNewNoteWindow = new NewNoteWindow(NULL, NORMAL_NOTE);

	com_sptr<INote> spNote;
	com_sptr<INotebook> spNotebook;
	AppHelper::GetNoteAndBookById(m_noteid, &spNotebook, &spNote);
	QString bookid = AppHelper::GetNotebookId(spNotebook);
	pNewNoteWindow->open(bookid, m_noteid);
	pNewNoteWindow->showMaximized();
}

void MindNode::onDocumentContentsChanged(int from, int charsRemoved, int charsAdded)
{
	if (m_counter == 0)
	{
		m_content = document()->toPlainText();
		emit textChange();
	}
}

void MindNode::initDocFormat(const QString& text)
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
			format.setBackground(m_backgroudColor);
			format.setLeftMargin(10);
			format.setRightMargin(10);
			cursor.setBlockFormat(format);

			QTextCharFormat chrFormat = childBlock.charFormat();
			chrFormat.setFont(QFont(QString::fromUtf16((char16_t*)L"微软雅黑"), pointSize(m_level)));
			chrFormat.setForeground(m_textColor);
			cursor.setBlockCharFormat(chrFormat);
		}
	}
	cursor.insertText(text);

	QTextFrameFormat frameFormat = rootFrame->frameFormat();
	frameFormat.setBackground(m_backgroudColor);
	frameFormat.setMargin(0);
	frameFormat.setPadding(10);

	frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
	frameFormat.setBorderBrush(QColor(23, 157, 235));
	frameFormat.setBorder(m_borderWidth);
	rootFrame->setFrameFormat(frameFormat);

	connect(doc, SIGNAL(contentsChange(int,int,int)), this, SLOT(onDocumentContentsChanged(int, int, int)));
}

void MindNode::focusOutEvent(QFocusEvent* event)
{
	QGraphicsTextItem::focusOutEvent(event);
	clearSelection();
	setTextInteractionFlags(Qt::NoTextInteraction);
}

void MindNode::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsTextItem::mousePressEvent(event);
}

void MindNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void MindNode::onCreateChildNodeRight()
{
	emit childNodeCreate(this, true);
}

void MindNode::onCreateChildNodeLeft()
{
	emit childNodeCreate(this, false);
}

void MindNode::onCreateSliblingNode()
{
	emit silibingNodeCreate(this);
}

void MindNode::onDeleteNode()
{
	emit deleteNode(this);
}

void MindNode::removeNode(MindNode* pNode)
{
	m_children.removeAll(pNode);
	initMenu();
}

void MindNode::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::RightButton)
	{
		Q_ASSERT(m_pMenu);
		QGraphicsView* v = scene()->views().first();
		QPointF sceneP = mapToScene(event->pos());
		QPoint viewP = v->mapFromScene(sceneP);
		QPoint sendMenuEventPos = v->viewport()->mapToGlobal(viewP);
		m_pMenu->popup(sendMenuEventPos);
	}
	QGraphicsTextItem::mouseReleaseEvent(event);
}

bool MindNode::sceneEvent(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::GraphicsSceneHoverEnter:
		//按钮部分已经在矩形以外了，还需要一些判断。
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

void MindNode::SetContent(const std::wstring& content)
{
	m_content = QString::fromStdWString(content);
}

std::wstring MindNode::GetContent() const
{
	return m_content.toStdWString();
}

void MindNode::append(MindNode* pNode)
{
	pNode->setParent(this);
	m_children.append(pNode);
	initMenu();
}

void MindNode::udpateBorderFormat(const QStyleOptionGraphicsItem* option)
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

QRectF MindNode::boundingRect() const
{
	QRectF br = QGraphicsTextItem::boundingRect();
	if (needShowDecoration())
		return br.adjusted(0, 0, iconSize, 0);
	else
		return br;
}

void MindNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	udpateBorderFormat(option);
	QGraphicsTextItem::paint(painter, option, widget);
}

void MindNode::setPosition(QPointF pos)
{
	QGraphicsTextItem::setPos(pos);
}

int MindNode::pointSize(int level) const
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