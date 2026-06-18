// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "actionswidget.hpp"

#include <KLocalizedString>
#include <QHBoxLayout>
#include <QWidget>

#include "buttonactionswidget.hpp"

#include "keybindings/actionmanager.hpp"
#include "properties/property.hpp"
using namespace Qt::Literals::StringLiterals;

ActionsWidget::ActionsWidget(ActionManager *actionManager, QWidget *parent)
    : PropertyWidget{parent}
{
    m_widget = new QWidget{parent};

    auto layout{new QHBoxLayout(m_widget)};
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignLeft);

    auto deleteButton{new ButtonActionsWidget(m_widget)};
    auto groupButton{new ButtonActionsWidget(m_widget)};
    auto ungroupButton{new ButtonActionsWidget(m_widget)};

    deleteButton->setToolTip(i18nc("@info:tooltip", "Delete selection"));
    groupButton->setToolTip(i18nc("@info:tooltip", "Group selection"));
    ungroupButton->setToolTip(i18nc("@info:tooltip", "Ungroup selection"));

    layout->addWidget(deleteButton);
    layout->addWidget(groupButton);
    layout->addWidget(ungroupButton);

    if (actionManager) {
        deleteButton->setDefaultAction(actionManager->action(ActionManager::Action::DeleteSelection));
        groupButton->setDefaultAction(actionManager->action(ActionManager::Action::GroupItems));
        ungroupButton->setDefaultAction(actionManager->action(ActionManager::Action::UngroupItems));
    }

    m_widget->hide();
}

QString ActionsWidget::name() const
{
    return i18n("Actions");
}

Property ActionsWidget::value() const
{
    return Property{};
}

#include "moc_actionswidget.cpp"
