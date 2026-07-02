// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "bottomleftwidgets.hpp"
#include "clickablelabel.hpp"
#include "context/uicontext.hpp"
#include "frame.hpp"
#include "keybindings/actionmanager.hpp"
#include <KLocalizedString>
#include <QStyle>
#include <QToolButton>

using namespace Qt::Literals::StringLiterals;
BottomLeftWidgets::BottomLeftWidgets(ActionManager *actionManager, QWidget *parent)
    : QWidget{parent}
    , m_layout(new QHBoxLayout{this})
{
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
    zoomOutButton->setIconSize(iconSize);
    zoomOutButton->setDefaultAction(actionManager->action(KStandardActions::ZoomOut));

    auto zoomLabel{new ClickableLabel{zoomControlFrame}};
    zoomLabel->setText(i18nc("@label, percent text", "100%"));
    zoomLabel->setToolTip(i18nc("@info:tooltip", "Reset Zoom"));

    connect(zoomLabel, &ClickableLabel::clicked, this, &BottomLeftWidgets::resetZoom);

    auto zoomInButton{new QToolButton{zoomControlFrame}};
    zoomInButton->setAutoRaise(true);
    zoomInButton->setIconSize(iconSize);
    zoomInButton->setDefaultAction(actionManager->action(KStandardActions::ZoomIn));

    connect(this, &BottomLeftWidgets::zoomFactorChanged, this, [zoomLabel](qreal newZoomFactor) {
        const int zoomValue{qRound(newZoomFactor * 100)};
        zoomLabel->setText(i18nc("@label, percent value", "%1%", zoomValue));
    });

    zoomControlLayout->setSpacing(style()->pixelMetric(QStyle::PM_ToolBarItemMargin));
    zoomControlLayout->addWidget(zoomOutButton);
    zoomControlLayout->addWidget(zoomLabel);
    zoomControlLayout->addWidget(zoomInButton);

    m_layout->addWidget(zoomControlFrame, 0, Qt::AlignLeft);
}

#include "moc_bottomleftwidgets.cpp"
