// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QPointF>

#include "itemcommand.hpp"
class ApplicationContext;

class MoveItemCommand : public ItemCommand
{
public:
    MoveItemCommand(QVector<std::shared_ptr<Item>> items, QPointF delta);

    void execute(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;

private:
    QPointF m_delta;
};
