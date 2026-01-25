/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "stokestylewidget.hpp"
#include "buttonactionswidget.hpp"
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

    auto solidButton{new ButtonActionsWidget(m_widget)};
    solidButton->setObjectName(u"solidButton"_s);
    solidButton->setProperty("stroke-style", u"Solid"_s);
    solidButton->setToolTip(tr("Solid"));
    solidButton->setCheckable(true);
    // Laurent: wait for icons (from kde team)
    solidButton->setIcon(QIcon::fromTheme(u"line_solid"_s));
    m_group->addButton(solidButton);

    auto dashLineButton{new ButtonActionsWidget(m_widget)};
    dashLineButton->setObjectName(u"dashLineButton"_s);
    dashLineButton->setProperty("stroke-style", u"DashLine"_s);
    dashLineButton->setToolTip(tr("Dash Line"));
    // Laurent: wait for icons (from kde team)
    dashLineButton->setIcon(QIcon::fromTheme(u"line_dashed"_s));
    dashLineButton->setCheckable(true);
    m_group->addButton(dashLineButton);

    auto dotLineButton{new ButtonActionsWidget(m_widget)};
    dotLineButton->setObjectName(u"dotLineButton"_s);
    dotLineButton->setProperty("stroke-style", u"DotLine"_s);
    dotLineButton->setToolTip(tr("Dot Line"));
    // Laurent: wait for icons (from kde team)
    dotLineButton->setIcon(QIcon::fromTheme(u"line_dotter"_s));
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
    return tr("Stroke Style");
}

const Property StokeStyleWidget::value() const
{
    return Property{m_group->checkedButton()->property("stroke-style"), Property::Type::StrokeStyle};
}

#include "moc_stokestylewidget.cpp"
