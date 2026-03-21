// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "startarrowtypewidget.hpp"
#include <KLocalizedString>
#include <QButtonGroup>
#include <QToolButton>
using namespace Qt::StringLiterals;

StartArrowTypeWidget::StartArrowTypeWidget(QWidget *parent)
    : ArrowTypeWidget{parent}
{
}

QString StartArrowTypeWidget::name() const
{
    return i18nc("Name of a property", "Start Arrow Head");
}

const Property StartArrowTypeWidget::value() const
{
    return Property{m_group->checkedButton()->property("arrow-head").value<QString>(), Property::Type::StartArrowType};
}
