// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "frame.hpp"
#include <QApplication>
#include <QStyle>

using namespace Qt::Literals::StringLiterals;
Frame::Frame(QWidget *parent)
    : QFrame{parent}
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setFrameShape(QFrame::NoFrame);
    setFrameShadow(QFrame::Plain);
    setAutoFillBackground(true);

    const QPalette pal{QApplication::palette()};
    const QColor borderColor{pal.color(QPalette::Light)};

    setObjectName("outlinedFrame");

    const QString frameStyleSheet =
        QString{
            u"#outlinedFrame {"_s
            u"  border: %1px solid %2;"_s
            u"  border-radius: %3px;"_s
            u"  background: %4;"_s
            u"}"_s}
            .arg(1)
            .arg(borderColor.name(QColor::HexArgb))
            .arg(style()->pixelMetric(QStyle::PM_ToolBarItemMargin))
            .arg(pal.color(QPalette::Window).name());

    setStyleSheet(frameStyleSheet);
}
