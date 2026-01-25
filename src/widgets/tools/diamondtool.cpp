// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "diamondtool.hpp"

#include "item/factory/diamondfactory.hpp"
using namespace Qt::Literals::StringLiterals;

DiamondTool::DiamondTool()
{
    m_itemFactory = std::make_unique<DiamondFactory>();
    m_properties += Property::Type::BackgroundColor;
}

QString DiamondTool::tooltip() const
{
    return QObject::tr("Diamond Tool");
}

QString DiamondTool::icon() const
{
    return QString();
}
