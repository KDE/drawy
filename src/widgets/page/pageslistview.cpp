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
#include <QStyleOptionToolButton>
#include <QStyledItemDelegate>

using namespace Qt::Literals::StringLiterals;

namespace
{
class PageItemDelegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QSize size{QStyledItemDelegate::sizeHint(option, index)};
        if (option.widget) {
            const int iconWidth{option.widget->style()->pixelMetric(QStyle::PM_ToolBarIconSize, nullptr, option.widget)};
            QStyleOptionToolButton opt;
            opt.initFrom(option.widget);
            opt.iconSize = QSize{iconWidth, iconWidth};
            const int targetHeight{option.widget->style()->sizeFromContents(QStyle::CT_ToolButton, &opt, opt.iconSize, option.widget).height()};
            size.setHeight(qMax(size.height(), targetHeight));
        }
        return size;
    }
};
}

PagesListView::PagesListView(ActionManager *actionManager, QWidget *parent)
    : QListView(parent)
    , mActionManager(actionManager)
{
    setFrameShape(QFrame::NoFrame);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragDropMode(QAbstractItemView::InternalMove);
    setDefaultDropAction(Qt::MoveAction);
    setDropIndicatorShown(true);
    setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setUniformItemSizes(true);
    setItemDelegate(new PageItemDelegate(this));

    QPalette pal{palette()};
    pal.setColor(QPalette::Base, Qt::transparent);
    setPalette(pal);
    viewport()->setAutoFillBackground(false);

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

QSize PagesListView::sizeHint() const
{
    if (!model()) {
        return QListView::sizeHint();
    }
    const int count{model()->rowCount()};
    if (count == 0) {
        return {0, 0};
    }
    int rowHeight{sizeHintForRow(0)};
    if (rowHeight <= 0) {
        const int iconWidth{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
        QStyleOptionToolButton opt;
        opt.initFrom(this);
        opt.iconSize = QSize{iconWidth, iconWidth};
        rowHeight = style()->sizeFromContents(QStyle::CT_ToolButton, &opt, opt.iconSize, this).height();
    }
    const int totalItemHeight{rowHeight * count + spacing() * qMax(0, count - 1)};
    const int frameMargin{frameWidth() * 2};
    return {QListView::sizeHint().width(), totalItemHeight + frameMargin};
}

QSize PagesListView::minimumSizeHint() const
{
    return {0, 0};
}

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
