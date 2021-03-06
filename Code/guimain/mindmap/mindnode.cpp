#include "stdafx.h"
#include "mindnode.h"
#include "mindmapscene.h"
#include "mindtransaction.h"
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
RoundedRectItem::RoundedRectItem(QGraphicsItem* parent, const QColor& clr, Qt::PenStyle style)
	: MindNode("", NULL)
{
	m_color = clr;
	createPathItem(m_color, style);
}

QPainterPath RoundedRectItem::shape() const
{
	QPainterPath path;
	path.addRoundedRect(boundingRect(), m_radius, m_radius);
	return path;
}

QRectF RoundedRectItem::boundingRect() const
{
	return QRectF(0, 0, MyStyle::dpiScaled(82), MyStyle::dpiScaled(34));
}

void RoundedRectItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setPen(QPen(m_color, 5, Qt::DotLine));
	painter->drawRoundedRect(boundingRect(), m_radius, m_radius);
}


/////////////////////////////////////////////////////////////
MindNode::MindNode(const QString& text, MindNode* parent)
	: QGraphicsTextItem(parent)
	, m_level(0)
	, m_content(text)
	, m_mouseState(MS_UNKNOWN)
	, m_bHovered(false)
	, m_bToRight(true)
	, m_borderWidth(MyStyle::dpiScaled(2))
	, m_cornerRadius(MyStyle::dpiScaled(7))
	, m_parent(parent)
	, m_counter(0)
	, m_left_expand(EXP_EXPAND)
	, m_right_expand(EXP_EXPAND)
	, m_pMenu(NULL)
	, m_pathItem(NULL)
	, m_bDragging(false)
	, m_scene(NULL)
	, m_linkItem(NULL)
{
	setFlags(ItemIsMovable | ItemSendsGeometryChanges | ItemIsSelectable | ItemClipsToShape);
}

MindNode::~MindNode()
{
}

QList<MindNode*> MindNode::Children(bool excludeHolder, int direction) const
{
	QList<MindNode*> _children;
	foreach(QGraphicsItem * pObj, m_children)
	{
		MindNode* p = qgraphicsitem_cast<MindNode*>(pObj);
		if (direction == 0 && p->isToRight())
			continue;
		if (direction == 1 && !p->isToRight())
			continue;

		if (p)
		{
			if (excludeHolder && p->isHolder())
				continue;
			_children.append(p);
		}
	}
	return _children;
}

void MindNode::setup(MindMapScene* pScene)
{
	ScopeBlockSIG scope(this);
	m_scene = pScene;
	initSignalSlots(pScene);
	initUIColor();
	initDocFormat(m_content);
	setCornerRadius(m_cornerRadius);
	createLinkNoteItem();
	initDirection();
	initMenu();
	if (!isTopRoot())
	{
		createPathItem(m_mainThemeColor, Qt::SolidLine);
	}
	initExpandBtns();
}

void MindNode::initSignalSlots(MindMapScene* pScene)
{
	if (document())
		connect(document(), SIGNAL(contentsChanged()), pScene, SLOT(onNodeContentsChanged()));
	connect(this, SIGNAL(dataChanged()), pScene, SLOT(onNodeStateChanged()));
	connect(this, SIGNAL(nodeCreated(MindNode*)), pScene, SLOT(onNodeCreated(MindNode*)));
	connect(this, SIGNAL(nodeDeleted(MindNode*)), pScene, SLOT(onNodeDeleted(MindNode*)));
	connect(this, SIGNAL(expandChanged()), pScene, SLOT(onNodeStateChanged()));
	connect(this, SIGNAL(nodeDragged(MindNode*)), pScene, SLOT(onNodeDragged(MindNode*)));
	connect(this, SIGNAL(nodeDragging(MindNode*)), pScene, SLOT(onNodeDragging(MindNode*)));
	connect(this, SIGNAL(nodeLinkAdded()), pScene, SLOT(onNodeStateChanged()));
	connect(pScene, SIGNAL(zoomInOrOut(qreal)), this, SLOT(onZoom(qreal)));
}

