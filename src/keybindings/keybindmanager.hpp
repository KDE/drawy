// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QKeySequence>
#include <QShortcut>
#include <unordered_map>

#include "action.hpp"

class KeybindManager : public QObject
{
public:
    explicit KeybindManager(QObject *parent);

    void addKeybinding(Action *action, const QString &sequence);
    void removeKeybinding(QKeySequence sequence);
    void disable();
    void enable();

private:
    std::unordered_map<QString, QShortcut *> m_keyToShortcut;
    std::unordered_map<QString, Action *> m_keyToAction;
};
