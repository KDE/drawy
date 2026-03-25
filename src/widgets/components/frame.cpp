// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "frame.hpp"
#include <QEvent>
#include <QPainter>
#include <QStyle>

using namespace Qt::Literals::StringLiterals;
Frame::Frame(QWidget *parent)
    : QFrame(parent)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setFrameShape(QFrame::NoFrame);
    setAutoFillBackground(false);

    setAttribute(Qt::WA_Hover, true);
    setFocusPolicy(Qt::StrongFocus);
}

void Frame::paintEvent([[maybe_unused]] QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const QPalette pal = palette();

    const QColor background = pal.color(QPalette::Window);
    const bool isLightBackground = background.lightnessF() > 0.5;
    QColor borderColor = isLightBackground ? pal.color(QPalette::Dark) : pal.color(QPalette::Light);

    const int radius = style()->pixelMetric(QStyle::PM_ToolBarItemMargin, nullptr, this);

    QRectF r = rect();

    // High-DPI 1-physical-pixel border
    const qreal dpr = devicePixelRatioF();
    const qreal penWidth = 1.0 / dpr;

    r.adjust(penWidth / 2.0, penWidth / 2.0, -penWidth / 2.0, -penWidth / 2.0);

    // Background
    p.setPen(Qt::NoPen);
    p.setBrush(background);
    p.drawRoundedRect(r, radius, radius);

    // Border
    QPen pen(borderColor);
    pen.setWidthF(penWidth);
    p.setPen(pen);
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(r, radius, radius);
}

void Frame::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::PaletteChange || event->type() == QEvent::StyleChange) {
        update();
    }

    QFrame::changeEvent(event);
}
#include "moc_frame.cpp"
