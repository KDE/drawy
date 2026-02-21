// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QPointF>

#include "itemcommand.hpp"
#include <QObject>

class ApplicationContext;

class LineResizeItemCommand : public ItemCommand
{
    Q_GADGET
public:
    enum class HandleType {
        Start,
        End
    };

    Q_ENUM(HandleType)

    LineResizeItemCommand(QList<std::shared_ptr<Item>> items, const QPointF initialPos, const QPointF finalPos, const HandleType handleType);

    void execute(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;
    [[nodiscard]] QString commandTitle() const override;

private:
    QPointF m_initialPos{};
    QPointF m_finalPos{};

    HandleType m_handleType{};
};
