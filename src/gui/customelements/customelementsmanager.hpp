/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include "customelements/customelement.hpp"
#include "libdrawygui_export.h"
#include <QList>
#include <QObject>
#include <memory>

class Item;
class LIBDRAWYGUI_EXPORT CustomElementsManager : public QObject
{
    Q_OBJECT
public:
    ~CustomElementsManager() override;
    static CustomElementsManager *self();

    void loadCustomElements();
    void saveCustomElements();

    [[nodiscard]] bool isEmpty() const;

    void exportToFile(const QString &fileName);
    void addItem(const std::shared_ptr<Item> &item);

private:
    explicit CustomElementsManager(QObject *parent = nullptr);
    QList<CustomElement> mCustomElements;
};
