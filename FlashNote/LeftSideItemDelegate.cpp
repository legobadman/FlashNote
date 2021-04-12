#include "stdafx.h"
#include "LeftSideItemDelegate.h"
#include "moc_LeftSideItemDelegate.cpp"

//#define LEGACY_PAINT_CODE

LeftSideItemDelegate::LeftSideItemDelegate(QWidget* parent)
	: QStyledItemDelegate(parent)
{
}

void LeftSideItemDelegate::setModelData(QWidget* editor,
	QAbstractItemModel* model,
	const QModelIndex& index) const
{
	QStyledItemDelegate::setModelData(editor, model, index);
}

QSize LeftSideItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	int w = ((QWidget*)parent())->width();
	if (index.row() == 0)
		return QSize(w, 60);
	else
		return QSize(w, 36);
}

void LeftSideItemDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
	QStyledItemDelegate::initStyleOption(option, index);

	bool bNewNote = index.row() == 0;
	QColor backgroundClr(42, 51, 60);
	QColor selectedClr(QColor(74, 93, 107));

	option->palette.setColor(QPalette::All, QPalette::Text, QColor(213, 221, 227));
	option->backgroundBrush.setStyle(Qt::SolidPattern);
	option->showDecorationSelected = false;
	if (bNewNote)
	{
		option->decorationSize = QSize(28, 28);
	}
	else
	{
		option->decorationSize = QSize(16, 16);
	}

	if (option->state & QStyle::State_Selected)
	{
		option->backgroundBrush.setColor(selectedClr);
	}
	else if (option->state & QStyle::State_MouseOver)
	{
		if (bNewNote)
		{
			option->backgroundBrush.setColor(backgroundClr);
			option->palette.setColor(QPalette::All, QPalette::Text, QColor(255, 255, 255));
		}
		else
		{
			option->backgroundBrush.setColor(QColor(53, 69, 83));
		}
	}
	else
	{
		option->backgroundBrush.setColor(backgroundClr);
	}
	if (bNewNote)
		option->font.setPointSize(13);
	else
		option->font.setPointSize(12);
}

static QSizeF viewItemTextLayout(QTextLayout& textLayout, int lineWidth, int maxHeight = -1, int* lastVisibleLine = nullptr)
{
	if (lastVisibleLine)
		*lastVisibleLine = -1;
	qreal height = 0;
	qreal widthUsed = 0;
	textLayout.beginLayout();
	int i = 0;
	while (true) {
		QTextLine line = textLayout.createLine();
		if (!line.isValid())
			break;
		line.setLineWidth(lineWidth);
		line.setPosition(QPointF(0, height));
		height += line.height();
		widthUsed = qMax(widthUsed, line.naturalTextWidth());
		// we assume that the height of the next line is the same as the current one
		if (maxHeight > 0 && lastVisibleLine && height + line.height() > maxHeight) {
			const QTextLine nextLine = textLayout.createLine();
			*lastVisibleLine = nextLine.isValid() ? i : -1;
			break;
		}
		++i;
	}
	textLayout.endLayout();
	return QSizeF(widthUsed, height);
}

QString calculateElidedText(const QString& text, const QTextOption& textOption,
	const QFont& font, const QRect& textRect, const Qt::Alignment valign,
	Qt::TextElideMode textElideMode, int flags,
	bool lastVisibleLineShouldBeElided, QPointF* paintStartPosition)
{
	QTextLayout textLayout(text, font);
	textLayout.setTextOption(textOption);

	// In AlignVCenter mode when more than one line is displayed and the height only allows
	// some of the lines it makes no sense to display those. From a users perspective it makes
	// more sense to see the start of the text instead something inbetween.
	const bool vAlignmentOptimization = paintStartPosition && valign.testFlag(Qt::AlignVCenter);

	int lastVisibleLine = -1;
	viewItemTextLayout(textLayout, textRect.width(), vAlignmentOptimization ? textRect.height() : -1, &lastVisibleLine);

	const QRectF boundingRect = textLayout.boundingRect();
	// don't care about LTR/RTL here, only need the height
	const QRect layoutRect = QStyle::alignedRect(Qt::LayoutDirectionAuto, valign,
		boundingRect.size().toSize(), textRect);

	if (paintStartPosition)
		*paintStartPosition = QPointF(textRect.x(), layoutRect.top());

	QString ret;
	qreal height = 0;
	const int lineCount = textLayout.lineCount();
	for (int i = 0; i < lineCount; ++i) {
		const QTextLine line = textLayout.lineAt(i);
		height += line.height();

		// above visible rect
		if (height + layoutRect.top() <= textRect.top()) {
			if (paintStartPosition)
				paintStartPosition->ry() += line.height();
			continue;
		}

		const int start = line.textStart();
		const int length = line.textLength();
		const bool drawElided = line.naturalTextWidth() > textRect.width();
		bool elideLastVisibleLine = lastVisibleLine == i;
		if (!drawElided && i + 1 < lineCount && lastVisibleLineShouldBeElided) {
			const QTextLine nextLine = textLayout.lineAt(i + 1);
			const int nextHeight = height + nextLine.height() / 2;
			// elide when less than the next half line is visible
			if (nextHeight + layoutRect.top() > textRect.height() + textRect.top())
				elideLastVisibleLine = true;
		}

		QString text = textLayout.text().mid(start, length);
		if (drawElided || elideLastVisibleLine) {
			if (elideLastVisibleLine) {
				if (text.endsWith(QChar::LineSeparator))
					text.chop(1);
				text += QChar(0x2026);
			}
		}
		else {
			ret += text;
		}

		// below visible text, can stop
		if ((height + layoutRect.top() >= textRect.bottom()) ||
			(lastVisibleLine >= 0 && lastVisibleLine == i))
			break;
	}
	return ret;
}

