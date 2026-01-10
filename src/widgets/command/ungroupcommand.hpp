// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UNGROUPCOMMAND_H
#define UNGROUPCOMMAND_H

#include "itemcommand.hpp"
class ApplicationContext;
class GroupItem;

class UngroupCommand : public ItemCommand
{
public:
    explicit UngroupCommand(const QList<std::shared_ptr<Item>> &items);

    void execute(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;

private:
    QList<std::shared_ptr<GroupItem>> m_groups;
};

#endif // UNGROUPCOMMAND_H
