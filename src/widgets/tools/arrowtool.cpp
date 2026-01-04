// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "arrowtool.hpp"

#include "item/factory/arrowfactory.hpp"

ArrowTool::ArrowTool()
{
    m_itemFactory = std::make_unique<ArrowFactory>();
}

QString ArrowTool::tooltip() const
{
    return QObject::tr("Arrow Tool");
}

IconManager::Icon ArrowTool::icon() const
{
    return IconManager::Icon::TOOL_ARROW;
}
