// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "topleftwidgets.hpp"
#include "config-drawy.hpp"
#include "context/helpmenu.hpp"
#include "context/uicontext.hpp"
#include "frame.hpp"
#include "keybindings/actionmanager.hpp"
#include <KActionMenu>
#include <KColorSchemeManager>
#include <KColorSchemeMenu>
#include <KHelpMenu>
#include <KLocalizedString>
#include <QMenu>
#include <QStyle>
#include <QToolButton>

using namespace Qt::StringLiterals;
TopLeftWidgets::TopLeftWidgets(ActionManager *actionManager, QWidget *parent)
    : QWidget{parent}
    , m_layout{new QHBoxLayout{this}}
    , mHelpMenu(new HelpMenu(this))
    , mColorSchemeMenu(KColorSchemeMenu::createMenu(KColorSchemeManager::instance(), this))
{
    m_layout->setContentsMargins(0, 0, 0, 0);

    // menu button
    auto menuFrame{new Frame{this}};
    auto menuFrameLayout = new QHBoxLayout{menuFrame};

    const int iconWidth{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    const QSize iconSize{iconWidth, iconWidth};

    auto menuButton{new QToolButton{menuFrame}};
    menuButton->setIcon(QIcon::fromTheme(u"application-menu"_s));
    menuButton->setAutoRaise(true);
    menuButton->setIconSize(iconSize);
    menuButton->setToolTip(i18nc("@info:tooltip", "Open Menu"));
    menuButton->setPopupMode(QToolButton::InstantPopup);

    auto menu{new QMenu{this}};
    menuButton->setMenu(menu);

    // Add more actions as required
    connect(menu, &QMenu::aboutToShow, this, [menu, actionManager, this]() -> void {
        menu->addAction(actionManager->action(KStandardActions::New));
        menu->addAction(actionManager->action(KStandardAction::OpenRecent));
        menu->addAction(actionManager->action(KStandardActions::Open));
        menu->addAction(actionManager->action(KStandardActions::Save));
        menu->addAction(actionManager->action(KStandardActions::SaveAs));
        menu->addAction(actionManager->action(ActionManager::Action::ExportAsImage));
        menu->addSeparator();
        menu->addAction(actionManager->action(ActionManager::Action::Clear));
        menu->addSeparator();
        menu->addAction(mColorSchemeMenu);
        menu->addAction(actionManager->action(KStandardActions::KeyBindings));
        menu->addAction(actionManager->action(KStandardActions::Preferences));
        menu->addSeparator();
        menu->addMenu(mHelpMenu->menu());
        menu->addSeparator();
#if HAVE_WHATSNEWSNGSUPPORT
        menu->addAction(actionManager->action(ActionManager::Action::WhatsNew));
        menu->addSeparator();
#endif
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
