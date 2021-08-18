#include "stdafx.h"
#include "mindnode.h"
#include "mindmapscene.h"
#include "guihelper.h"
#include "newnotewindow.h"
#include "selectnotebookdlg.h"
#include "mindprogressnode.h"
#include "MyStyle.h"
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMenu>
#include <QLineEdit>
#include <QTextFrame>


class ScopeBlockSIG
{
public:
	ScopeBlockSIG(MindNode* pNode) : m_node(pNode)
	{
		if (m_node)
		{
			m_node->document()->blockSignals(true);
			m_node->blockSignals(true);
		}
	}
	~ScopeBlockSIG()
	{
		if (m_node)
		{
			m_node->document()->blockSignals(false);
			m_node->blockSignals(false);
		}
	}

private:
	MindNode* m_node;
};


//////////////////////////////////////////////////////////////
RoundedRectItem::RoundedRectItem(QGraphicsItem* parent)
	: MindNode("", NULL)
{

}

QPainterPath RoundedRectItem::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());
	return path;
}

QRectF RoundedRectItem::boundingRect() const
{
	return QRectF(0, 0, MyStyle::dpiScaled(82), MyStyle::dpiScaled(34));
}

void RoundedRectItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setPen(Qt::NoPen);
	painter->setBrush(QColor(0, 181, 72));
	painter->drawRoundedRect(boundingRect(), 2, 2);
}


/////////////////////////////////////////////////////////////
MindNode::MindNode(const QString& text, MindNode* parent)
	: QGraphicsTextItem(parent)		//先初始化text会导致格式问题，先不这么处理。
	, m_level(0)
	, m_content(text)
	, m_mouseState(MS_UNKNOWN)
	, m_bHovered(false)
	, m_bToRight(true)
	, m_borderWidth(2)
	, m_cornerRadius(7)
	, m_parent(parent)
	, m_counter(0)
	, m_left_expand(EXP_EXPAND)
	, m_right_expand(EXP_EXPAND)
	, m_pMenu(NULL)
	, m_pathItem(NULL)
	, m_bDragging(false)
{
	setFlags(ItemIsMovable | ItemSendsGeometryChanges | ItemIsSelectable | ItemClipsToShape);
}

MindNode::~MindNode()
{
	if (m_pathItem && scene())
	{
		//scene()->removeItem(m_pathItem);
		//m_pathItem = NULL;
	}
	//	delete m_pathItem;	//todo: smart pointer
}

QList<MindNode*> MindNode::Children(bool excludeDragging) const
{
	QList<MindNode*> _children;
	foreach(QGraphicsItem * pObj, m_children)//childItems()) //为了维护插入时的顺序，只能用自己的容器了。
	{
		MindNode* p = qgraphicsitem_cast<MindNode*>(pObj);
		if (p)
		{
			if (excludeDragging && p->isDragging())
				continue;
			_children.append(p);
		}
	}
	return _children;
}

void MindNode::setup(MindMapScene* pScene)
{
	//setup的顺序是：先执行子节点的setup，再执行parent节点的setup。
	//而子节点在setup前，其parent的基本属性已读取。
	ScopeBlockSIG scope(this);
	initSignalSlots(pScene);
	initUIColor();
	initDocFormat(m_content);
	setCornerRadius(m_cornerRadius);
	resetDecoration();
	initDirection();
	initMenu();
	if (!isTopRoot())
	{
		m_pathItem = new QGraphicsPathItem(this);
		//m_pathItem = new QGraphicsPathItem;
		//pScene->addItem(m_pathItem);
	}
	initExpandBtns();
}

void MindNode::initSignalSlots(MindMapScene* pScene)
{
	if (document())
		connect(document(), SIGNAL(contentsChanged()), pScene, SLOT(onNodeContentsChanged()));
	connect(this, SIGNAL(dataChanged(bool)), pScene, SIGNAL(itemContentChanged(bool)));
	connect(this, SIGNAL(nodeCreated(MindNode*)), pScene, SLOT(onNodeCreated(MindNode*)));
	connect(this, SIGNAL(nodeDeleted(MindNode*)), pScene, SLOT(onNodeDeleted(MindNode*)));
	connect(this, SIGNAL(expandChanged()), pScene, SLOT(onNodeStateChanged()));
	connect(this, SIGNAL(nodeDragged(MindNode*)), pScene, SLOT(onNodeDragged(MindNode*)));
	connect(this, SIGNAL(nodeDragging(MindNode*)), pScene, SLOT(onNodeDragging(MindNode*)));
}

