// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawywidgets_private_export.h"
#include "polygondrawingtool.hpp"

class LIBDRAWYWIDGETS_TESTS_EXPORT EllipseTool : public PolygonDrawingTool
{
public:
    EllipseTool();

    ~EllipseTool() override = default;

    [[nodiscard]] QString tooltip() const override;
    [[nodiscard]] QString icon() const override;
};
