// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "itemcommand.hpp"
class ApplicationContext;
class GroupItem;

class UngroupCommand : public ItemCommand
{
public:
    explicit UngroupCommand(QList<std::shared_ptr<Item>> items);

    void redo(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;
    [[nodiscard]] QString text() const override;

private:
    QList<std::shared_ptr<GroupItem>> m_groups;
    QList<QList<std::shared_ptr<Item>>> m_ungroupedItems;
};
