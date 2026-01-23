/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "zorderwidget.hpp"
#include "buttonactionswidget.hpp"
#include "context/applicationcontext.hpp"
#include "context/uicontext.hpp"
#include "iconmanager/iconmanager.hpp"
#include <QHBoxLayout>
ZOrderWidget::ZOrderWidget(QWidget *parent)
    : PropertyWidget(parent)
{
    m_widget = new QWidget{parent};

    auto layout{new QHBoxLayout(m_widget)};
    layout->setContentsMargins({});

    auto moveOnTopButton{new ButtonActionsWidget(m_widget)};
    auto moveOnBottomButton{new ButtonActionsWidget(m_widget)};
    auto moveUpButton{new ButtonActionsWidget(m_widget)};
    auto moveDownButton{new ButtonActionsWidget(m_widget)};

    auto iconManager{ApplicationContext::instance()->uiContext()->iconManager()};
    if (iconManager) {
        // TODO fix icons
        moveOnTopButton->setIcon(iconManager->icon(IconManager::Icon::ACTION_DELETE));
        moveOnBottomButton->setIcon(iconManager->icon(IconManager::Icon::ACTION_GROUP));
        moveUpButton->setIcon(iconManager->icon(IconManager::Icon::ACTION_UNGROUP));
        moveDownButton->setIcon(iconManager->icon(IconManager::Icon::ACTION_UNGROUP));
    }

    moveOnTopButton->setToolTip(tr("Move on Top"));
    moveUpButton->setToolTip(tr("Move Up"));
    moveDownButton->setToolTip(tr("Move Down"));
    moveOnBottomButton->setToolTip(tr("Move on Bottom"));

    layout->addWidget(moveOnTopButton);
    layout->addWidget(moveUpButton);
    layout->addWidget(moveDownButton);
    layout->addWidget(moveOnBottomButton);

    // auto
    // actionManager{ApplicationContext::instance()->uiContext()->actionManager()};

    connect(moveOnTopButton, &ButtonActionsWidget::clicked, this, []() {
        // TODO add enum for defining action.
        // actionManager->deleteSelection();
    });
    connect(moveUpButton, &ButtonActionsWidget::clicked, this, []() {
        // actionManager->deleteSelection();
    });
    connect(moveDownButton, &ButtonActionsWidget::clicked, this, []() {
        // actionManager->deleteSelection();
    });
    connect(moveOnBottomButton, &ButtonActionsWidget::clicked, this, []() {
        // actionManager->deleteSelection();
    });

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
