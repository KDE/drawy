// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QList>
#include <memory>

#include "command.hpp"
class Item;

class ItemCommand : public Command
{
public:
    explicit ItemCommand(QList<std::shared_ptr<Item>> items);
    ~ItemCommand() override;

protected:
    QList<std::shared_ptr<Item>> m_items;
};