void MindNode::initUIColor()
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

	QPalette pal = palette();
	//处理选中文本的情况。
	pal.setBrush(QPalette::Active, QPalette::Highlight, QColor(0, 129, 218));
	pal.setBrush(QPalette::Active, QPalette::HighlightedText, QColor(255, 255, 255));
	pal.setBrush(QPalette::Inactive, QPalette::Highlight, m_backgroudColor);
	pal.setBrush(QPalette::Inactive, QPalette::HighlightedText, m_textColor);

	setPalette(pal);
}

void MindNode::initExpandBtns()
{
	if (isTopRoot())
	{
		bool bHasLeftChild = false, bHasRightChild = false;
		for (auto it = m_children.begin(); it != m_children.end(); it++)
		{
			if ((*it)->isToRight())
				bHasRightChild = true;
			else
				bHasLeftChild = true;
		}

		if (m_pLCollaspBtn == NULL)
		{
			if (bHasLeftChild)
			{
				m_pLCollaspBtn.reset(new MindNodeButton(this));
				m_pLCollaspBtn->installSceneEventFilter(this);
				m_pLCollaspBtn->hide();
				connect(m_pLCollaspBtn.get(), SIGNAL(toggled()), this, SLOT(onLeftExpandBtnToggle()));
			}
			else
			{
				m_left_expand = EXP_NODEFINE;
			}
		}

		if (m_pRCollaspBtn == NULL)
		{
			if (bHasRightChild)
			{
				m_pRCollaspBtn.reset(new MindNodeButton(this));
				m_pRCollaspBtn->installSceneEventFilter(this);
				m_pRCollaspBtn->hide();
				connect(m_pRCollaspBtn.get(), SIGNAL(toggled()), this, SLOT(onRightExpandBtnToggle()));
			}
			else
			{
				m_right_expand = EXP_NODEFINE;
			}
		}
	}
	else if (m_bToRight)
	{
		if (m_pRCollaspBtn == NULL)
		{
			m_left_expand = EXP_NODEFINE;
			if (!m_children.empty())
			{
				m_pRCollaspBtn.reset(new MindNodeButton(this));
				m_pRCollaspBtn->installSceneEventFilter(this);
				m_pRCollaspBtn->hide();
				connect(m_pRCollaspBtn.get(), SIGNAL(toggled()), this, SLOT(onRightExpandBtnToggle()));
			}
			else
			{
				m_right_expand = EXP_NODEFINE;
			}
		}
	}
	else
	{
		if (m_pLCollaspBtn == NULL)
		{
			m_right_expand = EXP_NODEFINE;
			if (!m_children.empty())
			{
				m_pLCollaspBtn.reset(new MindNodeButton(this));
				m_pLCollaspBtn->installSceneEventFilter(this);
				m_pLCollaspBtn->hide();
				connect(m_pLCollaspBtn.get(), SIGNAL(toggled()), this, SLOT(onLeftExpandBtnToggle()));
			}
			else
			{
				m_left_expand = EXP_NODEFINE;
			}
		}
	}

	//设置位置
	if (m_pLCollaspBtn)
	{
		int x = -MindNodeButton::m_radius * 2 + 3;
		int y = boundingRect().height() / 2 - 13;
		m_pLCollaspBtn->setZValue(zValue() + 3);
		m_pLCollaspBtn->setPos(QPointF(x, y));
	}
	if (m_pRCollaspBtn)
	{
		int x = boundingRect().width() - 3;
		int y = boundingRect().height() / 2 - 13;
		m_pRCollaspBtn->setZValue(zValue() + 3);
		m_pRCollaspBtn->setPos(QPointF(x, y));
	}
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

void MindNode::resetDecoration()
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
		pNewNoteWindow->show();
	}
}

