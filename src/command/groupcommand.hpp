// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GROUPCOMMAND_H
#define GROUPCOMMAND_H

#include "itemcommand.hpp"
class ApplicationContext;
class GroupItem;

class GroupCommand : public ItemCommand {
public:
    explicit GroupCommand(QVector<std::shared_ptr<Item>> items);

    void execute(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;

private:
    std::shared_ptr<GroupItem> m_group;
};

#endif  // GROUPCOMMAND_H
