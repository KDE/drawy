/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include "libdrawywidgets_private_export.h"
#include "propertywidget.hpp"

class QToolButton;
class LIBDRAWYWIDGETS_TESTS_EXPORT FontStyleWidget : public PropertyWidget
{
    Q_OBJECT
public:
    explicit FontStyleWidget(QWidget *parent = nullptr);

    [[nodiscard]] QString name() const override;
    [[nodiscard]] Property value() const override;

    void setValue(const QVariant &val) override;

private:
    QToolButton *m_bold = nullptr;
    QToolButton *m_italic = nullptr;
    QToolButton *m_underlined = nullptr;
    QToolButton *m_strikethrough = nullptr;
};
