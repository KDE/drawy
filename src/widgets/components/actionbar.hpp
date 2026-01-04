// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QFrame>
#include <QHBoxLayout>
#include <QHash>
#include <QPushButton>

#include "iconmanager/iconmanager.hpp"

class ActionBar : public QFrame
{
    Q_OBJECT
public:
    explicit ActionBar(QWidget *parent = nullptr);

    void addButton(const QString &tooltip, IconManager::Icon icon);
    [[nodiscard]] QPushButton *button(IconManager::Icon id);

private:
    QHash<IconManager::Icon, QPushButton *> m_map{};
    QHBoxLayout *const m_layout;
};
