/*
 * SPDX-FileCopyrightText: 2026 Abdelhadi Wael <waelhadi380@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once
#include "libdrawywidgets_private_export.h"
#include "propertywidget.hpp"

class QToolButton;

class LIBDRAWYWIDGETS_TESTS_EXPORT TextAlignmentWidget : public PropertyWidget
{
    Q_OBJECT
public:
    explicit TextAlignmentWidget(QWidget *parent = nullptr);

    [[nodiscard]] QString name() const override;
    [[nodiscard]] Property value() const override;

    void setValue(const QVariant &value) override;

private:
    QToolButton *m_alignLeft = nullptr;
    QToolButton *m_alignRight = nullptr;
    QToolButton *m_alignCenter = nullptr;
    QToolButton *m_alignJustify = nullptr;
};
