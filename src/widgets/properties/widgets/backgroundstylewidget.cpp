/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "backgroundstylewidget.hpp"
#include "buttonactionswidget.hpp"
#include "item/itemutils.hpp"
#include <QButtonGroup>
#include <QHBoxLayout>

using namespace Qt::Literals::StringLiterals;
BackgroundStyleWidget::BackgroundStyleWidget(QWidget *parent)
    : PropertyWidget(parent)
{
    m_widget = new QWidget{parent};
    m_widget->setObjectName(u"m_widget"_s);
    m_group = new QButtonGroup{m_widget};

    auto layout{new QHBoxLayout(m_widget)};
    layout->setObjectName(u"layout"_s);
    layout->setContentsMargins({});
    layout->setAlignment(Qt::AlignLeft);

    auto solidButton{new ButtonActionsWidget(m_widget)};
    solidButton->setObjectName(u"solidButton"_s);
    solidButton->setProperty("background-style", ItemUtils::convertItemBackgroundTypeEnumToString(Item::BackgroundType::Solid));
    solidButton->setToolTip(tr("Solid"));
    solidButton->setCheckable(true);
    // Laurent: wait for icons (from kde team)
    solidButton->setIcon(QIcon::fromTheme(u"line_solid"_s));
    m_group->addButton(solidButton);

    auto diagonalCrossButton{new ButtonActionsWidget(m_widget)};
    diagonalCrossButton->setObjectName(u"diagonalCrossButton"_s);
    diagonalCrossButton->setProperty("background-style", ItemUtils::convertItemBackgroundTypeEnumToString(Item::BackgroundType::CrossDiagonal));
    diagonalCrossButton->setToolTip(tr("Diagonal Cross"));
    diagonalCrossButton->setCheckable(true);
    // Laurent: wait for icons (from kde team)
    diagonalCrossButton->setIcon(QIcon::fromTheme(u"line_solid"_s));
    m_group->addButton(diagonalCrossButton);

    auto diagonalButton{new ButtonActionsWidget(m_widget)};
    diagonalButton->setObjectName(u"diagonalButton"_s);
    diagonalButton->setProperty("background-style", ItemUtils::convertItemBackgroundTypeEnumToString(Item::BackgroundType::Diagonal));
    diagonalButton->setToolTip(tr("Diagonal"));
    diagonalButton->setCheckable(true);
    // Laurent: wait for icons (from kde team)
    diagonalButton->setIcon(QIcon::fromTheme(u"line_solid"_s));
    m_group->addButton(diagonalButton);

    layout->addWidget(solidButton);
    layout->addWidget(diagonalCrossButton);
    layout->addWidget(diagonalButton);

    m_group->button(m_group->id(solidButton))->setChecked(true);
    connect(m_group, &QButtonGroup::idClicked, this, [this]() {
        Q_EMIT changed(value());
    });

    m_widget->hide();
}

QString BackgroundStyleWidget::name() const
{
    return tr("Background Style");
}

const Property BackgroundStyleWidget::value() const
{
    return Property{m_group->checkedButton()->property("background-style"), Property::Type::BackgroundStyle};
}

#include "moc_backgroundstylewidget.cpp"
