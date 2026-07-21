/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "libdrawywidgets_private_export.h"
#include <QWidget>
class QListView;
class CustomElementsManager;
class LIBDRAWYWIDGETS_TESTS_EXPORT CustomElementsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomElementsWidget(CustomElementsManager *manager, QWidget *parent = nullptr);
    ~CustomElementsWidget() override;

private:
    QListView *const mListView;
};
