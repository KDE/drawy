// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QHBoxLayout>
#include <QWidget>

class ActionBar : public QWidget
{
    Q_OBJECT
public:
    explicit ActionBar(QWidget *parent = nullptr);

private:
    QHBoxLayout *const m_layout;
};
