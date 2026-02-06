/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "alignmentwidget.hpp"
#include "buttonactionswidget.hpp"
#include "context/applicationcontext.hpp"
#include "context/uicontext.hpp"
#include "keybindings/actionmanager.hpp"

#include <QGridLayout>
using namespace Qt::Literals::StringLiterals;
AlignmentWidget::AlignmentWidget(ActionManager *actionManager, QWidget *parent)
    : PropertyWidget(parent)
{
    m_widget = new QWidget{parent};
    m_widget->setObjectName(u"m_widget"_s);

    auto layout{new QGridLayout(m_widget)};
    layout->setObjectName(u"layout"_s);
    layout->setContentsMargins({});
    layout->setAlignment(Qt::AlignLeft);

    auto alignLeftButton{new ButtonActionsWidget(m_widget)};
    alignLeftButton->setIcon(QIcon::fromTheme(u"align-horizontal-left"_s));
    alignLeftButton->setObjectName(u"alignLeftButton"_s);
    alignLeftButton->setToolTip(tr("Align to Left"));

    auto centralHorizontalButton{new ButtonActionsWidget(m_widget)};
    centralHorizontalButton->setIcon(QIcon::fromTheme(u"align-horizontal-center"_s));
    centralHorizontalButton->setObjectName(u"centralHorizontalButton"_s);
    centralHorizontalButton->setToolTip(tr("Align to Center"));

    auto alignRightButton{new ButtonActionsWidget(m_widget)};
    alignRightButton->setIcon(QIcon::fromTheme(u"align-horizontal-right"_s));
    alignRightButton->setObjectName(u"alignRightButton"_s);
    alignRightButton->setToolTip(tr("Align to Right"));

    auto alignTopButton{new ButtonActionsWidget(m_widget)};
    alignTopButton->setIcon(QIcon::fromTheme(u"align-vertical-top"_s));
    alignTopButton->setObjectName(u"alignTopButton"_s);
    alignTopButton->setToolTip(tr("Align to Top"));

    auto centralVerticalButton{new ButtonActionsWidget(m_widget)};
    centralVerticalButton->setIcon(QIcon::fromTheme(u"align-vertical-center"_s));
    centralVerticalButton->setObjectName(u"centralVerticalButton"_s);
    centralVerticalButton->setToolTip(tr("Align to Middle"));

    auto alignBottomButton{new ButtonActionsWidget(m_widget)};
    alignBottomButton->setIcon(QIcon::fromTheme(u"align-vertical-bottom"_s));
    alignBottomButton->setObjectName(u"alignBottomButton"_s);
    alignBottomButton->setToolTip(tr("Align to Bottom"));

    layout->addWidget(alignLeftButton, 0, 0);
    layout->addWidget(centralHorizontalButton, 0, 1);
    layout->addWidget(alignRightButton, 0, 2);
    layout->addWidget(alignTopButton, 1, 0);
    layout->addWidget(centralVerticalButton, 1, 1);
    layout->addWidget(alignBottomButton, 1, 2);

    connect(alignLeftButton, &ButtonActionsWidget::clicked, this, [actionManager]() {
        actionManager->alignItems(ItemUtils::AlignType::AlignLeft);
    });
    connect(centralHorizontalButton, &ButtonActionsWidget::clicked, this, [actionManager]() {
        actionManager->alignItems(ItemUtils::AlignType::AlignHorizontalCenter);
    });
    connect(alignRightButton, &ButtonActionsWidget::clicked, this, [actionManager]() {
        actionManager->alignItems(ItemUtils::AlignType::AlignRight);
    });
    connect(alignTopButton, &ButtonActionsWidget::clicked, this, [actionManager]() {
        actionManager->alignItems(ItemUtils::AlignType::AlignTop);
    });
    connect(centralVerticalButton, &ButtonActionsWidget::clicked, this, [actionManager]() {
        actionManager->alignItems(ItemUtils::AlignType::AlignVerticalCenter);
    });
    connect(alignBottomButton, &ButtonActionsWidget::clicked, this, [actionManager]() {
        actionManager->alignItems(ItemUtils::AlignType::AlignBottom);
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
