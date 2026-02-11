/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "lockcommand.hpp"
#include "item/item.hpp"
#include <KLocalizedString>

LockCommand::LockCommand(QList<std::shared_ptr<Item>> items, bool lock)
    : ItemCommand{std::move(items)}
    , m_locked(lock)
{
}

void LockCommand::execute([[maybe_unused]] ApplicationContext *context)
{
    for (const auto &item : std::as_const(m_items)) {
        m_lockedStates[item] = item->locked();
        item->setLocked(m_locked);
    }
}

void LockCommand::undo([[maybe_unused]] ApplicationContext *context)
{
    for (const auto &item : std::as_const(m_items)) {
        item->setLocked(m_lockedStates[item]);
    }
}

QString LockCommand::commandTitle() const
{
    return i18n("Change Lock");
}
