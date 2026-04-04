/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "stokestylewidget.hpp"
#include "strokestyleactionswidget.hpp"
#include <KLocalizedString>
#include <QButtonGroup>
#include <QHBoxLayout>

using namespace Qt::Literals::StringLiterals;
StokeStyleWidget::StokeStyleWidget(QWidget *parent)
    : PropertyWidget{parent}
{
    m_widget = new QWidget{parent};
    m_widget->setObjectName(u"m_widget"_s);
    m_group = new QButtonGroup{m_widget};

    auto layout{new QHBoxLayout(m_widget)};
    layout->setObjectName(u"layout"_s);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignLeft);

    auto solidButton{new StrokeStyleActionsWidget(m_widget)};
    solidButton->setObjectName(u"solidButton"_s);
    solidButton->setProperty("stroke-style", u"Solid"_s);
    solidButton->setToolTip(i18nc("@info:tooltip", "Solid"));
    solidButton->setCheckable(true);
    m_group->addButton(solidButton);

    auto dashLineButton{new StrokeStyleActionsWidget(m_widget)};
    dashLineButton->setObjectName(u"dashLineButton"_s);
    dashLineButton->setProperty("stroke-style", u"DashLine"_s);
    dashLineButton->setToolTip(i18nc("@info:tooltip", "Dashed"));
    dashLineButton->setCheckable(true);
    m_group->addButton(dashLineButton);

    auto dotLineButton{new StrokeStyleActionsWidget(m_widget)};
    dotLineButton->setObjectName(u"dotLineButton"_s);
    dotLineButton->setProperty("stroke-style", u"DotLine"_s);
    dotLineButton->setToolTip(i18nc("@info:tooltip", "Dotted"));
    dotLineButton->setCheckable(true);
    m_group->addButton(dotLineButton);

    m_group->button(m_group->id(solidButton))->setChecked(true);

    layout->addWidget(solidButton);
    layout->addWidget(dashLineButton);
    layout->addWidget(dotLineButton);

    connect(m_group, &QButtonGroup::idClicked, this, [this]() {
        Q_EMIT changed(value());
    });

    m_widget->hide();
}

StokeStyleWidget::~StokeStyleWidget() = default;

QString StokeStyleWidget::name() const
{
    return i18n("Stroke Style");
}

const Property StokeStyleWidget::value() const
{
    return Property{m_group->checkedButton()->property("stroke-style"), Property::Type::StrokeStyle};
}

void StokeStyleWidget::setValue(const QVariant &val)
{
    const auto buttons{m_group->buttons()};
    for (const auto b : std::as_const(buttons)) {
        if (b->property("stroke-style").toString() == val.toString()) {
            b->setChecked(true);
            break;
        }
    }
}

#include "moc_stokestylewidget.cpp"
