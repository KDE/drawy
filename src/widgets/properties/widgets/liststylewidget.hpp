/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libdrawywidgets_private_export.h"
#include "propertywidget.hpp"

class QComboBox;

class LIBDRAWYWIDGETS_TESTS_EXPORT ListStyleWidget : public PropertyWidget
{
    Q_OBJECT
public:
    explicit ListStyleWidget(QWidget *parent = nullptr);

    [[nodiscard]] QString name() const override;
    [[nodiscard]] Property value() const override;

    void setValue(const QVariant &val) override;

private:
    QComboBox *const m_comboBox;
};
