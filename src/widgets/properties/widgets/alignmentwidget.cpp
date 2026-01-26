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
AlignmentWidget::AlignmentWidget(QWidget *parent)
    : PropertyWidget(parent)
{
    m_widget = new QWidget{parent};
    m_widget->setObjectName(u"m_widget"_s);

    auto layout{new QGridLayout(m_widget)};
    layout->setObjectName(u"layout"_s);
    layout->setContentsMargins({});
    layout->setSpacing(0);

    auto alignLeftButton{new ButtonActionsWidget(m_widget)};
    alignLeftButton->setIcon(QIcon::fromTheme(u"align-horizontal-left"_s));
    alignLeftButton->setObjectName(u"alignLeftButton"_s);
    alignLeftButton->setToolTip(tr("Align to Left"));

    auto centralHorizontalButton{new ButtonActionsWidget(m_widget)};
    centralHorizontalButton->setIcon(QIcon::fromTheme(u"align-horizontal-center"_s));
    centralHorizontalButton->setObjectName(u"centralHorizontalButton"_s);
    centralHorizontalButton->setToolTip(tr("Align Horizontal Center"));

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
    centralVerticalButton->setToolTip(tr("Align Vertical Center"));

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

    auto actionManager{ApplicationContext::instance()->uiContext()->actionManager()};
    connect(alignLeftButton, &ButtonActionsWidget::clicked, this, [actionManager]() {
        actionManager->alignItems(ActionManager::AlignType::AlignLeft);
    });
    connect(centralHorizontalButton, &ButtonActionsWidget::clicked, this, [actionManager]() {
        actionManager->alignItems(ActionManager::AlignType::CentralHorizontal);
    });
    connect(alignRightButton, &ButtonActionsWidget::clicked, this, [actionManager]() {
        actionManager->alignItems(ActionManager::AlignType::AlignRight);
    });
    connect(alignTopButton, &ButtonActionsWidget::clicked, this, [actionManager]() {
        actionManager->alignItems(ActionManager::AlignType::AlignTop);
    });
    connect(centralVerticalButton, &ButtonActionsWidget::clicked, this, [actionManager]() {
        actionManager->alignItems(ActionManager::AlignType::CentralVertical);
    });
    connect(alignBottomButton, &ButtonActionsWidget::clicked, this, [actionManager]() {
        actionManager->alignItems(ActionManager::AlignType::AlignBottom);
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
