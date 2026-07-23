/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <QObject>

class CustomElementsListViewTest : public QObject
{
    Q_OBJECT
public:
    explicit CustomElementsListViewTest(QObject *parent = nullptr);
    ~CustomElementsListViewTest() override = default;

private Q_SLOTS:
    void shouldHaveDefaultValues();
};
