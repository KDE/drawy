/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "customelementslistviewdelegate.hpp"
#include "customelements/customelementsmodel.hpp"

CustomElementsListViewDelegate::CustomElementsListViewDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

CustomElementsListViewDelegate::~CustomElementsListViewDelegate() = default;

void CustomElementsListViewDelegate::paint([[maybe_unused]] QPainter *painter,
                                           [[maybe_unused]] const QStyleOptionViewItem &option,
                                           [[maybe_unused]] const QModelIndex &index) const
{
    // TODO
}

QSize CustomElementsListViewDelegate::sizeHint([[maybe_unused]] const QStyleOptionViewItem &option, [[maybe_unused]] const QModelIndex &index) const
{
    return {};
}
