// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "actionswidget.hpp"

#include <QBoxLayout>
#include <QPushButton>
#include <QWidget>

#include "buttonactionswidget.hpp"
#include "context/applicationcontext.hpp"
#include "context/uicontext.hpp"
#include "iconmanager/iconmanager.hpp"
#include "keybindings/actionmanager.hpp"
#include "properties/property.hpp"
using namespace Qt::Literals::StringLiterals;
ActionsWidget::ActionsWidget(QWidget *parent)
    : PropertyWidget{parent}
{
    m_widget = new QWidget{parent};

    QHBoxLayout *layout{new QHBoxLayout(m_widget)};
    layout->setContentsMargins(0, 0, 0, 0);

    auto deleteButton{new ButtonActionsWidget(m_widget)};
    auto groupButton{new ButtonActionsWidget(m_widget)};
    auto ungroupButton{new ButtonActionsWidget(m_widget)};

    IconManager *iconManager{ApplicationContext::instance()->uiContext()->iconManager()};
    deleteButton->setIcon(iconManager->icon(IconManager::Icon::ACTION_DELETE));
    groupButton->setIcon(iconManager->icon(IconManager::Icon::ACTION_GROUP));
    ungroupButton->setIcon(iconManager->icon(IconManager::Icon::ACTION_UNGROUP));

    deleteButton->setToolTip(tr("Delete selection"));
    groupButton->setToolTip(tr("Group selection"));
    ungroupButton->setToolTip(tr("Ungroup selection"));

    layout->addWidget(deleteButton);
    layout->addWidget(groupButton);
    layout->addWidget(ungroupButton);

    ActionManager &actionManager{ApplicationContext::instance()->uiContext()->actionManager()};

    connect(deleteButton, &ButtonActionsWidget::clicked, this, [&]() {
        actionManager.deleteSelection();
    });
    connect(groupButton, &ButtonActionsWidget::clicked, this, [&]() {
        actionManager.groupItems();
    });
    connect(ungroupButton, &ButtonActionsWidget::clicked, this, [&]() {
        actionManager.ungroupItems();
    });

    m_widget->hide();
}

QString ActionsWidget::name() const
{
    return tr("Actions");
}

const Property ActionsWidget::value() const
{
    return Property{};
}

#include "moc_actionswidget.cpp"