void viewItemDrawText(QPainter* p, const QStyleOptionViewItem* option, const QRect& rect)
{
	//参照QCommonStylePrivate::viewItemDrawText，做适量简化
	const QWidget* widget = option->widget;
	const int textMargin = 2;

	QRect textRect = rect.adjusted(textMargin, 0, -textMargin, 0); // remove width padding
	const bool wrapText = option->features & QStyleOptionViewItem::WrapText;
	QTextOption textOption;
	textOption.setWrapMode(wrapText ? QTextOption::WordWrap : QTextOption::ManualWrap);
	textOption.setTextDirection(option->direction);
	textOption.setAlignment(QStyle::visualAlignment(option->direction, option->displayAlignment));

	QPointF paintPosition;
	const QString newText = calculateElidedText(option->text, textOption,
		option->font, textRect, option->displayAlignment,
		option->textElideMode, 0,
		true, &paintPosition);

	QTextLayout textLayout(newText, option->font);
	textLayout.setTextOption(textOption);
	viewItemTextLayout(textLayout, textRect.width());
	textLayout.draw(p, paintPosition);
}

void LeftSideItemDelegate::drawBackground(QStyleOptionViewItem& opt, QPainter* p, const QWidget* widget) const
{
	if (opt.features & QStyleOptionViewItem::Alternate)
		p->fillRect(opt.rect, opt.palette.alternateBase());

	if (opt.backgroundBrush.style() != Qt::NoBrush)
	{
		const QPointF oldBrushOrigin = p->brushOrigin();
		p->setBrushOrigin(opt.rect.topLeft());
		p->fillRect(opt.rect, opt.backgroundBrush);
		p->setBrushOrigin(oldBrushOrigin);
	}
	return;
}

void LeftSideItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
	const QModelIndex& index) const
{
#ifdef LEGACY_PAINT_CODE
	QStyledItemDelegate::paint(painter, option, index);
#else
	Q_ASSERT(index.isValid());

	QStyleOptionViewItem opt = option;
	initStyleOption(&opt, index);

	LEFT_SIDE_TYPE type = (LEFT_SIDE_TYPE)index.data(Qt::UserRole + 1).toInt();

	const QWidget* widget = option.widget;

	painter->save();
	painter->setClipRect(opt.rect);

	const QAbstractItemView* view = qobject_cast<const QAbstractItemView*>(widget);

	QPalette palette = opt.palette;
	palette.setColor(QPalette::All, QPalette::HighlightedText, palette.color(QPalette::Active, QPalette::Text));
	// Note that setting a saturated color here results in ugly XOR colors in the focus rect
	palette.setColor(QPalette::All, QPalette::Highlight, palette.base().color().darker(108));
	opt.palette = palette;

	int icon_xoffset = 21, icon_yoffset = 12;
	int y = opt.rect.y() + icon_yoffset;
	int iconSize = opt.decorationSize.height();
	const int icon2text = 3;

	QRect iconRect(icon_xoffset, y, iconSize, iconSize);
	QRect textRect(icon_xoffset + iconSize + icon2text, y - 8, 94, 30);

	// draw the background
	drawBackground(opt, painter, widget);

	// draw the icon
	QIcon::Mode mode = QIcon::Normal;
	if (!(opt.state & QStyle::State_Enabled))
		mode = QIcon::Disabled;
	else if (opt.state & QStyle::State_Selected)
		mode = QIcon::Selected;
	QIcon::State state = opt.state & QStyle::State_Open ? QIcon::On : QIcon::Off;
	opt.icon.paint(painter, iconRect, opt.decorationAlignment, mode, state);

	// draw the text
	if (!opt.text.isEmpty()) {
		QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled
			? QPalette::Normal : QPalette::Disabled;
		if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
			cg = QPalette::Inactive;

		if (opt.state & QStyle::State_Selected) {
			painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
		}
		else {
			painter->setPen(opt.palette.color(cg, QPalette::Text));
		}
		if (opt.state & QStyle::State_Editing) {
			painter->setPen(opt.palette.color(cg, QPalette::Text));
			painter->drawRect(textRect.adjusted(0, 0, -1, -1));
		}
		viewItemDrawText(painter, &opt, textRect);
	}
	painter->restore();
#endif
}