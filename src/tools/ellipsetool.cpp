// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ellipsetool.hpp"

#include "../item/factory/ellipsefactory.hpp"

EllipseTool::EllipseTool() {
    m_itemFactory = std::make_unique<EllipseFactory>();
}

QString EllipseTool::tooltip() const {
    return "Ellipse Tool";
}

IconManager::Icon EllipseTool::icon() const {
    return IconManager::TOOL_ELLIPSE;
}
