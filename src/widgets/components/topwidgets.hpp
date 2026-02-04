// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QHBoxLayout>
#include <QWidget>

class TopWidgets : public QWidget
{
    Q_OBJECT
public:
    explicit TopWidgets(QWidget *parent = nullptr);

    void initialize();
    [[nodiscard]] bool isInitialized() const;

private:
    QHBoxLayout *const m_layout;

    bool m_isInitialized{false};
};