void MindNode::onNewNote(const QString& noteid)
{
	Q_ASSERT(!noteid.isEmpty());
	m_noteid = noteid;
	initMenu();
	resetDecoration();
	emit dataChanged(false);
	emit textChange();
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
	pNewNoteWindow->show();
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

void MindNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void MindNode::onCreateChildNodeRight()
{
	//展开子项。
	m_right_expand = EXP_EXPAND;
	NewChild(true);
}

void MindNode::onCreateChildNodeLeft()
{
	m_left_expand = EXP_EXPAND;
	NewChild(false);
}

void MindNode::onCreateSliblingNode()
{
	m_parent->NewChild(isToRight());
}

void MindNode::onDeleteNode()
{
	m_parent->m_children.removeAll(this);
	initMenu();
	m_parent->checkRemoveExpandBtns(m_bToRight);
	emit nodeDeleted(this);
}

void MindNode::checkRemoveExpandBtns(bool bToRight)
{
	if (bToRight)
	{
		bool bHasRightChildren = false;
		for (auto it = m_children.begin(); it != m_children.end(); it++)
		{
			if ((*it)->isToRight())
			{
				bHasRightChildren = true;
				break;
			}
		}
		if (bHasRightChildren == false)
		{
			m_pRCollaspBtn->setParentItem(NULL);
			m_pRCollaspBtn.clear();
		}
	}
	else
	{
		bool bHasLeftChildren = false;
		for (auto it = m_children.begin(); it != m_children.end(); it++)
		{
			if (!(*it)->isToRight())
			{
				bHasLeftChildren = true;
				break;
			}
		}
		if (bHasLeftChildren == false)
		{
			m_pLCollaspBtn->setParentItem(NULL);
			m_pLCollaspBtn.clear();
		}
	}
}

void MindNode::setLeftExpand(bool bExpanded)
{
	if (bExpanded)
		m_left_expand = EXP_EXPAND;
	else
		m_left_expand = EXP_COLLAPSE;
}

void MindNode::setRightExpand(bool bExpanded)
{
	if (bExpanded)
		m_right_expand = EXP_EXPAND;
	else
		m_right_expand = EXP_COLLAPSE;
}

bool MindNode::isExpanded(MindNodeButton* pBtn)
{
	if (m_pLCollaspBtn.get() == pBtn)
	{
		return m_left_expand == EXP_EXPAND;
	}
	else if (m_pRCollaspBtn.get() == pBtn)
	{
		return m_right_expand == EXP_EXPAND;
	}
	Q_ASSERT(false);
	return false;
}

bool MindNode::isCollapsed(bool bRight) const
{
	if (bRight)
	{
		return m_right_expand == EXP_COLLAPSE;
	}
	else
	{
		return m_left_expand == EXP_COLLAPSE;
	}
}

void MindNode::onLeftExpandBtnToggle()
{
	bool bVisible = true;
	if (m_left_expand == EXP_EXPAND)
	{
		m_left_expand = EXP_COLLAPSE;
		bVisible = false;
	}
	else
	{
		m_left_expand = EXP_EXPAND;
		bVisible = true;
	}
	//将所有左节点隐藏起来
	for (auto it = m_children.begin(); it != m_children.end(); it++)
	{
		if (!(*it)->isToRight())
		{
			(*it)->setVisible(bVisible);
			//(*it)->pathItem()->setVisible(bVisible);
		}
	}
	emit expandChanged();
}

void MindNode::onRightExpandBtnToggle()
{
	if (m_content == u8"新增节点4")
	{
		QPointF pos = this->scenePos();
		int j;
		j = 0;
	}

	bool bVisible = true;
	if (m_right_expand == EXP_EXPAND)
	{
		m_right_expand = EXP_COLLAPSE;
		bVisible = false;
	}
	else
	{
		m_right_expand = EXP_EXPAND;
		bVisible = true;
	}
	//将所有右节点隐藏起来
	for (auto it = m_children.begin(); it != m_children.end(); it++)
	{
		if ((*it)->isToRight())
		{
			(*it)->setVisible(bVisible);
			//(*it)->pathItem()->setVisible(bVisible);
		}
	}
	emit expandChanged();
}

void MindNode::removeChild(MindNode* pNode)
{
	m_children.removeAll(pNode);
	pNode->setParentItem(NULL);
}

void MindNode::insertChild(MindNode* pNode, int idx)
{
	m_children.insert(m_children.begin() + idx, pNode);
	pNode->m_parent = this;	//后续逐渐去掉m_parent。
	pNode->setParentItem(this);
}

void MindNode::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	m_initClickScenePos = event->scenePos();
	QGraphicsTextItem::mousePressEvent(event);
}

qreal MindNode::_dist(const QPointF& p1, const QPointF& p2)
{
	QPointF temp = p2 - p1;
	return sqrt(temp.x() * temp.x() + temp.y() * temp.y());
}

