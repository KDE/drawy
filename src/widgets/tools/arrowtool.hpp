// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawywidgets_private_export.h"
#include "linedrawingtool.hpp"

class LIBDRAWYWIDGETS_TESTS_EXPORT ArrowTool : public LineDrawingTool
{
public:
    explicit ArrowTool(ApplicationContext *context);
    ~ArrowTool() override = default;

    [[nodiscard]] Tool::Type type() const override;
    [[nodiscard]] QString icon() const override;
};
