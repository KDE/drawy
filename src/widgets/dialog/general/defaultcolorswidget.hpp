// SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "dialog/configgroupinterface.hpp"
#include "libdrawywidgets_private_export.h"
class KColorButton;

class LIBDRAWYWIDGETS_TESTS_EXPORT DefaultColorsWidget : public ConfigGroupInterface
{
    Q_OBJECT
public:
    explicit DefaultColorsWidget(QWidget *parent = nullptr);

    void save() override;
    void load() override;

private:
    KColorButton *const m_lightForegroundColor;
    KColorButton *const m_darkForegroundColor;
    KColorButton *const m_lightFirstColor;
    KColorButton *const m_lightSecondColor;
    KColorButton *const m_lightThirdColor;
    KColorButton *const m_lightFourthColor;
    KColorButton *const m_darkFirstColor;
    KColorButton *const m_darkSecondColor;
    KColorButton *const m_darkThirdColor;
    KColorButton *const m_darkFourthColor;
};
