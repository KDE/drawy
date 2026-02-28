// SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "dialog/configgroupinterface.hpp"
#include "libdrawywidgets_private_export.h"
class KColorButton;

class LIBDRAWYWIDGETS_TESTS_EXPORT CanvasColorWidget : public ConfigGroupInterface
{
    Q_OBJECT
public:
    explicit CanvasColorWidget(QWidget *parent = nullptr);

    void save() override;
    void load() override;

private:
    KColorButton *const m_lightThemeColor;
    KColorButton *const m_darkThemeColor;
};
