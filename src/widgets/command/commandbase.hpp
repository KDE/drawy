/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include "libdrawywidgets_private_export.h"
#include <QString>
class ApplicationContext;
class LIBDRAWYWIDGETS_TESTS_EXPORT CommandBase
{
public:
    CommandBase();
    virtual ~CommandBase();

    virtual void redo(ApplicationContext *context) = 0;
    virtual void undo(ApplicationContext *context) = 0;
    virtual QString text() const = 0;
};
