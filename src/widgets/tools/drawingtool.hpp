// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "item/factory/itemfactory.hpp"
#include "libdrawywidgets_private_export.h"
#include "tool.hpp"

class LIBDRAWYWIDGETS_TESTS_EXPORT DrawingTool : public Tool
{
public:
    explicit DrawingTool(ApplicationContext *context);
    ~DrawingTool() override = default;

protected:
    std::unique_ptr<ItemFactory> m_itemFactory;
    bool m_isDrawing{false};
};
