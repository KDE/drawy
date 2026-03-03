// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "libdrawywidgets_private_export.h"
#include "polygondrawingtool.hpp"
class LIBDRAWYWIDGETS_TESTS_EXPORT RectangleTool : public PolygonDrawingTool
{
public:
    explicit RectangleTool(ApplicationContext *context);
    ~RectangleTool() override = default;

    [[nodiscard]] Tool::Type type() const override;
    [[nodiscard]] QString icon() const override;
};
