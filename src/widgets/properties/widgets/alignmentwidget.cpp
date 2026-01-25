/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "alignmentwidget.hpp"
#include "buttonactionswidget.hpp"
#include <QGridLayout>
using namespace Qt::Literals::StringLiterals;
AlignmentWidget::AlignmentWidget(QWidget *parent)
    : PropertyWidget(parent)
{
    m_widget = new QWidget{parent};

    auto layout{new QGridLayout(m_widget)};
    layout->setContentsMargins({});

    auto alignLeftButton{new ButtonActionsWidget(m_widget)};
    alignLeftButton->setIcon(QIcon::fromTheme(u"align-horizontal-left"_s));
    auto centralHorizontalButton{new ButtonActionsWidget(m_widget)};
    centralHorizontalButton->setIcon(QIcon::fromTheme(u"align-horizontal-center"_s));
    auto alignRightButton{new ButtonActionsWidget(m_widget)};
    alignRightButton->setIcon(QIcon::fromTheme(u"align-horizontal-right"_s));

    auto alignTopButton{new ButtonActionsWidget(m_widget)};
    alignTopButton->setIcon(QIcon::fromTheme(u"align-vertical-top"_s));
    auto centralVerticalButton{new ButtonActionsWidget(m_widget)};
    centralVerticalButton->setIcon(QIcon::fromTheme(u"align-vertical-center"_s));
    auto alignBottomButton{new ButtonActionsWidget(m_widget)};
    alignBottomButton->setIcon(QIcon::fromTheme(u"align-vertical-bottom"_s));

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
