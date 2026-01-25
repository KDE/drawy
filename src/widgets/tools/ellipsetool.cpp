// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ellipsetool.hpp"

#include "item/factory/ellipsefactory.hpp"
using namespace Qt::Literals::StringLiterals;

EllipseTool::EllipseTool()
{
    m_itemFactory = std::make_unique<EllipseFactory>();
    m_properties += Property::Type::BackgroundColor;
}

QString EllipseTool::tooltip() const
{
    return QObject::tr("Ellipse Tool");
}

QString EllipseTool::icon() const
{
    return u"tool_ellipse"_s;
}
