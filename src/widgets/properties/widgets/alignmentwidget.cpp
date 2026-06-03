/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "alignmentwidget.hpp"
#include "buttonactionswidget.hpp"
#include "keybindings/actionmanager.hpp"

#include <KLocalizedString>
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
    alignLeftButton->setObjectName(u"alignLeftButton"_s);

    auto centralHorizontalButton{new ButtonActionsWidget(m_widget)};
    centralHorizontalButton->setObjectName(u"centralHorizontalButton"_s);

    auto alignRightButton{new ButtonActionsWidget(m_widget)};
    alignRightButton->setObjectName(u"alignRightButton"_s);

    auto alignTopButton{new ButtonActionsWidget(m_widget)};
    alignTopButton->setObjectName(u"alignTopButton"_s);

    auto centralVerticalButton{new ButtonActionsWidget(m_widget)};
    centralVerticalButton->setObjectName(u"centralVerticalButton"_s);

    auto alignBottomButton{new ButtonActionsWidget(m_widget)};
    alignBottomButton->setObjectName(u"alignBottomButton"_s);

    if (actionManager) {
        alignLeftButton->setDefaultAction(actionManager->action(ActionManager::Action::AlignLeft));
        centralHorizontalButton->setDefaultAction(actionManager->action(ActionManager::Action::AlignHorizontalCenter));
        alignRightButton->setDefaultAction(actionManager->action(ActionManager::Action::AlignRight));
        alignTopButton->setDefaultAction(actionManager->action(ActionManager::Action::AlignTop));
        centralVerticalButton->setDefaultAction(actionManager->action(ActionManager::Action::AlignVerticalCenter));
        alignBottomButton->setDefaultAction(actionManager->action(ActionManager::Action::AlignBottom));
    }

    layout->addWidget(alignLeftButton, 0, 0);
    layout->addWidget(centralHorizontalButton, 0, 1);
    layout->addWidget(alignRightButton, 0, 2);
    layout->addWidget(alignTopButton, 1, 0);
    layout->addWidget(centralVerticalButton, 1, 1);
    layout->addWidget(alignBottomButton, 1, 2);

    m_widget->hide();
}

QString AlignmentWidget::name() const
{
    return i18n("Alignment");
}

Property AlignmentWidget::value() const
{
    return {};
}

#include "moc_alignmentwidget.cpp"
