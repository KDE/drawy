/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once
#include "command/itemcommand.hpp"
#include "item/arrowutils.hpp"

class ArrowTypeCommand : public ItemCommand
{
public:
    explicit ArrowTypeCommand(QList<std::shared_ptr<Item>> items, ArrowUtils::ArrowPos arrowPos, ArrowUtils::ArrowType arrowStyle);

    void redo(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;

    [[nodiscard]] QString text() const override;

private:
    std::unordered_map<std::shared_ptr<Item>, ArrowUtils::ArrowType> m_originalArrowStyle;
    ArrowUtils::ArrowPos m_arrowPos = ArrowUtils::ArrowPos::EndArrow;
    ArrowUtils::ArrowType m_arrowStyle = ArrowUtils::ArrowType::None;
};
