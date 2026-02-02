// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "clickablelabel.hpp"

ClickableLabel::ClickableLabel(QWidget *parent)
    : QLabel{parent}
{
    setCursor(Qt::PointingHandCursor);
}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    Q_EMIT clicked(event);
}
