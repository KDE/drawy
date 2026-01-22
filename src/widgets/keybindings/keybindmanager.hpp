// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QKeySequence>
#include <QMap>
#include <QShortcut>

#include "action.hpp"

class KeybindManager : public QObject
{
public:
    explicit KeybindManager(QObject *parent);

    void addKeybinding(Action *action, const QKeySequence &sequence);
    void setEnabled(bool enabled);

private:
    QMap<QKeySequence, QShortcut *> m_keyToShortcut;
    QMap<QKeySequence, Action *> m_keyToAction;
};
