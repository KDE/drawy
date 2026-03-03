/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include "libdrawywidgets_private_export.h"
#include "tools/drawingtool.hpp"
class LIBDRAWYWIDGETS_TESTS_EXPORT ImageTool : public DrawingTool
{
public:
    explicit ImageTool(ApplicationContext *context);
    ~ImageTool() override = default;

    [[nodiscard]] Tool::Type type() const override;
    [[nodiscard]] QString icon() const override;

    void mouseReleased(ApplicationContext *context) override;

private:
    QString m_filter;
};
