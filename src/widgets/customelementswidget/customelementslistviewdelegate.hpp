/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QStyledItemDelegate>

class CustomElementsListViewDelegate : public QStyledItemDelegate
{
public:
    explicit CustomElementsListViewDelegate(QObject *parent = nullptr);
    ~CustomElementsListViewDelegate() override;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};
