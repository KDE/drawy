// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "dialog/configgroupinterface.hpp"
#include "libdrawywidgets_private_export.h"
class QSpinBox;
class QWidget;
class QLineEdit;
class QCheckBox;

class LIBDRAWYWIDGETS_TESTS_EXPORT AutoSaveWidget : public ConfigGroupInterface
{
    Q_OBJECT
public:
    explicit AutoSaveWidget(QWidget *parent);

    void save() override;
    void load() override;
    void slotEnableChanged(Qt::CheckState state);

private:
    QSpinBox *const m_delay{};
    QWidget *const m_pathWidget{};
    QLineEdit *const m_path{};
    QCheckBox *const m_enabled{};
};
