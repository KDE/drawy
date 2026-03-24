// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "startarrowtypewidget.hpp"
#include "item/arrow/arrowutils.hpp"
#include <KLocalizedString>
#include <QButtonGroup>
#include <QToolButton>
using namespace Qt::StringLiterals;

StartArrowTypeWidget::StartArrowTypeWidget(ApplicationContext *context, QWidget *parent)
    : ArrowTypeWidget{context, parent}
{
    // default value
    setValue(ArrowUtils::toString(ArrowHead::Type::None));
}

QString StartArrowTypeWidget::name() const
{
    return i18nc("Name of a property", "Start Arrow Head");
}

const Property StartArrowTypeWidget::value() const
{
    return Property{m_group->checkedButton()->property("arrow-head").value<QString>(), Property::Type::StartArrowType};
}
