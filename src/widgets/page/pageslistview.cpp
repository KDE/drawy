/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "pageslistview.hpp"
#include "keybindings/actionmanager.hpp"
#include "pagemodel.hpp"
#include <KLocalizedString>
#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>

using namespace Qt::Literals::StringLiterals;
PagesListView::PagesListView(ActionManager *actionManager, QWidget *parent)
    : QListView(parent)
    , mActionManager(actionManager)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(QAbstractItemView::InternalMove);
    setDefaultDropAction(Qt::MoveAction);
    setDropIndicatorShown(true);
    setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setUniformItemSizes(true);

    auto deleteAction = new QAction(this);
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setShortcutContext(Qt::WidgetShortcut);
    connect(deleteAction, &QAction::triggered, this, [this]() {
        if (currentIndex().isValid() && state() != QAbstractItemView::EditingState) {
            mActionManager->deletePage(currentIndex().row());
        }
    });
    addAction(deleteAction);
}

PagesListView::~PagesListView() = default;

void PagesListView::setModel(QAbstractItemModel *model)
{
    QListView::setModel(model);
    connect(model, &QAbstractItemModel::modelReset, this, &PagesListView::slotSelectCurrentPage);
    connect(model, &QAbstractItemModel::dataChanged, this, &PagesListView::slotSelectCurrentPage);
    connect(selectionModel(), &QItemSelectionModel::currentRowChanged, this, &PagesListView::slotCurrentRowChanged);
    slotSelectCurrentPage();
}

void PagesListView::slotSelectCurrentPage()
{
    for (int row = 0, count = model()->rowCount(); row < count; ++row) {
        const QModelIndex idx = model()->index(row, 0);
        if (idx.data(PageModel::IsCurrentPageRole).toBool()) {
            if (currentIndex() != idx) {
                setCurrentIndex(idx);
            }
            return;
        }
    }
}

void PagesListView::slotCurrentRowChanged(const QModelIndex &current)
{
    if (current.isValid()) {
        mActionManager->activatePage(current.row());
    }
}

void PagesListView::movePage(int row, int destinationRow)
{
    // destinationRow uses QAbstractItemModel::moveRows semantic
    model()->moveRow({}, row, {}, destinationRow);
}

void PagesListView::contextMenuEvent(QContextMenuEvent *event)
{
    const QModelIndex index = indexAt(event->pos());
    QMenu menu(this);
    menu.addAction(mActionManager->action(ActionManager::Action::NewPage));
    if (index.isValid()) {
        const int row{index.row()};
        const int count{model()->rowCount()};
        menu.addSeparator();
        menu.addAction(QIcon::fromTheme(u"edit-rename"_s), i18nc("@action", "Rename…"), this, [this, index]() {
            edit(index);
        });
        auto moveUpAction = menu.addAction(QIcon::fromTheme(u"go-up"_s), i18nc("@action", "Move Up"), this, [this, row]() {
            movePage(row, row - 1);
        });
        moveUpAction->setEnabled(row > 0);
        auto moveDownAction = menu.addAction(QIcon::fromTheme(u"go-down"_s), i18nc("@action", "Move Down"), this, [this, row]() {
            movePage(row, row + 2);
        });
        moveDownAction->setEnabled(row < count - 1);
        if (count > 1) {
            menu.addSeparator();
            menu.addAction(QIcon::fromTheme(u"list-remove"_s), i18nc("@action", "Delete Page"), this, [this, row]() {
                mActionManager->deletePage(row);
            });
        }
    }
    menu.exec(event->globalPos());
}

#include "moc_pageslistview.cpp"
