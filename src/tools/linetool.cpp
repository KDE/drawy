// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "linetool.hpp"

#include "../item/factory/linefactory.hpp"

LineTool::LineTool() {
    m_itemFactory = std::make_unique<LineFactory>();
}

QString LineTool::tooltip() const {
    return "Line Tool";
}

IconManager::Icon LineTool::icon() const {
    return IconManager::TOOL_LINE;
}