void MindNode::initUIColor()
{
	m_mainThemeColor = AppHelper::colorBlue();
	if (m_level == 0)
	{
		m_textColor = QColor(255, 255, 255);
		m_backgroudColor = m_mainThemeColor;
		m_highlightedBorder = QColor(23, 157, 235);
		m_selectedBorder = QColor(23, 157, 235);
		m_borderFocusout = QColor(m_backgroudColor);
	}
	else
	{
		if (hasNoChildren()) {
            m_textColor = QColor(0, 0, 0);
            m_highlightedBorder = QColor(136, 203, 242);
            m_selectedBorder = QColor(23, 157, 235);
            m_backgroudColor = QColor(255, 255, 255);
            m_borderFocusout = QColor(m_backgroudColor);
		}
		else
		{
            m_textColor = QColor(0, 0, 0);
            m_highlightedBorder = QColor(136, 203, 242);
            m_selectedBorder = QColor(23, 157, 235);
            m_backgroudColor = QColor(242, 242, 242);
            m_borderFocusout = QColor(m_backgroudColor);
		}
	}

	QPalette pal = palette();
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
				m_pLCollaspBtn->setVisible(m_left_expand == EXP_COLLAPSE);
				connect(m_pLCollaspBtn.get(), SIGNAL(toggled()), this, SLOT(onLeftExpandBtnToggle()));
			}
		}

		if (m_pRCollaspBtn == NULL)
		{
			if (bHasRightChild)
			{
				m_pRCollaspBtn.reset(new MindNodeButton(this));
				m_pRCollaspBtn->setVisible(m_right_expand == EXP_COLLAPSE);
				connect(m_pRCollaspBtn.get(), SIGNAL(toggled()), this, SLOT(onRightExpandBtnToggle()));
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
				m_pRCollaspBtn->setVisible(m_right_expand == EXP_COLLAPSE);
				connect(m_pRCollaspBtn.get(), SIGNAL(toggled()), this, SLOT(onRightExpandBtnToggle()));
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
				m_pLCollaspBtn->setVisible(m_left_expand == EXP_COLLAPSE);
				connect(m_pLCollaspBtn.get(), SIGNAL(toggled()), this, SLOT(onLeftExpandBtnToggle()));
			}
		}
	}

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

void MindNode::createPathItem(const QColor& clr, Qt::PenStyle style)
{
	if (m_pathItem == NULL)
	{
		m_pathItem = new QGraphicsPathItem(this);
		const int penWidth = 3;
		QPen pen(clr, penWidth);
		pen.setStyle(style);
		m_pathItem->setPen(pen);
	}
}

void MindNode::createLinkNoteItem()
{
	if (!m_noteid.isEmpty() && m_linkItem == NULL)
	{
		qreal scale = 1.0;
		static qreal dpi = MyStyle::dpiScaled(1);
		QString iconFile = ":/icons/16x16/link_note_black.svg";
		if (dpi == 1.0)
		{
			iconFile = ":/icons/16x16/link_note_black.svg";
		}
		else if (dpi == 1.25)
		{
			iconFile = ":/icons/20x20/link_note_black.svg";
		}
		else if (dpi == 1.5)
		{
			iconFile = ":/icons/24x24/link_note_black.svg";
		}
		else if (dpi == 1.75)
		{
			iconFile = ":/icons/28x28/link_note_black.svg";
		}
		else if (dpi == 2)
		{
			iconFile = ":/icons/32x32/link_note_black.svg";
		}
        m_linkItem = new LinkNoteItem(iconFile, this);
		setDecorationSize(MyStyle::dpiScaled(sIconSize));
		connect(m_linkItem, SIGNAL(triggered()), this, SLOT(onEditAssociateNote()));
	}
}

void MindNode::setTextColor(const QColor& clr)
{
	m_textColor = clr;
}

void MindNode::setMainThemeColor(const QColor& mainTheme)
{
	m_mainThemeColor = mainTheme;
}

void MindNode::setColors(const QColor& mainTheme, const QColor& bgClr, const QColor& selectedBdr, const QColor& highlightBlr, const QColor& focusOutBdr)
{
	m_mainThemeColor = mainTheme;
	m_backgroudColor = bgClr;
	m_selectedBorder = selectedBdr;
	m_highlightedBorder = highlightBlr;
	m_borderFocusout = focusOutBdr;
}

