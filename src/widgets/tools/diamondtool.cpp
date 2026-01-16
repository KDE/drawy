// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "diamondtool.hpp"

#include "item/factory/diamondfactory.hpp"

DiamondTool::DiamondTool()
{
    m_itemFactory = std::make_unique<DiamondFactory>();
    m_properties += Property::Type::BackgroundColor;
}

QString DiamondTool::tooltip() const
{
    return QObject::tr("Diamond Tool");
}

IconManager::Icon DiamondTool::icon() const
{
    return IconManager::Icon::TOOL_DIAMOND;
}
