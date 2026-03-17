/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "backgroundstyleactionswidget.h"
#include "item/item.hpp"
#include "item/itemutils.hpp"
#include <QPainter>
#include <QPalette>

BackgroundStyleActionsWidget::BackgroundStyleActionsWidget(QWidget *parent)
    : QToolButton(parent)
{
}

BackgroundStyleActionsWidget::~BackgroundStyleActionsWidget() = default;

void BackgroundStyleActionsWidget::paintEvent(QPaintEvent *event)
{
    QToolButton::paintEvent(event);

    const auto bgStyleString{property("background-style").toString()};
    const auto bgStyle{ItemUtils::convertBackgroundTypeStringToEnum(bgStyleString)};

    QPainter p(this);

    QRectF r = rect();
    r.adjust(5, 5, -5, -5);

    if (bgStyle == Item::BackgroundType::None) {
        p.setPen(Qt::red);
        p.drawLine(r.topLeft(), r.bottomRight());
        return;
    }

    const Qt::BrushStyle style = ItemUtils::convertItemBackgroundTypeStringToBrushStyle(bgStyleString);
    p.setRenderHint(QPainter::Antialiasing);

    QPalette pal{palette()};

    const QBrush backgroundBrush(pal.color(QPalette::Text), style);
    p.fillRect(r, backgroundBrush);
}

#include "moc_backgroundstyleactionswidget.cpp"
