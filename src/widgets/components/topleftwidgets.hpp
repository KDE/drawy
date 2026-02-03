// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QHBoxLayout>
#include <QWidget>

class TopLeftWidgets : public QWidget
{
public:
    TopLeftWidgets(QWidget *parent = nullptr);

private:
    QHBoxLayout *m_layout{nullptr};
};
