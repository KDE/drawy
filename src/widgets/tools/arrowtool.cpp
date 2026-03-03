// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "arrowtool.hpp"
#include <KLocalizedString>

#include "item/factory/arrowfactory.hpp"
using namespace Qt::Literals::StringLiterals;

ArrowTool::ArrowTool(ApplicationContext *context)
    : PolygonDrawingTool(context)
{
    m_itemFactory = std::make_unique<ArrowFactory>();
    m_properties += Property::Type::ArrowStyle;
}

QString ArrowTool::icon() const
{
    return u"draw-arrow"_s;
}

Tool::Type ArrowTool::type() const
{
    return Tool::Type::Arrow;
}
