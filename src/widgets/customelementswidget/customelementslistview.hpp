/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawywidgets_private_export.h"
#include <QListView>
class LIBDRAWYWIDGETS_TESTS_EXPORT CustomElementsListView : public QListView
{
    Q_OBJECT
public:
    explicit CustomElementsListView(QWidget *parent = nullptr);
    ~CustomElementsListView() override;
};
