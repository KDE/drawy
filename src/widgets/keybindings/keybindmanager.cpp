// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "keybindmanager.hpp"
#include <kactioncollection.h>

KeybindManager::KeybindManager(QObject *parent)
    : QObject{parent}
    , mActionCollection(new KActionCollection(this))
{
}

void KeybindManager::setEnabled(bool enabled)
{
    QList<QAction *> acts = mActionCollection->actions();
    for (const auto &act : acts) {
        act->setEnabled(enabled);
    }
}

KActionCollection *KeybindManager::actionCollection() const
{
    return mActionCollection;
}
