/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "arrowstyleactionswidget.h"

ArrowStyleActionsWidget::ArrowStyleActionsWidget(ArrowUtils::ArrowType arrowStyle, QWidget *parent)
    : QToolButton(parent)
    , mArrowStyle(arrowStyle)
{
    setAutoRaise(true);
    setToolTip(ArrowUtils::tooltipFromArrowType(arrowStyle));
}

ArrowStyleActionsWidget::~ArrowStyleActionsWidget() = default;
