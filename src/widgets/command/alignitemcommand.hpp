/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "itemcommand.hpp"
#include <QList>
#include <QPointF>
class ApplicationContext;
class AlignItemCommand : public ItemCommand
{
public:
    enum class Alignment : uint8_t {
        Unknow = 0,
        AlignToTop,
        AlignToBottom,
        AlignToRight,
        AlignToLeft,
        AlignHorizontalCenter,
        AlignVerticalCenter,
    };

    explicit AlignItemCommand(QList<std::shared_ptr<Item>> items, AlignItemCommand::Alignment alignment);

    void execute(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;

    [[nodiscard]] QString commandTitle() const override;

private:
    const AlignItemCommand::Alignment m_alignment = AlignItemCommand::Alignment::Unknow;
    QList<QPointF> m_moveToPoint;
};
