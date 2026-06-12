/*
 * SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include "dialog/configgroupinterface.hpp"
#include "libdrawywidgets_private_export.h"
#include <QWidget>

class QSlider;
class LIBDRAWYWIDGETS_TESTS_EXPORT FreehandSettingsWidget : public ConfigGroupInterface
{
    Q_OBJECT
public:
    explicit FreehandSettingsWidget(QWidget *parent = nullptr);
    ~FreehandSettingsWidget() override;

    void save() override;
    void load() override;

private:
    QSlider *m_streamlineSlider;
    QSlider *m_thinningSlider;
    QSlider *m_smoothingSlider;
};
