// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SELECTCOMMAND_H
#define SELECTCOMMAND_H

#include "itemcommand.hpp"
class ApplicationContext;

class SelectCommand : public ItemCommand
{
public:
    explicit SelectCommand(QList<std::shared_ptr<Item>> items);

    void execute(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;
};

#endif // SELECTCOMMAND_H
