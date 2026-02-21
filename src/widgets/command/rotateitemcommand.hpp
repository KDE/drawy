// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QPointF>

#include "itemcommand.hpp"
class ApplicationContext;

class RotateItemCommand : public ItemCommand
{
public:
    RotateItemCommand(QList<std::shared_ptr<Item>> items, const qreal angle, const QPointF pivot);

    void execute(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;
    [[nodiscard]] QString commandTitle() const override;

private:
    qreal m_angle;
    QPointF m_pivot{};
};
