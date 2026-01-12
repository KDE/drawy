/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */
#pragma once

#include <QObject>

class ItemUtilsTest : public QObject
{
    Q_OBJECT
public:
    explicit ItemUtilsTest(QObject *parent = nullptr);
    ~ItemUtilsTest() override = default;

private Q_SLOTS:
    void shouldConvertEnumItemTypeToString();
    void shouldConvertItemTypeStringToEnum();

    void shouldConvertEnumStrokeTypeToString();
    void shouldConvertStrokeTypeStringToEnum();
};
