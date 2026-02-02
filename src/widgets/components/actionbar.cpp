// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "actionbar.hpp"
#include "clickablelabel.hpp"
#include "context/applicationcontext.hpp"
#include "context/renderingcontext.hpp"
#include "frame.hpp"
#include <QLabel>
#include <QStyle>
#include <QToolButton>
#include <qpushbutton.h>

using namespace Qt::Literals::StringLiterals;
ActionBar::ActionBar(QWidget *parent)
    : QWidget{parent}
    , m_layout(new QHBoxLayout{this})
{
    auto context = ApplicationContext::instance();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_layout->setContentsMargins(0, 0, 0, 0);

    int iconWidth{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    QSize iconSize{iconWidth, iconWidth};

    // Hamburger menu
    QToolButton *hamburgerMenu = new QToolButton{this};
    hamburgerMenu->setIcon(QIcon::fromTheme(u"application-menu"_s));
    hamburgerMenu->setIconSize(iconSize);

    // Zoom controls
    Frame *zoomControlFrame = new Frame{this};

    QHBoxLayout *zoomControlLayout = new QHBoxLayout{zoomControlFrame};

    QToolButton *zoomOutButton = new QToolButton{zoomControlFrame};
    zoomOutButton->setAutoRaise(true);
    zoomOutButton->setIcon(QIcon::fromTheme(u"value-decrease"_s));
    zoomOutButton->setIconSize(iconSize);

    connect(zoomOutButton, &QToolButton::clicked, this, [context]() {
        context->renderingContext()->zoomOut();
    });

    ClickableLabel *zoomLabel = new ClickableLabel{zoomControlFrame};
    zoomLabel->setText(u"100%"_s);
    zoomLabel->setToolTip(tr("Reset Zoom"));

    connect(zoomLabel, &ClickableLabel::clicked, this, [context]() {
        context->renderingContext()->updateZoomFactor(1);
    });

    QToolButton *zoomInButton = new QToolButton{zoomControlFrame};
    zoomInButton->setAutoRaise(true);
    zoomInButton->setIcon(QIcon::fromTheme(u"value-increase"_s));
    zoomInButton->setIconSize(iconSize);

    connect(zoomInButton, &QToolButton::clicked, this, [context]() {
        context->renderingContext()->zoomIn();
    });

    connect(context->renderingContext(), &RenderingContext::zoomFactorChanged, this, [zoomLabel](qreal newZoomFactor) {
        int zoomValue{qRound(newZoomFactor * 100)};
        zoomLabel->setText(QString(u"%1%"_s).arg(zoomValue));
    });

    zoomControlLayout->setSpacing(style()->pixelMetric(QStyle::PM_ToolBarItemMargin));
    zoomControlLayout->addWidget(zoomOutButton);
    zoomControlLayout->addWidget(zoomLabel);
    zoomControlLayout->addWidget(zoomInButton);

    m_layout->addWidget(zoomControlFrame, 0, Qt::AlignLeft);
}

#include "moc_actionbar.cpp"
