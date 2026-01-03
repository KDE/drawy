// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "polygondrawingtool.hpp"

class EllipseTool : public PolygonDrawingTool
{
public:
    EllipseTool();

    ~EllipseTool() override = default;

    QString tooltip() const override;
    IconManager::Icon icon() const override;
};
