// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ellipsetool.hpp"
#include <KLocalizedString>

#include "item/factory/ellipsefactory.hpp"
using namespace Qt::Literals::StringLiterals;

EllipseTool::EllipseTool(ApplicationContext *context)
    : PolygonDrawingTool(context)
{
    m_itemFactory = std::make_unique<EllipseFactory>();
    m_properties += Property::Type::BackgroundColor;
    m_properties += Property::Type::BackgroundStyle;
}

QString EllipseTool::icon() const
{
    return u"tool_ellipse"_s;
}

Tool::Type EllipseTool::type() const
{
    return Tool::Type::Ellipse;
}
