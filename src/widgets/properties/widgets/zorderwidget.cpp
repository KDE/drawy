/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "zorderwidget.hpp"
#include "buttonactionswidget.hpp"
#include "context/uicontext.hpp"
#include "keybindings/actionmanager.hpp"
#include <QHBoxLayout>

ZOrderWidget::ZOrderWidget(ActionManager *actionManager, QWidget *parent)
    : PropertyWidget(parent)
{
    m_widget = new QWidget{parent};
    m_widget->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    auto layout{new QHBoxLayout(m_widget)};
    layout->setContentsMargins({});

    auto moveOnTopButton{new ButtonActionsWidget(m_widget)};
    auto moveOnBottomButton{new ButtonActionsWidget(m_widget)};
    auto moveUpButton{new ButtonActionsWidget(m_widget)};
    auto moveDownButton{new ButtonActionsWidget(m_widget)};

    layout->addWidget(moveOnTopButton);
    layout->addWidget(moveUpButton);
    layout->addWidget(moveDownButton);
    layout->addWidget(moveOnBottomButton);

    layout->addStretch(1);

    if (actionManager) {
        moveOnBottomButton->addAction(actionManager->action(ActionManager::Action::SendToBack));
        moveOnTopButton->addAction(actionManager->action(ActionManager::Action::BringToFront));
        moveUpButton->addAction(actionManager->action(ActionManager::Action::BringForward));
        moveDownButton->addAction(actionManager->action(ActionManager::Action::SendBackward));
    }

    m_widget->hide();
}

QString ZOrderWidget::name() const
{
    return tr("Z-order");
}

const Property ZOrderWidget::value() const
{
    return {};
}

#include "moc_zorderwidget.cpp"
