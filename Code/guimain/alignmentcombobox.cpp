#include "stdafx.h"
#include "popupwidget.h"
#include "MyStyle.h"
#include "menubutton.h"
#include "alignmentcombobox.h"
#include "moc_alignmentcombobox.cpp"


AlignItemDelegate::AlignItemDelegate(QWidget* parent)
    : QStyledItemDelegate(parent)
{
}

void AlignItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}

QSize AlignItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    return MyStyle::dpiScaledSize(QSize(70, 30));
}

void AlignItemDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
    return QStyledItemDelegate::initStyleOption(option, index);
}


AlignmentComboBox::AlignmentComboBox(QWidget* parent /* = nullptr */)
    : MenuButton(parent)
    , m_popup(nullptr)
    , m_pModel(nullptr)
    , m_pView(nullptr)
{
    setFixedSize(MyStyle::dpiScaledSize(QSize(45, 30)));
    setIcon(QIcon(":/icons/16x16/left_alignment.png"));
    setIconSize(MyStyle::dpiScaledSize(QSize(16, 16)));
    connect(this, SIGNAL(clicked()), this, SLOT(popup()));
}

AlignmentComboBox::~AlignmentComboBox()
{
}

void AlignmentComboBox::popup()
{
    if (!m_popup)
    {
        m_popup = new PopupWidget(this);
        m_pView = new QListView(m_popup);
        m_pModel = new QStandardItemModel(m_pView);

        QStandardItem* pLeftAlignment = new QStandardItem(QIcon(":/icons/16x16/left_alignment.png"), u8"左对齐");
        QStandardItem* pCenterAlignment = new QStandardItem(QIcon(":/icons/16x16/center_alignment.png"), u8"居中");
        QStandardItem* pRightAlignment = new QStandardItem(QIcon(":/icons/16x16/right_alignment.png"), u8"右对齐");
        QStandardItem* pTwoSideAlignment = new QStandardItem(QIcon(":/icons/16x16/twoside_alignment.png"), u8"两端对齐");

        m_pModel->appendRow(pLeftAlignment);
        m_pModel->appendRow(pCenterAlignment);
        m_pModel->appendRow(pRightAlignment);
        m_pModel->appendRow(pTwoSideAlignment);

        m_pView->setModel(m_pModel);
        m_pView->setContentsMargins(QMargins(9,9,9,9));
        m_pView->setItemDelegate(new AlignItemDelegate(m_pView));
        m_pView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        m_pView->setFixedSize(MyStyle::dpiScaledSize(QSize(98, 125)));
        m_popup->setContentWidget(m_pView);

        connect(m_pView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onItemClicked(const QModelIndex&)));
    }

    QPoint pGlobal = mapToGlobal(QPoint(0, 0));
    const int margin = 5;
    setDown(true);
    m_popup->exec(pGlobal.x(), pGlobal.y() + height() + margin, 98, 160);
    setDown(false);
}

void AlignmentComboBox::onItemClicked(const QModelIndex& index)
{
    emit alignmentChanged(index.row());
}
