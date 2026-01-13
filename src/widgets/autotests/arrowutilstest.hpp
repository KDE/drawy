/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include <QObject>

class ArrowUtilsTest : public QObject
{
    Q_OBJECT
public:
    explicit ArrowUtilsTest(QObject *parent = nullptr);
    ~ArrowUtilsTest() override = default;

private Q_SLOTS:
    void shouldConvertArrowTypeStringToArrowEnum();
    void shouldConvertArrowTypeEnumToString();
};
