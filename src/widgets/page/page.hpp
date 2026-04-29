/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "libdrawywidgets_private_export.h"

class LIBDRAWYWIDGETS_TESTS_EXPORT Page
{
public:
    Page();
    ~Page();

    [[nodiscard]] QString name() const;
    void setName(const QString &newName);

private:
    QString mName;
};
