// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawywidgets_export.h"
#include <QWidget>

class LIBDRAWYWIDGETS_EXPORT MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void viewFullScreen(bool fullScreen);

private:
    void applyCustomStyles();
    bool m_config_useSystemStyles{true};
};
