/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawywidgets_private_export.h"
#include <QListView>
class ActionManager;
class LIBDRAWYWIDGETS_TESTS_EXPORT PagesListView : public QListView
{
    Q_OBJECT
public:
    explicit PagesListView(ActionManager *actionManager, QWidget *parent = nullptr);
    ~PagesListView() override;

    void setModel(QAbstractItemModel *model) override;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    LIBDRAWYWIDGETS_NO_EXPORT void slotSelectCurrentPage();
    LIBDRAWYWIDGETS_NO_EXPORT void slotCurrentRowChanged(const QModelIndex &current);
    LIBDRAWYWIDGETS_NO_EXPORT void movePage(int row, int destinationRow);
    ActionManager *const mActionManager;
};
