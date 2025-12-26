// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "polygondrawingtool.hpp"
class PropertyManager;

class LineTool : public PolygonDrawingTool {
public:
    LineTool();
    ~LineTool() override = default;

    QString tooltip() const override;
    IconManager::Icon icon() const override;
};
