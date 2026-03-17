/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "arrowstylewidget.hpp"
#include "arrowbuttonactionswidget.hpp"
#include "context/uicontext.hpp"
#include "item/arrowutils.hpp"
#include "keybindings/actionmanager.hpp"
#include <KLocalizedString>
#include <QHBoxLayout>

using namespace Qt::Literals::StringLiterals;
ArrowStyleWidget::ArrowStyleWidget(ActionManager *actionManager, QWidget *parent)
    : PropertyWidget(parent)
    , mStartArrowButton(new ArrowButtonActionsWidget(parent))
    , mEndArrowButton(new ArrowButtonActionsWidget(parent))
{
    m_widget = new QWidget{parent};

    auto layout{new QHBoxLayout(m_widget)};
    layout->setObjectName(u"layout"_s);
    layout->setContentsMargins({});
    layout->setAlignment(Qt::AlignLeft);

    connect(mStartArrowButton, &ArrowButtonActionsWidget::selectedArrow, this, [actionManager](ArrowUtils::ArrowType type) {
        if (actionManager) {
            actionManager->changeArrowType(ArrowUtils::ArrowPos::StartArrow, type);
        }
    });
    connect(mEndArrowButton, &ArrowButtonActionsWidget::selectedArrow, this, [actionManager](ArrowUtils::ArrowType type) {
        if (actionManager) {
            actionManager->changeArrowType(ArrowUtils::ArrowPos::EndArrow, type);
        }
    });

    layout->addWidget(mStartArrowButton);
    layout->addWidget(mEndArrowButton);
    m_widget->hide();
}

QString ArrowStyleWidget::name() const
{
    return i18n("Arrow");
}

const Property ArrowStyleWidget::value() const
{
    return {};
}

#include "moc_arrowstylewidget.cpp"
