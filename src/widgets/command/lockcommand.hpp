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
class LockCommand : public ItemCommand
{
public:
    explicit LockCommand(QList<std::shared_ptr<Item>> items, bool lock);

    void execute(ApplicationContext *context) override;
    void undo(ApplicationContext *context) override;

    [[nodiscard]] QString commandTitle() const override;

private:
    std::unordered_map<std::shared_ptr<Item>, bool> m_lockedStates;
    bool m_locked = false;
};
