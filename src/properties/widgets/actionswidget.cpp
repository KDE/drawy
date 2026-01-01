// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "actionswidget.hpp"

#include <QBoxLayout>
#include <QPushButton>
#include <QWidget>

#include "../../context/applicationcontext.hpp"
#include "../../context/uicontext.hpp"
#include "../../iconmanager/iconmanager.hpp"
#include "../../keybindings/actionmanager.hpp"
#include "../property.hpp"

ActionsWidget::ActionsWidget(QWidget *parent)
    : PropertyWidget{parent}
{
    m_widget = new QWidget{parent};

    QHBoxLayout *layout{new QHBoxLayout(m_widget)};
    layout->setContentsMargins(0, 0, 0, 0);

    QPushButton *deleteButton{new QPushButton(m_widget)};
    QPushButton *groupButton{new QPushButton(m_widget)};
    QPushButton *ungroupButton{new QPushButton(m_widget)};

    IconManager *iconManager{ApplicationContext::instance()->uiContext()->iconManager()};
    deleteButton->setIcon(iconManager->icon(IconManager::ACTION_DELETE));
    groupButton->setIcon(iconManager->icon(IconManager::ACTION_GROUP));
    ungroupButton->setIcon(iconManager->icon(IconManager::ACTION_UNGROUP));

    deleteButton->setToolTip(tr("Delete selection"));
    groupButton->setToolTip(tr("Group selection"));
    ungroupButton->setToolTip(tr("Ungroup selection"));

    deleteButton->setProperty("class", "drawyPropertyBarActionButton");
    groupButton->setProperty("class", "drawyPropertyBarActionButton");
    ungroupButton->setProperty("class", "drawyPropertyBarActionButton");

    layout->addWidget(deleteButton);
    layout->addWidget(groupButton);
    layout->addWidget(ungroupButton);

    ActionManager *actionManager{ApplicationContext::instance()->uiContext()->actionManager()};

    QObject::connect(deleteButton, &QPushButton::clicked, this, [actionManager]() {
        actionManager->deleteSelection();
    });
    QObject::connect(groupButton, &QPushButton::clicked, this, [actionManager]() {
        actionManager->groupItems();
    });
    QObject::connect(ungroupButton, &QPushButton::clicked, this, [actionManager]() {
        actionManager->ungroupItems();
    });

    m_widget->hide();
}

QString ActionsWidget::name() const
{
    return tr("Actions");
};

const Property ActionsWidget::value() const
{
    return Property{};
}
