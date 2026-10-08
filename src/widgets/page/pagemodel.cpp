/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "pagemodel.hpp"
#include "page.hpp"
#include "pagemanager.hpp"
#include <KLocalizedString>
#include <QFont>

PageModel::PageModel(PageManager *pageManager, QObject *parent)
    : QAbstractListModel{parent}
    , mPageManager(pageManager)
{
    // PageManager signals are emitted after the change, so reset the model
    connect(mPageManager, &PageManager::pageInserted, this, &PageModel::slotResetModel);
    connect(mPageManager, &PageManager::pageRemoved, this, &PageModel::slotResetModel);
    connect(mPageManager, &PageManager::pagesReset, this, &PageModel::slotResetModel);
    connect(mPageManager, &PageManager::pageMoved, this, [this]() {
        if (!mMovingRows) {
            slotResetModel();
        }
    });
    connect(mPageManager, &PageManager::pageRenamed, this, [this](int row) {
        const QModelIndex idx = index(row);
        Q_EMIT dataChanged(idx, idx, {Qt::DisplayRole, Qt::EditRole});
    });
    connect(mPageManager, &PageManager::currentPageChanged, this, &PageModel::slotCurrentPageChanged);
}

PageModel::~PageModel() = default;

void PageModel::slotResetModel()
{
    beginResetModel();
    endResetModel();
}

void PageModel::slotCurrentPageChanged()
{
    if (mMovingRows) {
        return;
    }
    const int count{rowCount()};
    if (count > 0) {
        Q_EMIT dataChanged(index(0), index(count - 1), {Qt::FontRole, IsCurrentPageRole});
    }
}

int PageModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(mPageManager->pages().count());
}

QVariant PageModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid | CheckIndexOption::ParentIsInvalid)) {
        return {};
    }
    const Page *page{mPageManager->pages().at(index.row())};
    switch (role) {
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        if (page->name().isEmpty()) {
            return i18nc("@label default page name", "Page %1", index.row() + 1);
        }
        return page->name();
    case Qt::EditRole:
        return page->name();
    case Qt::FontRole:
        if (index.row() == mPageManager->currentPage()) {
            QFont font;
            font.setBold(true);
            return font;
        }
        return {};
    case IsCurrentPageRole:
        return index.row() == mPageManager->currentPage();
    }
    return {};
}

bool PageModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || !checkIndex(index, CheckIndexOption::IndexIsValid | CheckIndexOption::ParentIsInvalid)) {
        return false;
    }
    const QString name{value.toString().trimmed()};
    if (name.isEmpty() || name == mPageManager->pages().at(index.row())->name()) {
        return false;
    }
    mPageManager->renamePage(index.row(), name);
    return true;
}

Qt::ItemFlags PageModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        // Allow to drop between items
        return Qt::ItemIsDropEnabled;
    }
    return QAbstractListModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDragEnabled;
}

Qt::DropActions PageModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

bool PageModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    if (sourceParent.isValid() || destinationParent.isValid() || count != 1) {
        return false;
    }
    const int rows{rowCount()};
    if (sourceRow < 0 || sourceRow >= rows || destinationChild < 0 || destinationChild > rows) {
        return false;
    }
    // destinationChild is the row before which the page is inserted (Qt semantic)
    // PageManager::movePage expects the final index
    const int to{destinationChild > sourceRow ? destinationChild - 1 : destinationChild};
    if (to == sourceRow) {
        return false;
    }
    if (!beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, destinationChild)) {
        return false;
    }
    mMovingRows = true;
    mPageManager->movePage(sourceRow, to);
    mMovingRows = false;
    endMoveRows();
    slotCurrentPageChanged();
    return true;
}

#include "moc_pagemodel.cpp"
