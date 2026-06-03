/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawywidgets_private_export.h"
#include "propertywidget.hpp"
class QButtonGroup;
class LIBDRAWYWIDGETS_TESTS_EXPORT StokeStyleWidget : public PropertyWidget
{
    Q_OBJECT
public:
    explicit StokeStyleWidget(QWidget *parent = nullptr);
    ~StokeStyleWidget() override;

    [[nodiscard]] QString name() const override;
    [[nodiscard]] Property value() const override;

    void setValue(const QVariant &val) override;

private:
    QButtonGroup *m_group = nullptr;
};
