/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "strokestyleactionswidget.hpp"
#include "item/itemutils.hpp"

StrokeStyleActionsWidget::StrokeStyleActionsWidget(QWidget *parent)
    : QToolButton(parent)
{
}

StrokeStyleActionsWidget::~StrokeStyleActionsWidget() = default;

void StrokeStyleActionsWidget::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);

    const auto strokeStyleString{property("stroke-style").toString()};
    const auto strokeStyle{ItemUtils::convertItemStrokeTypeStringToPenStyle(strokeStyleString)};

    QPainter p(this);

    QRectF r = rect();
    r.adjust(5, 5, -5, -5);

    p.setRenderHint(QPainter::Antialiasing);

    const QPalette pal{palette()};
    p.setPen(QPen(pal.color(QPalette::Text), 2, strokeStyle));
    p.drawLine(r.topLeft(), r.bottomRight());
}

#include "moc_strokestyleactionswidget.cpp"
