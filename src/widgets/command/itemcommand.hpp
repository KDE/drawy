// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "commandbase.hpp"
#include "libdrawywidgets_private_export.h"
#include <QList>
#include <memory>
class Item;
class LIBDRAWYWIDGETS_TESTS_EXPORT ItemCommand : public CommandBase
{
public:
    explicit ItemCommand(QList<std::shared_ptr<Item>> items);
    ~ItemCommand() override;

protected:
    QList<std::shared_ptr<Item>> m_items;
};
