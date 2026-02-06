// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QWidget>
class ApplicationContext;
class QHBoxLayout;
class TopWidgets : public QWidget
{
    Q_OBJECT
public:
    explicit TopWidgets(ApplicationContext *context, QWidget *parent = nullptr);

    void initialize();
    [[nodiscard]] bool isInitialized() const;

private:
    QHBoxLayout *const m_layout;
    ApplicationContext *const m_context;

    bool m_isInitialized{false};
};
