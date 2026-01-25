// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "rectangletool.hpp"

#include "item/factory/rectanglefactory.hpp"
using namespace Qt::Literals::StringLiterals;

RectangleTool::RectangleTool()
{
    m_itemFactory = std::make_unique<RectangleFactory>();
    m_properties += Property::Type::BackgroundColor;
}

QString RectangleTool::tooltip() const
{
    return QObject::tr("Rectangle Tool");
}

QString RectangleTool::icon() const
{
    return u"tool_rectangle"_s;
}
