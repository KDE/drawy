// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "diamondtool.hpp"
#include <KLocalizedString>

#include "item/factory/diamondfactory.hpp"
using namespace Qt::Literals::StringLiterals;

DiamondTool::DiamondTool(ApplicationContext *context)
    : PolygonDrawingTool(context)
{
    m_itemFactory = std::make_unique<DiamondFactory>();
    m_properties += Property::Type::BackgroundColor;
    m_properties += Property::Type::BackgroundStyle;
}

QString DiamondTool::icon() const
{
    return u"choice-rhomb"_s;
}

Tool::Type DiamondTool::type() const
{
    return Tool::Type::Diamond;
}
