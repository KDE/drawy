// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "itemcommand.hpp"

#include <QDebug>
#include <utility>

ItemCommand::ItemCommand(QVector<std::shared_ptr<Item>> items)
    : m_items{std::move(items)}
{
}

ItemCommand::~ItemCommand()
{
    qDebug() << "Object deleted: ItemCommand";
}
