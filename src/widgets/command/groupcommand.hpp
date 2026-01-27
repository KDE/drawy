// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "itemcommand.hpp"
class ApplicationContext;
class GroupItem;

class GroupCommand : public ItemCommand
{
public:
    explicit GroupCommand(QList<std::shared_ptr<Item>> items);

    void execute(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;
    [[nodiscard]] QString commandTitle() const override;

private:
    const std::shared_ptr<GroupItem> m_group;
};
