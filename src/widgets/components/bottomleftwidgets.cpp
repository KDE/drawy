// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "bottomleftwidgets.hpp"
#include "clickablelabel.hpp"
#include "context/applicationcontext.hpp"
#include "context/renderingcontext.hpp"
#include "frame.hpp"
#include <QLabel>
#include <QStyle>
#include <QToolButton>
#include <qpushbutton.h>

using namespace Qt::Literals::StringLiterals;
BottomLeftWidgets::BottomLeftWidgets(QWidget *parent)
    : QWidget{parent}
    , m_layout(new QHBoxLayout{this})
{
    auto context{ApplicationContext::instance()};

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_layout->setContentsMargins(0, 0, 0, 0);

    const int iconWidth{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    const QSize iconSize{iconWidth, iconWidth};

    // Hamburger menu
    auto hamburgerMenu{new QToolButton{this}};
    hamburgerMenu->setIcon(QIcon::fromTheme(u"application-menu"_s));
    hamburgerMenu->setIconSize(iconSize);

    // Zoom controls
    auto zoomControlFrame{new Frame{this}};
    auto zoomControlLayout{new QHBoxLayout{zoomControlFrame}};

    auto zoomOutButton{new QToolButton{zoomControlFrame}};
    zoomOutButton->setAutoRaise(true);
    zoomOutButton->setIcon(QIcon::fromTheme(u"value-decrease"_s));
    zoomOutButton->setIconSize(iconSize);
    zoomOutButton->setToolTip(tr("Zoom Out"));

    connect(zoomOutButton, &QToolButton::clicked, this, [context]() {
        context->renderingContext()->zoomOut();
    });

    auto zoomLabel{new ClickableLabel{zoomControlFrame}};
    zoomLabel->setText(u"100%"_s);
    zoomLabel->setToolTip(tr("Reset Zoom"));

    connect(zoomLabel, &ClickableLabel::clicked, this, [context]() {
        context->renderingContext()->updateZoomFactor(1);
    });

    auto zoomInButton{new QToolButton{zoomControlFrame}};
    zoomInButton->setAutoRaise(true);
    zoomInButton->setIcon(QIcon::fromTheme(u"value-increase"_s));
    zoomInButton->setIconSize(iconSize);
    zoomInButton->setToolTip(tr("Zoom In"));

    connect(zoomInButton, &QToolButton::clicked, this, [context]() {
        context->renderingContext()->zoomIn();
    });

    connect(context->renderingContext(), &RenderingContext::zoomFactorChanged, this, [zoomLabel, context, zoomInButton, zoomOutButton](qreal newZoomFactor) {
        const int zoomValue{qRound(newZoomFactor * 100)};
        zoomLabel->setText(QString(u"%1%"_s).arg(zoomValue));

        if (context->renderingContext()->canZoomIn()) {
            zoomInButton->setEnabled(true);
        } else {
            zoomInButton->setEnabled(false);
        }

        if (context->renderingContext()->canZoomOut()) {
            zoomOutButton->setEnabled(true);
        } else {
            zoomOutButton->setEnabled(false);
        }
    });

    zoomControlLayout->setSpacing(style()->pixelMetric(QStyle::PM_ToolBarItemMargin));
    zoomControlLayout->addWidget(zoomOutButton);
    zoomControlLayout->addWidget(zoomLabel);
    zoomControlLayout->addWidget(zoomInButton);

    m_layout->addWidget(zoomControlFrame, 0, Qt::AlignLeft);
}

#include "moc_bottomleftwidgets.cpp"
