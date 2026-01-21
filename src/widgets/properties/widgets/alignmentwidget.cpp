/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "alignmentwidget.hpp"
#include "buttonactionswidget.hpp"
#include "context/applicationcontext.hpp"
#include "context/uicontext.hpp"
#include "iconmanager/iconmanager.hpp"
#include <QGridLayout>
AlignmentWidget::AlignmentWidget(QWidget *parent)
    : PropertyWidget(parent)
{
    m_widget = new QWidget{parent};

    auto layout{new QGridLayout(m_widget)};
    layout->setContentsMargins({});

    auto alignLeftButton{new ButtonActionsWidget(m_widget)};
    auto centralHorizontalButton{new ButtonActionsWidget(m_widget)};
    auto alignRightButton{new ButtonActionsWidget(m_widget)};

    auto alignTopButton{new ButtonActionsWidget(m_widget)};
    auto centralVerticalButton{new ButtonActionsWidget(m_widget)};
    auto alignBottomButton{new ButtonActionsWidget(m_widget)};

    auto iconManager{ApplicationContext::instance()->uiContext()->iconManager()};
    if (iconManager) {
        // TODO fix icons
    }

    layout->addWidget(alignLeftButton, 0, 0);
    layout->addWidget(centralHorizontalButton, 0, 1);
    layout->addWidget(alignRightButton, 0, 2);
    layout->addWidget(alignTopButton, 1, 0);
    layout->addWidget(centralVerticalButton, 1, 1);
    layout->addWidget(alignBottomButton, 1, 2);

    connect(alignLeftButton, &ButtonActionsWidget::clicked, this, []() {
        // TODO add enum for defining action.
    });
    connect(centralHorizontalButton, &ButtonActionsWidget::clicked, this, []() {
        // TODO add enum for defining action.
    });
    connect(alignRightButton, &ButtonActionsWidget::clicked, this, []() {
        // TODO add enum for defining action.
    });
    connect(alignTopButton, &ButtonActionsWidget::clicked, this, []() {
        // TODO add enum for defining action.
    });
    connect(centralVerticalButton, &ButtonActionsWidget::clicked, this, []() {
        // TODO add enum for defining action.
    });
    connect(alignBottomButton, &ButtonActionsWidget::clicked, this, []() {
        // TODO add enum for defining action.
    });

    m_widget->hide();
}

QString AlignmentWidget::name() const
{
    return tr("Alignment");
}

const Property AlignmentWidget::value() const
{
    return {};
}

#include "moc_alignmentwidget.cpp"
