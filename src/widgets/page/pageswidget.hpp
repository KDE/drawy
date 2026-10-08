/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawywidgets_private_export.h"
#include <QWidget>
class ActionManager;
class PagesListView;
class QAbstractItemModel;
class LIBDRAWYWIDGETS_TESTS_EXPORT PagesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PagesWidget(ActionManager *actionManager, QAbstractItemModel *model, QWidget *parent = nullptr);
    ~PagesWidget() override;

private:
    PagesListView *const mListView;
};
