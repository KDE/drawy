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
    MoveItemCommand(QList<std::shared_ptr<Item>> items, const QPointF worldInitialPos, const QPointF worldFinalPos);

    void redo(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;
    [[nodiscard]] QString text() const override;

private:
    QPointF m_worldInitialPos;
    QPointF m_worldFinalPos;
};
