/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QDialog>
class QPlainTextEdit;
class DebugDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DebugDialog(QWidget *parent = nullptr);
    ~DebugDialog() override;

    void setDebugInfo(const QString &str);

private:
    void readConfig();
    void writeConfig();
    QPlainTextEdit *const mPlainTextEdit;
};
