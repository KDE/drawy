// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
// SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
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

    void loadFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    LIBDRAWYWIDGETS_NO_EXPORT void applyCustomStyles();
    bool m_config_useSystemStyles{true};
    bool m_forceClose = false;
};
