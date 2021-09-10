#include "stdafx.h"
#include "toolbuttonstyleoption.h"

StyleOptionToolButton::StyleOptionToolButton()
	: buttonStyle(0)
	, roundCorner(0)
	, orientation(Qt::Horizontal)
	, hideText(false)
	, largeMargins(false)
	, triggerAble(false)
	, buttonEnabled(true)
	, bDown(false)
	, bgBrush(Qt::NoBrush)
	, bHasDisabledBound(false)
	, bEmbeded(false)
	, isRemark(false)
	, isTouchScreenMode(false)
	, isSplitButtonLarge(false)
	, isRecommend(false)
{
	this->type = Type;
}
