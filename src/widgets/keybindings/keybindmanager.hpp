// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QObject>
class KActionCollection;
class KeybindManager : public QObject
{
public:
    explicit KeybindManager(QObject *parent);

    void setEnabled(bool enabled);

    KActionCollection *actionCollection() const;

private:
    KActionCollection *const mActionCollection;
};
