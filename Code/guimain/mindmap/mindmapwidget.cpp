#include "stdafx.h"
#include "mindmapwidget.h"
#include "rapidxml_print.hpp"
#include "toolbutton.h"
#include "labelbutton.h"
#include "MyStyle.h"
#include <QAction>
#include <QShortcut>


MindMapWidget::MindMapWidget(QWidget* parent)
	: QWidget(parent)
	, m_scene(new MindMapScene)
	, m_view(new MindMapView)
	, m_pRoot(NULL)
{
	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_view->setScene(m_scene);
	m_view->setDragMode(QGraphicsView::RubberBandDrag);
	m_view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	m_view->setContextMenuPolicy(Qt::ActionsContextMenu);
	m_view->setFrameShape(QFrame::HLine);
	m_view->setFrameShadow(QFrame::Sunken);

	QVBoxLayout* pMainLayout = new QVBoxLayout;

	QVBoxLayout* pToolLayout = new QVBoxLayout;
	pToolLayout->setContentsMargins(16, 0, 16, 0);

    QHBoxLayout* pHBoxLayout = new QHBoxLayout;

	pHBoxLayout->setContentsMargins(0, 0, 0, 0);
	m_undoBtn = new NLabelButton;
	m_undoBtn->setIcons(QSize(24, 24), QIcon(":/icons/undo.png"), QIcon(":/icons/undo_disable.png"), QIcon(":/icons/undo_hover.png"));
	pHBoxLayout->addWidget(m_undoBtn);

	m_redoBtn = new NLabelButton;
	m_redoBtn->setIcons(QSize(24, 24), QIcon(":/icons/redo.png"), QIcon(":/icons/redo_disable.png"), QIcon(":/icons/redo_hover.png"));
	pHBoxLayout->addWidget(m_redoBtn);
	pHBoxLayout->addStretch();
	undoRedoEnable(false, false);

	QFrame* line = new QFrame;
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Plain);
	line->setLineWidth(1);
    QPalette palette = line->palette();
    palette.setColor(QPalette::WindowText, QColor(233, 233, 233));
    line->setPalette(palette);

	pToolLayout->addLayout(pHBoxLayout);
	pToolLayout->addWidget(line);

	pMainLayout->addLayout(pToolLayout);
	pMainLayout->addWidget(m_view);
	setLayout(pMainLayout);

    m_undo = new QShortcut(this);
	m_undo->setKey(QKeySequence("Ctrl+Z"));
    m_redo = new QShortcut(this);
	m_redo->setKey(QKeySequence("Ctrl+Y"));

	connect(m_scene, SIGNAL(itemContentChanged(bool)), this, SIGNAL(itemContentChanged(bool)));
	connect(m_scene, SIGNAL(undoRedoEnable(bool, bool)), this, SLOT(undoRedoEnable(bool, bool)));

	connect(m_undoBtn, SIGNAL(clicked()), this, SLOT(undo()));
	connect(m_redoBtn, SIGNAL(clicked()), this, SLOT(redo()));
	connect(m_undo, SIGNAL(activated()), this, SLOT(undo()));
	connect(m_redo, SIGNAL(activated()), this, SLOT(redo()));
}

MindMapWidget::~MindMapWidget()
{
}

void MindMapWidget::initContent(QString content, bool bSchedule)
{
	m_scene->initContent(content, bSchedule);
}

bool MindMapWidget::eventFilter(QObject* watched, QEvent* event)
{
	//if (watched == m_undoBtn)
	//{
 //       if (event->type() == QEvent::Enter)
 //       {
	//		m_undoBtn->setPixmap(QIcon(":/icons/undo_hover.png").pixmap(MyStyle::dpiScaledSize(QSize(24, 24))));
 //       }
 //       if (event->type() == QEvent::Leave)
 //       {
	//		m_undoBtn->setPixmap(QIcon(":/icons/undo.png").pixmap(MyStyle::dpiScaledSize(QSize(24, 24))));
 //       }
	//}
	//if (watched == m_redoBtn)
	//{
 //       if (event->type() == QEvent::Enter)
 //       {
	//		m_redoBtn->setPixmap(QIcon(":/icons/redo_hover.png").pixmap(MyStyle::dpiScaledSize(QSize(24, 24))));
 //       }
 //       if (event->type() == QEvent::Leave)
 //       {
	//		m_redoBtn->setPixmap(QIcon(":/icons/redo.png").pixmap(MyStyle::dpiScaledSize(QSize(24, 24))));
 //       }
	//}
	return QWidget::eventFilter(watched, event);
}

QString MindMapWidget::mindmapXML()
{
	return m_scene->mindmapXML();
}

void MindMapWidget::redo()
{
	m_scene->redo();
}

void MindMapWidget::undo()
{
	m_scene->undo();
}

void MindMapWidget::undoRedoEnable(bool undoEnable, bool redoEnable)
{
	m_undoBtn->setEnabled(undoEnable);
	m_redoBtn->setEnabled(redoEnable);
}