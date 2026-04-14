/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "item/itemutils.hpp"
#include "itemcommand.hpp"
#include <QList>
#include <QPointF>
class ApplicationContext;
class AlignItemCommand : public ItemCommand
{
public:
    explicit AlignItemCommand(QList<std::shared_ptr<Item>> items, ItemUtils::AlignType alignment);

    void redo(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;

    [[nodiscard]] QString text() const override;
    [[nodiscard]] bool hasChanged() const;

private:
    void calculateMoveItems();
    ItemUtils::AlignType m_alignment = ItemUtils::AlignType::Unknown;

    QList<QPointF> m_initialPositions;
    QList<QPointF> m_finalPositions;
};
