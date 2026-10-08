/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "libdrawywidgets_private_export.h"
#include <QAbstractListModel>
class PageManager;
class LIBDRAWYWIDGETS_TESTS_EXPORT PageModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum PageRoles {
        IsCurrentPageRole = Qt::UserRole + 1,
    };
    Q_ENUM(PageRoles)

    explicit PageModel(PageManager *pageManager, QObject *parent = nullptr);
    ~PageModel() override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = {}) const override;
    [[nodiscard]] QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;
    [[nodiscard]] Qt::DropActions supportedDropActions() const override;
    [[nodiscard]] bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;

private:
    LIBDRAWYWIDGETS_NO_EXPORT void slotResetModel();
    LIBDRAWYWIDGETS_NO_EXPORT void slotCurrentPageChanged();
    PageManager *const mPageManager;
    bool mMovingRows = false;
};
