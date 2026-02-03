/*
 * SPDX-FileCopyrightText: 2026 Nikolay Kochulin <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QObject>

class SvgExportTest : public QObject
{
    Q_OBJECT
public:
    explicit SvgExportTest(QObject *parent = nullptr);
    ~SvgExportTest() override = default;

private Q_SLOTS:
    void shouldNotChangeOutput();
    void shouldNotChangeOutput_data();
    void initTestCase();
};
