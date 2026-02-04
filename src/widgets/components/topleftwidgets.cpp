// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "topleftwidgets.hpp"
#include "context/applicationcontext.hpp"
#include "context/uicontext.hpp"
#include "frame.hpp"
#include "keybindings/actionmanager.hpp"
#include <QHBoxLayout>
#include <QMenu>
#include <QStyle>
#include <QToolButton>

using namespace Qt::StringLiterals;
TopLeftWidgets::TopLeftWidgets(QWidget *parent)
    : QWidget{parent}
    , m_layout{new QHBoxLayout{this}}
{
    m_layout->setContentsMargins(0, 0, 0, 0);

    auto context{ApplicationContext::instance()};
    auto actionManager{context->uiContext()->actionManager()};

    // menu button
    auto menuFrame{new Frame{this}};
    auto menuFrameLayout = new QHBoxLayout{menuFrame};

    const int iconWidth{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    const QSize iconSize{iconWidth, iconWidth};

    auto menuButton{new QToolButton{menuFrame}};
    menuButton->setIcon(QIcon::fromTheme(u"application-menu"_s));
    menuButton->setAutoRaise(true);
    menuButton->setIconSize(iconSize);
    menuButton->setToolTip(tr("Open Menu"));
    menuButton->setPopupMode(QToolButton::InstantPopup);

    auto menu{new QMenu{this}};
    menuButton->setMenu(menu);

    // Add more actions as required
    connect(menu, &QMenu::aboutToShow, this, [menu, actionManager]() -> void {
        menu->addAction(actionManager->action(KStandardActions::New));
        menu->addAction(actionManager->action(KStandardActions::Open));
        menu->addAction(actionManager->action(KStandardActions::Save));
        menu->addAction(actionManager->action(KStandardActions::SaveAs));
        menu->addSeparator();
        menu->addAction(actionManager->action(ActionManager::Action::ExportAsSVG));
        menu->addSeparator();
        menu->addAction(actionManager->action(KStandardActions::Preferences));
        menu->addAction(actionManager->action(KStandardActions::Quit));
    });

    // separator
    auto vLine{new QFrame{menuFrame}};
    vLine->setFrameShape(QFrame::VLine);
    vLine->setFixedHeight(menuButton->height());

    // save controls
    auto saveButton{new QToolButton{menuFrame}};
    saveButton->setAutoRaise(true);
    saveButton->setIconSize(iconSize);
    saveButton->setDefaultAction(actionManager->action(KStandardActions::Save));

    auto loadButton{new QToolButton{menuFrame}};
    loadButton->setAutoRaise(true);
    loadButton->setIconSize(iconSize);
    loadButton->setDefaultAction(actionManager->action(KStandardActions::Open));

    menuFrameLayout->addWidget(menuButton);
    menuFrameLayout->addWidget(vLine);
    menuFrameLayout->addWidget(saveButton);
    menuFrameLayout->addWidget(loadButton);

    m_layout->addWidget(menuFrame);
}

#include "moc_topleftwidgets.cpp"
