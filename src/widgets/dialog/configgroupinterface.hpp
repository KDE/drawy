// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <QGroupBox>
#include <QVBoxLayout>

class ConfigGroupInterface : public QGroupBox
{
    Q_OBJECT
public:
    virtual void save() = 0;
    virtual void load() = 0;

    explicit ConfigGroupInterface(QWidget *parent, const QString &groupTitle);

protected:
    QVBoxLayout *const m_layout;
};
