/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include "item/arrowutils.hpp"
#include "libdrawywidgets_private_export.h"
#include <QToolButton>

class LIBDRAWYWIDGETS_TESTS_EXPORT ArrowButtonActionsWidget : public QToolButton
{
    Q_OBJECT
public:
    explicit ArrowButtonActionsWidget(QWidget *parent = nullptr);
    ~ArrowButtonActionsWidget() override;

Q_SIGNALS:
    void selectedArrow(ArrowUtils::ArrowType type);

private:
    LIBDRAWYWIDGETS_NO_EXPORT void fillMenu();
};
