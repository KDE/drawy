// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "itemcommand.hpp"
#include "drawy_debug.h"

#include <QDebug>
#include <utility>

ItemCommand::ItemCommand(QList<std::shared_ptr<Item>> items)
    : m_items{std::move(items)}
{
}

ItemCommand::~ItemCommand()
{
    qCDebug(DRAWY_LOG) << "Object deleted: ItemCommand";
}
