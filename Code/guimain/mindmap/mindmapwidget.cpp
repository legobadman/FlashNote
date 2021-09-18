#include "stdafx.h"
#include "mindmapwidget.h"
#include "rapidxml_print.hpp"
#include "toolbutton.h"
#include "labelbutton.h"
#include "MyStyle.h"
#include <QAction>
#include <QShortcut>
#include <qmath.h>


MindMapWidget::MindMapWidget(QWidget* parent)
	: QWidget(parent)
	, m_scene(new MindMapScene)
	, m_view(new MindMapView)
	, m_pRoot(NULL)
	, m_factor(1.)
{
	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_view->setScene(m_scene);
	m_view->setDragMode(QGraphicsView::RubberBandDrag);
	m_view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	m_view->setContextMenuPolicy(Qt::ActionsContextMenu);
	m_view->setFrameShape(QFrame::HLine);
	m_view->setFrameShadow(QFrame::Sunken);
	m_view->viewport()->installEventFilter(this);
	m_view->setMouseTracking(true);
	_modifiers = Qt::ControlModifier;
	_zoom_factor_base = 1.0015;

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

	m_zoomout = new NLabelButton;
	m_zoomout->setIcons(QSize(24, 24), QIcon(":/icons/zoomout.png"), QIcon(":/icons/zoomout.png"), QIcon(":/icons/zoomout_hover.png"));
	pHBoxLayout->addWidget(m_zoomout);

	m_zoom_factor = new QLabel;
	QFont font("Microsoft YaHei", 9);
	m_zoom_factor->setFont(font);
	QPalette palette = m_zoom_factor->palette();
	palette.setColor(QPalette::WindowText, QColor(130, 130, 130));
	m_zoom_factor->setPalette(palette);
	m_zoom_factor->setText(QString("%1%").arg((int)(_zoom_factor_base * 100)));
	pHBoxLayout->addWidget(m_zoom_factor);

	m_zoomin = new NLabelButton;
	m_zoomin->setIcons(QSize(24, 24), QIcon(":/icons/zoomin.png"), QIcon(":/icons/zoomin.png"), QIcon(":/icons/zoomin_hover.png"));
	pHBoxLayout->addWidget(m_zoomin);

	NLabelButton* pReset = new NLabelButton;
	pReset->setIcons(QSize(24, 24), QIcon(":/icons/reset.png"), QIcon(":/icons/reset.png"), QIcon(":/icons/reset_hover.png"));
	pHBoxLayout->addWidget(pReset);

	pHBoxLayout->addStretch();
	undoRedoEnable(false, false);

	QFrame* line = new QFrame;
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Plain);
	line->setLineWidth(1);
    palette = line->palette();
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
	connect(m_zoomin, SIGNAL(clicked()), this, SLOT(zoomIn()));
	connect(m_zoomout, SIGNAL(clicked()), this, SLOT(zoomOut()));
	connect(pReset, SIGNAL(clicked()), this, SLOT(resetTransform()));
}

MindMapWidget::~MindMapWidget()
{
}

void MindMapWidget::gentle_zoom(qreal factor)
{
	m_view->scale(factor, factor);
	m_view->centerOn(target_scene_pos);
    QPointF delta_viewport_pos = target_viewport_pos - QPointF(m_view->viewport()->width() / 2.0,
		m_view->viewport()->height() / 2.0);
    QPointF viewport_center = m_view->mapFromScene(target_scene_pos) - delta_viewport_pos;
	m_view->centerOn(m_view->mapToScene(viewport_center.toPoint()));

	qreal factor_i_want = m_view->transform().m11();
	m_zoom_factor->setText(QString("%1%").arg((int)(factor_i_want * 100 + 0.00001)));

	emit zoomed();
}

void MindMapWidget::zoomIn()
{
	m_factor = min(m_factor + m_factor_step, 4.0);
	qreal current_factor = m_view->transform().m11();
	qreal factor_complicate = m_factor / current_factor;
	gentle_zoom(factor_complicate);
}

void MindMapWidget::zoomOut()
{
	m_factor = max(m_factor - m_factor_step, 0.4);
    qreal current_factor = m_view->transform().m11();
	qreal factor_complicate = m_factor / current_factor;
    gentle_zoom(factor_complicate);
}

void MindMapWidget::resetTransform()
{
	m_view->resetTransform();
	m_factor = 1.0;
	m_zoom_factor->setText("100%");
}

void MindMapWidget::set_modifiers(Qt::KeyboardModifiers modifiers)
{
	_modifiers = modifiers;
}

void MindMapWidget::set_zoom_factor_base(double value)
{
	_zoom_factor_base = value;
}

void MindMapWidget::initContent(QString content, bool bSchedule)
{
	m_scene->initContent(content, bSchedule);
}

bool MindMapWidget::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
        QPointF delta = target_viewport_pos - mouse_event->pos();
        if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5) {
            target_viewport_pos = mouse_event->pos();
            target_scene_pos = m_view->mapToScene(mouse_event->pos());
        }
    }
    else if (event->type() == QEvent::Wheel) {
        QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
        if (QApplication::keyboardModifiers() == _modifiers) {
            if (wheel_event->orientation() == Qt::Vertical) {
                double angle = wheel_event->angleDelta().y();
				if (angle > 0)
					zoomIn();
				else
					zoomOut();
                return true;
            }
        }
    }
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