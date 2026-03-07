/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "item/itemutils.hpp"
#include "itemcommand.hpp"
#include <QList>
class ApplicationContext;
class ZorderCommand : public ItemCommand
{
public:
    explicit ZorderCommand(QList<std::shared_ptr<Item>> items, ItemUtils::ZorderMove move);

    void redo(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;

    [[nodiscard]] QString text() const override;

private:
    std::unordered_map<std::shared_ptr<Item>, int> m_orderIndex;
    ItemUtils::ZorderMove m_zordermove = ItemUtils::ZorderMove::BringForward;
};