void MindNode::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	if (isTopRoot())
		return;

	if (!m_bDragging)
	{
		//检查是否有足够的拖动距离认为是脱离
		qreal dist = _dist(m_initClickScenePos, event->scenePos());
		if (dist > 3)
		{
			m_item_event_offset = event->scenePos() - this->scenePos();
			m_parent->removeChild(this);
			m_pathItem->hide();
			m_bDragging = true;
		}
	}

	if (m_bDragging)
	{
		QPointF pos = event->scenePos() - m_item_event_offset;
		setPos(pos);
		emit nodeDragging(this);
	}
	else
	{
		QGraphicsTextItem::mouseMoveEvent(event);
	}
}

void MindNode::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	if (m_bDragging)
	{
		m_bDragging = false;
		if (m_pathItem)
			m_pathItem->show();
		emit nodeDragged(this);
	}
	if (event->button() == Qt::RightButton)
	{
		Q_ASSERT(m_pMenu);
		QGraphicsView* v = scene()->views().first();
		QPointF sceneP = mapToScene(event->pos());
		QPoint viewP = v->mapFromScene(sceneP);
		QPoint sendMenuEventPos = v->viewport()->mapToGlobal(viewP);
		m_pMenu->popup(sendMenuEventPos);
	}
	else if (event->button() == Qt::LeftButton)
	{
		if (cursor().shape() == Qt::PointingHandCursor && !m_noteid.isEmpty())
		{
			onEditAssociateNote();
		}
	}
	QGraphicsTextItem::mouseReleaseEvent(event);
}

bool MindNode::sceneEvent(QEvent* event)
{
	switch (event->type())
	{
	case QEvent::GraphicsSceneHoverEnter:
		//按钮部分已经在矩形以外了，还需要一些判断。
		{
			QGraphicsSceneHoverEvent* e = static_cast<QGraphicsSceneHoverEvent*>(event);
			if (contains(e->pos()))
			{
				m_bHovered = true;
				if (m_pLCollaspBtn)
					m_pLCollaspBtn->show();
				if (m_pRCollaspBtn)
					m_pRCollaspBtn->show();
			}
			update();
			break;
		}
	case QEvent::GraphicsSceneHoverLeave:
		{
			QGraphicsSceneHoverEvent* e = static_cast<QGraphicsSceneHoverEvent*>(event);
			QPointF pos = e->pos();	
			m_bHovered = false;
			if (m_pLCollaspBtn)
				m_pLCollaspBtn->hide();
			if (m_pRCollaspBtn)
				m_pRCollaspBtn->hide();
			update();
			break;
		}
	case QEvent::GraphicsSceneHoverMove:
		{
			QGraphicsSceneMouseEvent* e = static_cast<QGraphicsSceneMouseEvent*>(event);
			QPointF pos = e->pos();
			QRectF br = boundingRect();
			if (!m_noteid.isEmpty() && QRectF(br.width() - 30, br.top() + 16, 16, 16).contains(pos))
			{
				setCursor(QCursor(Qt::PointingHandCursor));
			}
			else
			{
				setCursor(QCursor(Qt::ArrowCursor));
			}
		}
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

bool MindNode::sceneEventFilter(QGraphicsItem* watched, QEvent* event)
{
	if (watched == m_pRCollaspBtn.get())
	{
		switch (event->type() == QEvent::Paint)
		{
			break;
		}
	}
	return QGraphicsTextItem::sceneEventFilter(watched, event);
}

void MindNode::SetContent(const QString& content)
{
	m_content = content;
}

QString MindNode::GetContent() const
{
	return m_content;
}

void MindNode::AddChild(MindNode* pChild)
{
	m_children.append(pChild);
}

void MindNode::NewChild(bool toRight)
{
	MindNode* pChild = NULL;
	if (qobject_cast<MindProgressNode*>(this))
	{
		pChild = new MindProgressNode(u8"进度节点", this);
	}
	else
	{
		pChild = new MindNode(u8"新增节点", this);
	}
	pChild->setToRight(toRight);
	pChild->setLevel(level() + 1);
	m_children.append(pChild);
	pChild->initMenu();
	initExpandBtns();
	emit nodeCreated(pChild);
}

//QList<MindNode*> MindNode::children(bool toRight)
//{
//	QList<MindNode*> result;
//	QList<MindNode*>& children = m_children;
//	for (int i = 0; i < children.length(); i++)
//	{
//		if (children[i]->isToRight() == toRight)
//			result.append(children[i]);
//	}
//	return result;
//}

QPainterPath MindNode::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());
	return path;
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
	document()->blockSignals(true);
	rootFrame->setFrameFormat(frameFormat);
	document()->blockSignals(false);
}

