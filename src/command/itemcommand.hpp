// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QVector>
#include <memory>

#include "command.hpp"
class Item;

class ItemCommand : public Command {
public:
    ItemCommand(QVector<std::shared_ptr<Item>> items);
    ~ItemCommand() override;

protected:
    QVector<std::shared_ptr<Item>> m_items;
};
