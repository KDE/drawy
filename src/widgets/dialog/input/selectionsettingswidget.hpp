/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include "dialog/configgroupinterface.hpp"

class QRadioButton;

class SelectionSettingsWidget : public ConfigGroupInterface
{
    Q_OBJECT
public:
    explicit SelectionSettingsWidget(QWidget *parent = nullptr);
    ~SelectionSettingsWidget() override = default;

    void save() override;
    void load() override;

private:
    QRadioButton *const m_fullyContained;
    QRadioButton *const m_overlap;
};