QRectF MindNode::boundingRect() const
{
	QRectF br = QGraphicsTextItem::boundingRect();
	if (needShowDecoration())
		return br.adjusted(0, 0, iconSize, 0);
	else
		return br;
}

QRectF MindNode::childrenOrSelfRect(bool isToRight) const
{
	if (Children().isEmpty() || isCollapsed(isToRight))
	{
		return boundingRect();
	}
	else
	{
		return childrenRect(isToRight);
	}
}

QRectF MindNode::childrenRect(bool bToRight) const
{
	//折叠以后实际上隐藏的children的rect已经没有意义了，因为没有绘制回来。
	if (m_children.isEmpty() || isCollapsed(bToRight))
	{
		return QRectF();
	}
	else
	{
		QRectF hRect;
		//方法：找到最为边界的xy值（以this坐标系为参考系）
		qreal xLeft = INT_MAX, xRight = INT_MIN, yTop = INT_MAX, yBottom = INT_MIN;
		for (int i = 0; i < m_children.length(); i++)
		{
			MindNode* pChild = m_children[i];
			//TODO: holder也要设置toRight
			if (pChild->isToRight() != bToRight)
				continue;

			QRectF bRect = pChild->boundingRect();
			QRectF childRect = this->mapRectFromItem(pChild, pChild->childrenRect(bToRight));
			QPointF itemPos = pChild->pos();
			if (childRect.isEmpty())
			{
				yTop = min(yTop, itemPos.y());
				yBottom = max(yBottom, itemPos.y() + bRect.height());
				xLeft = min(xLeft, itemPos.x());
				xRight = max(xRight, itemPos.x() + bRect.width());
			}
			else
			{
				yTop = min(yTop, min(itemPos.y(), childRect.y()));
				yBottom = max(yBottom, max(itemPos.y() + bRect.height(), childRect.bottom()));
				if (bToRight)
				{
					xLeft = min(xLeft, itemPos.x());
					xRight = max(xRight, childRect.right());
				}
				else
				{
					xLeft = min(xLeft, childRect.left());
					xRight = max(xRight, itemPos.x() + bRect.width());
				}
			}
		}
		return QRectF(QPointF(xLeft, yTop), QPointF(xRight, yBottom));
	}
}

void MindNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	udpateBorderFormat(option);
	QGraphicsTextItem::paint(painter, option, widget);
}

void MindNode::setPosition(QPointF pos)
{
	QGraphicsTextItem::setPos(pos);
	if (m_pLCollaspBtn)
	{
		int x = - MindNodeButton::m_radius * 2 + 3;
		int y = boundingRect().height() / 2 - 13;
		m_pLCollaspBtn->setZValue(zValue() + 3);
		m_pLCollaspBtn->setPos(QPointF(x, y));
	}
	if (m_pRCollaspBtn)
	{
		int x = boundingRect().width() - 3;
		int y = boundingRect().height() / 2 - 13;
		m_pRCollaspBtn->setZValue(zValue() + 3);
		m_pRCollaspBtn->setPos(QPointF(x, y));
	}
	if (m_pathItem)
	{
		static int HMargin = 60;	//采用scene内部计算的HMargin
		QGraphicsItem* pParent = parentItem();
		QRectF bbox = boundingRect();

		QPointF parentPos = pParent->scenePos();

		//在scene坐标系下，计算root纵坐标。
		qreal ytemp = parentPos.y() + pParent->boundingRect().height() / 2.0;
		ytemp = mapFromScene(QPointF(0, ytemp)).y();

		QPointF rootConnector, childConnector;
		if (isToRight())
		{
			rootConnector = QPointF(-HMargin, ytemp);
		}
		else
		{
			rootConnector = QPointF(bbox.width() + HMargin, ytemp);
		}
		if (isToRight())
		{
			childConnector = QPointF(0, bbox.height() / 2);
		}
		else
		{
			childConnector = QPointF(bbox.right(), bbox.height() / 2);
		}

		QPointF c1((rootConnector.x() + childConnector.x()) / 2.0, rootConnector.y());
		QPointF c2((rootConnector.x() + childConnector.x()) / 2.0, childConnector.y());

		QPainterPath path;
		path.moveTo(rootConnector);
		path.cubicTo(c1, c2, childConnector);
		m_pathItem->setPen(QPen(QColor(0, 181, 72), 3));
		m_pathItem->setPath(path);
		m_pathItem->update();
	}
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