int MindNode::childNodeCount()
{
	return m_children.size();
}

bool MindNode::hasNoChildren()
{
	return m_children.isEmpty();
}

QPointer<QMenu> MindNode::getMenu()
{
	return m_pMenu;
}

void MindNode::onZoom(qreal factor)
{
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
	createLinkNoteItem();
	emit nodeLinkAdded();
}

bool MindNode::needShowDecoration() const
{
	return !m_noteid.isEmpty();
}

void MindNode::onEditAssociateNote()
{
	AppHelper::openNoteInIsoWindow(m_noteid);
}

void MindNode::initDocFormat(const QString& text)
{
	QTextDocument* doc = document();
	doc->clear();	//防止重入

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
			format.setLeftMargin(MyStyle::dpiScaled(10));
			format.setRightMargin(MyStyle::dpiScaled(10));
			cursor.setBlockFormat(format);

			QTextCharFormat chrFormat = childBlock.charFormat();
			chrFormat.setFont(QFont("Microsoft YaHei", pointSize(m_level)));
			chrFormat.setForeground(m_textColor);
			cursor.setBlockCharFormat(chrFormat);
		}
	}
	cursor.insertText(text);

	QTextFrameFormat frameFormat = rootFrame->frameFormat();
	frameFormat.setBackground(m_backgroudColor);
	frameFormat.setMargin(0);
	frameFormat.setPadding(MyStyle::dpiScaled(10));

	frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
	frameFormat.setBorderBrush(QColor(23, 157, 235));
	frameFormat.setBorder(m_borderWidth);
	rootFrame->setFrameFormat(frameFormat);
}

void MindNode::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void MindNode::onCreateChildNodeRight()
{
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
	RAIITransBatch batch(m_scene);
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
		if (bHasRightChildren == false && m_pRCollaspBtn)
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
		if (bHasLeftChildren == false && m_pLCollaspBtn)
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
	RAIITransBatch batch(m_scene);

	bool bVisible = true;
	if (m_left_expand == EXP_EXPAND)
	{
		m_left_expand = EXP_COLLAPSE;
		m_pLCollaspBtn->show();
		bVisible = false;
	}
	else
	{
		m_left_expand = EXP_EXPAND;
		m_pLCollaspBtn->hide();
		bVisible = true;
	}
	for (auto it = m_children.begin(); it != m_children.end(); it++)
	{
		if (!(*it)->isToRight())
		{
			(*it)->setVisible(bVisible);
		}
	}
	emit expandChanged();
}

void MindNode::onRightExpandBtnToggle()
{
	RAIITransBatch batch(m_scene);

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
		m_pRCollaspBtn->show();
		bVisible = false;
	}
	else
	{
		m_right_expand = EXP_EXPAND;
		m_pRCollaspBtn->hide();
		bVisible = true;
	}
	for (auto it = m_children.begin(); it != m_children.end(); it++)
	{
		if ((*it)->isToRight())
		{
			(*it)->setVisible(bVisible);
		}
	}
	emit expandChanged();
}

void MindNode::_collaspe(bool toRight)
{
	if (m_children.isEmpty())
		return;

	if (toRight)
		m_right_expand = EXP_COLLAPSE;
	else
		m_left_expand = EXP_COLLAPSE;

	for (auto it = m_children.begin(); it != m_children.end(); it++)
	{
		if ((*it)->isToRight() == toRight)
		{
			(*it)->setVisible(false);
		}
	}
}

void MindNode::removeChild(MindNode* pNode)
{
	m_children.removeAll(pNode);
	pNode->setParentItem(NULL);
}

void MindNode::resetAllChildDirection(bool toRight)
{
	if (toRight == m_bToRight)
		return;

	if (toRight)
	{
		m_right_expand = m_left_expand;
		m_left_expand = EXP_NODEFINE;
		if (!m_children.isEmpty())
		{
            m_pRCollaspBtn.reset(new MindNodeButton(this));
            m_pRCollaspBtn->setVisible(m_right_expand == EXP_COLLAPSE);
            connect(m_pRCollaspBtn.get(), SIGNAL(toggled()), this, SLOT(onRightExpandBtnToggle()));
		}
		m_pLCollaspBtn.clear();
	}
	else
	{
		m_left_expand = m_right_expand;
		m_right_expand = EXP_NODEFINE;
		if (!m_children.isEmpty())
		{
			m_pLCollaspBtn.reset(new MindNodeButton(this));
			m_pLCollaspBtn->setVisible(m_left_expand == EXP_COLLAPSE);
		}
        connect(m_pLCollaspBtn.get(), SIGNAL(toggled()), this, SLOT(onLeftExpandBtnToggle()));
		m_pRCollaspBtn.clear();
	}
	setToRight(toRight);

	for (auto it = m_children.begin(); it != m_children.end(); it++)
	{
		(*it)->resetAllChildDirection(toRight);
	}
}

