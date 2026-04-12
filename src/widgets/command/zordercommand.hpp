/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "data-structures/orderedlist.hpp"
#include "item/item.hpp"
#include "item/itemutils.hpp"
#include "itemcommand.hpp"
#include <QList>
#include <unordered_map>
class ApplicationContext;
class ZorderCommand : public ItemCommand
{
public:
    explicit ZorderCommand(QList<std::shared_ptr<Item>> items, ItemUtils::ZorderMove move);

    void redo(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;

    [[nodiscard]] QString text() const override;

private:
    using OriginalItemOrder = ItemOrderPosition<std::weak_ptr<Item>>;

    std::unordered_map<std::shared_ptr<Item>, OriginalItemOrder> m_originalOrder;
    QList<std::shared_ptr<Item>> m_originalSortedItems;
    ItemUtils::ZorderMove m_zorderMove = ItemUtils::ZorderMove::BringForward;
};
