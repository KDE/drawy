// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "libdrawywidgets_private_export.h"
#include <QList>
#include <memory>
class Item;
class ApplicationContext;
class LIBDRAWYWIDGETS_TESTS_EXPORT ItemCommand
{
public:
    explicit ItemCommand(QList<std::shared_ptr<Item>> items);
    virtual ~ItemCommand();

    virtual void redo(ApplicationContext *context) = 0;
    virtual void undo(ApplicationContext *context) = 0;
    virtual QString text() const = 0;

protected:
    QList<std::shared_ptr<Item>> m_items;
};
