// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "rectangletool.hpp"

#include "../item/factory/rectanglefactory.hpp"

RectangleTool::RectangleTool()
{
    m_itemFactory = std::make_unique<RectangleFactory>();
}

QString RectangleTool::tooltip() const
{
    return QObject::tr("Rectangle Tool");
}

IconManager::Icon RectangleTool::icon() const
{
    return IconManager::TOOL_RECTANGLE;
}
