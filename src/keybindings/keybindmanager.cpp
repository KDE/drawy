// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "keybindmanager.hpp"

KeybindManager::KeybindManager(QObject *parent)
    : QObject{parent}
{
}

void KeybindManager::addKeybinding(Action *action, const QString &sequence)
{
    if (m_keyToAction.find(sequence) != m_keyToAction.end())
        return;

    if (m_keyToShortcut.find(sequence) == m_keyToShortcut.end()) {
        m_keyToShortcut[sequence] = new QShortcut{QKeySequence::fromString(sequence), parent()};
    }

    m_keyToAction[sequence] = action;

    QShortcut *shortcut{m_keyToShortcut[sequence]};
    QObject::connect(shortcut, &QShortcut::activated, action, &Action::run);
}

// TODO: Implement this
void removeKeybinding(QKeySequence sequence);

void KeybindManager::disable()
{
    for (auto &keyShortcutPair : m_keyToShortcut) {
        keyShortcutPair.second->setEnabled(false);
    }
}

void KeybindManager::enable()
{
    for (auto &keyShortcutPair : m_keyToShortcut) {
        keyShortcutPair.second->setEnabled(true);
    }
}
