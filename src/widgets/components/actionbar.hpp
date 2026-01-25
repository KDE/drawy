// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QFrame>
#include <QHBoxLayout>
#include <QHash>
#include <QPushButton>

class ActionBar : public QFrame
{
    Q_OBJECT
public:
    explicit ActionBar(QWidget *parent = nullptr);

    [[nodiscard]] QPushButton *addButton(const QString &tooltip, const QString &icon);

private:
    QHBoxLayout *const m_layout;
};
