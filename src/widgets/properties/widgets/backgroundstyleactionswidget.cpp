/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "backgroundstyleactionswidget.h"
#include "item/itemutils.hpp"
#include <QPainter>

BackgroundStyleActionsWidget::BackgroundStyleActionsWidget(QWidget *parent)
    : QToolButton(parent)
{
}

BackgroundStyleActionsWidget::~BackgroundStyleActionsWidget() = default;

void BackgroundStyleActionsWidget::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);
    const Qt::BrushStyle style = ItemUtils::convertItemBackgroundTypeStringToBrushStyle(property("background-style").toString());
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    const QBrush backgroundBrush(Qt::blue, style);
    QRectF r = rect();
    r.adjust(5, 5, -5, -5);
    p.fillRect(r, backgroundBrush);
}

#include "moc_backgroundstyleactionswidget.cpp"