void MindNode::insertChild(MindNode* pNode, int idx)
{
	m_children.insert(m_children.begin() + idx, pNode);
	pNode->m_parent = this;
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

	//可能在拖动，此时禁止移动节点
	bool bCtrl = QApplication::keyboardModifiers() == Qt::ControlModifier;
	if (bCtrl)
		return;

	if (!m_bDragging)
	{
		qreal dist = _dist(m_initClickScenePos, event->scenePos());
		if (dist > 3)
		{
			m_scene->startMoveTransaction();
			m_item_event_offset = event->scenePos() - this->scenePos();
			m_dragging.m_pOrginalParent = m_parent;
			m_dragging.m_idx = m_parent->m_children.indexOf(this);
			m_parent->removeChild(this);
			m_pathItem->hide();
			_collaspe(m_bToRight);
			m_bDragging = true;
			setZValue(-1);
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
		setZValue(0);
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
		{
			QGraphicsSceneHoverEvent* e = static_cast<QGraphicsSceneHoverEvent*>(event);
			QPointF pt = e->pos();
			QRectF rcBoundRect = boundingRect();
			qreal offset = MyStyle::dpiScaled(1.5);
			//需要稍微扩大区域才能容纳部分稍微在外面一点的点。
			rcBoundRect.adjust(-offset, -offset, offset, offset);
			if (rcBoundRect.contains(pt))
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
			{
				m_pLCollaspBtn->setVisible(m_left_expand == EXP_COLLAPSE);
			}
			if (m_pRCollaspBtn)
			{
				m_pRCollaspBtn->setVisible(m_right_expand == EXP_COLLAPSE);
			}
			update();
			break;
		}
	case QEvent::GraphicsSceneHoverMove:
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

void MindNode::SetContent(const QString& content)
{
	m_content = content;
	initDocFormat(m_content);
}

QString MindNode::GetContent() const
{
	return m_content;
}

void MindNode::AddChild(MindNode* pChild)
{
	m_children.append(pChild);
	pChild->setParentItem(this);
}

void MindNode::NewChild(bool toRight)
{
	RAIITransBatch batch(m_scene);
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

void MindNode::focusInEvent(QFocusEvent* event)
{
    QGraphicsTextItem::focusInEvent(event);
	//TODO: 换行的要考察
	m_focusInText = m_content;
}

void MindNode::focusOutEvent(QFocusEvent* event)
{
    QGraphicsTextItem::focusOutEvent(event);
    clearSelection();
    setTextInteractionFlags(Qt::NoTextInteraction);
	m_content = document()->toPlainText();
	//风险：如果逃逸了focusOUt的事件，会导致没法提交事务。
	if (m_focusInText != m_content)
	{
		m_scene->transRepository()->AddExecute(TRANSFORM_PTR(new EditContentTransform(m_scene, this, m_focusInText, m_content)));
	}
}

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
		return br.adjusted(0, 0, MyStyle::dpiScaled(sIconSize * 0.75), 0);
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

void MindNode::resetPosBeforeDragging()
{
	if (m_dragging.m_idx != -1 && m_dragging.m_pOrginalParent)
	{
		m_parent = m_dragging.m_pOrginalParent;
		setParentItem(m_parent);
		m_parent->m_children.insert(m_dragging.m_idx, this);
	}
}

bool MindNode::_hasDraggingInSide(QPointF scenePos, bool toRight, int& dir_idx)
{
    //只有一个节点或者折叠点的检测区域。
	QRectF currItemRect = mapRectToScene(boundingRect());

	QList<MindNode*> allChildren = Children();
	QList<MindNode*> children = Children(true, toRight);

	static const int yOffsetFromRect = MyStyle::dpiScaled(5);
	static const int xOffsetFromRect = MyStyle::dpiScaled(160);
	static const int topOffset = MyStyle::dpiScaled(10);
	static const int rightOffset = MyStyle::dpiScaled(100);
	static const int leftOffset = MyStyle::dpiScaled(50);
	static const int xBoundariess = MyStyle::dpiScaled(110);

    QRectF rcSingleCollise;
	if (toRight)
	{
		rcSingleCollise = QRectF(QPointF(currItemRect.right() - rightOffset, currItemRect.top() - yOffsetFromRect),
			QPointF(currItemRect.right() + xOffsetFromRect, currItemRect.bottom() + yOffsetFromRect));

		if (!children.isEmpty() && m_right_expand == EXP_COLLAPSE && rcSingleCollise.contains(scenePos))
			onRightExpandBtnToggle();
	}
	else
	{
        rcSingleCollise = QRectF(QPointF(currItemRect.left() - xOffsetFromRect, currItemRect.top() - yOffsetFromRect),
            QPointF(currItemRect.left() - leftOffset, currItemRect.bottom() + yOffsetFromRect));

        if (!children.isEmpty() && m_left_expand == EXP_COLLAPSE && rcSingleCollise.contains(scenePos))
            onLeftExpandBtnToggle();
	}

    QRectF childrenNodesRect = mapRectToScene(childrenRect(toRight));
    //为了能匹配最上面子节点的位置，适度往上扩展rightChildrenRect
    childrenNodesRect.adjust(0, -topOffset, 0, 0);

    if (!children.isEmpty() &&
        childrenNodesRect.contains(scenePos) &&
		(
			(!toRight && childrenNodesRect.right() - scenePos.x() < xBoundariess) ||		//左边
			(toRight && scenePos.x() - childrenNodesRect.left() < xBoundariess)			//右边
		))
    {
		int holderIdx = 0;
        //观察在哪个子节点之间(上)(下)
        foreach(MindNode * pChild, children)
        {
            int yTop = pChild->mapRectToScene(pChild->boundingRect()).top();
            if (yTop < scenePos.y())
                holderIdx++;
            else
                break;
        }
        if (holderIdx == 0)
        {
            MindNode* behindNode = children[0];
            dir_idx = allChildren.indexOf(behindNode);
        }
        else
        {
            MindNode* aboveNode = children[max(0, holderIdx - 1)];
            //找到aboveNode在所有节点的索引。
            holderIdx = allChildren.indexOf(aboveNode);
            dir_idx = holderIdx + 1;
        }
        return true;
    }
    else if (children.isEmpty() && rcSingleCollise.contains(scenePos))
    {
        QString content = GetContent();
        dir_idx = Children(true, !toRight).length();	//可能另一边有节点
        return true;
    }
	else
	{
		return false;
	}
}

bool MindNode::hasDraggingInChildRect(QPointF scenePos, int& dir_idx, bool& toRight)
{
	if (m_right_expand != EXP_NODEFINE)
	{
		if (_hasDraggingInSide(scenePos, true, dir_idx))
		{
			toRight = true;
			return true;
		}
	}

	if (m_left_expand != EXP_NODEFINE)
	{
		if (_hasDraggingInSide(scenePos, false, dir_idx))
		{
			toRight = false;
			return true;
		}
	}
	return false;
}

void MindNode::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	udpateBorderFormat(option);
	QGraphicsTextItem::paint(painter, option, widget);
}

void MindNode::setPathItemVisible(bool bVisible)
{
	if (m_pathItem)
		m_pathItem->setVisible(bVisible);
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
	if (m_linkItem)
	{
		QRectF bbox = boundingRect();
		QRectF rcLink = m_linkItem->boundingRect();
		static int xoffset = 4, yoffset = 1;
        int x = boundingRect().width() - MyStyle::dpiScaled(sIconSize - xoffset);
        int y = boundingRect().height() / 2 - rcLink.height() / 2 + MyStyle::dpiScaled(yoffset);
		m_linkItem->setZValue(zValue() + 3);
		m_linkItem->setPos(QPointF(x, y));
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
