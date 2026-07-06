/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "cornerrectangletypewidget.hpp"
#include "buttonactionswidget.hpp"
#include "item/itemutils.hpp"
#include <KLocalizedString>
#include <QButtonGroup>
#include <QHBoxLayout>
using namespace Qt::Literals::StringLiterals;
CornerRectangleTypeWidget::CornerRectangleTypeWidget(QWidget *parent)
    : PropertyWidget(parent)
{
    m_widget = new QWidget{parent};
    m_widget->setObjectName(u"m_widget"_s);
    m_group = new QButtonGroup{m_widget};

    auto layout{new QHBoxLayout(m_widget)};
    layout->setObjectName(u"layout"_s);
    layout->setContentsMargins({});

    auto roundedCornerButton{new ButtonActionsWidget(m_widget)};
    roundedCornerButton->setObjectName(u"roundedCornerButton"_s);
    roundedCornerButton->setToolTip(i18nc("@info:tooltip", "Rounded Corner"));
    roundedCornerButton->setCheckable(true);
    roundedCornerButton->setIcon(QIcon::fromTheme(u"tool_rounded_rectangle"_s));
    roundedCornerButton->setProperty("corner-rectangle-type", ItemUtils::convertItemCornerRectangleTypeEnumToString(Item::CornerRectangleType::Rounded));
    m_group->addButton(roundedCornerButton);

    auto pointedCornerButton{new ButtonActionsWidget(m_widget)};
    pointedCornerButton->setObjectName(u"pointedCornerButton"_s);
    pointedCornerButton->setToolTip(i18nc("@info:tooltip", "Pointed Corner"));
    pointedCornerButton->setCheckable(true);
    pointedCornerButton->setIcon(QIcon::fromTheme(u"tool_rectangle"_s));
    pointedCornerButton->setProperty("corner-rectangle-type", ItemUtils::convertItemCornerRectangleTypeEnumToString(Item::CornerRectangleType::Pointed));
    m_group->addButton(pointedCornerButton);

    layout->addWidget(roundedCornerButton);
    layout->addWidget(pointedCornerButton);

    m_group->button(m_group->id(pointedCornerButton))->setChecked(true);
    connect(m_group, &QButtonGroup::idClicked, this, [this]() {
        Q_EMIT changed(value());
    });
    layout->addStretch(0);
    m_widget->hide();
}

QString CornerRectangleTypeWidget::name() const
{
    return i18n("Corner Type");
}

Property CornerRectangleTypeWidget::value() const
{
    return Property{m_group->checkedButton()->property("corner-rectangle-type"), Property::Type::CornerRectangleType};
}

void CornerRectangleTypeWidget::setValue(const QVariant &val)
{
    const auto grpButtons = m_group->buttons();
    for (auto b : grpButtons) {
        if (b->property("corner-rectangle-type").toString() == val.toString()) {
            b->setChecked(true);
            break;
        }
    }
}

#include "moc_cornerrectangletypewidget.cpp"
