// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "rectangletool.hpp"
#include <KLocalizedString>

#include "item/factory/rectanglefactory.hpp"
using namespace Qt::Literals::StringLiterals;

RectangleTool::RectangleTool(ApplicationContext *context)
    : PolygonDrawingTool(context)
{
    m_itemFactory = std::make_unique<RectangleFactory>();
    m_properties += Property::Type::BackgroundColor;
    m_properties += Property::Type::BackgroundStyle;
}

QString RectangleTool::tooltip() const
{
    return i18nc("@info:tooltip", "Rectangle Tool");
}

QString RectangleTool::icon() const
{
    return u"tool_rectangle"_s;
}

Tool::Type RectangleTool::type() const
{
    return Tool::Type::Rectangle;
}
