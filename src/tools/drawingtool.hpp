// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "../item/factory/itemfactory.hpp"
#include "tool.hpp"

class DrawingTool : public Tool {
protected:
    std::unique_ptr<ItemFactory> m_itemFactory{};

public:
    DrawingTool();
    ~DrawingTool() override = default;

protected:
    bool m_isDrawing{false};
};
