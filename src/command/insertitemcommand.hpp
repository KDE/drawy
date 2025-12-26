// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "itemcommand.hpp"
class ApplicationContext;

class InsertItemCommand : public ItemCommand {
public:
    InsertItemCommand(QVector<std::shared_ptr<Item>> items);

    void execute(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;
};
