/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "libdrawywidgets_private_export.h"
#include "propertywidget.hpp"
class QButtonGroup;
class LIBDRAWYWIDGETS_TESTS_EXPORT BackgroundStyleWidget : public PropertyWidget
{
    Q_OBJECT
public:
    explicit BackgroundStyleWidget(QWidget *parent = nullptr);
    [[nodiscard]] QString name() const override;
    [[nodiscard]] const Property value() const override;

private:
    QButtonGroup *m_group = nullptr;
};
