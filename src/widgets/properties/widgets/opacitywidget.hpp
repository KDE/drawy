/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "libdrawywidgets_private_export.h"
#include "propertywidget.hpp"
class QSlider;
class LIBDRAWYWIDGETS_TESTS_EXPORT OpacityWidget : public PropertyWidget
{
    Q_OBJECT
public:
    explicit OpacityWidget(QWidget *parent = nullptr);

    [[nodiscard]] QString name() const override;
    [[nodiscard]] Property value() const override;

    void setValue(const QVariant &value) override;

private:
    QSlider *const mSlider;
};
