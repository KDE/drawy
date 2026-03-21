// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "endarrowtypewidget.hpp"
#include "item/arrow/arrowutils.hpp"
#include <KLocalizedString>
#include <QButtonGroup>
#include <QToolButton>
using namespace Qt::StringLiterals;

EndArrowTypeWidget::EndArrowTypeWidget(QWidget *parent)
    : ArrowTypeWidget{parent}
{
    setValue(ArrowUtils::toString(ArrowHead::Type::Open));
}

QString EndArrowTypeWidget::name() const
{
    return i18nc("Name of a property", "End Arrow Head");
}

const Property EndArrowTypeWidget::value() const
{
    return Property{m_group->checkedButton()->property("arrow-head").value<QString>(), Property::Type::EndArrowType};
}
