/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once
#include "libdrawygui_export.h"
#include <QList>
#include <QObject>
#include <memory>

class Item;
class CustomElementsModel;
class LIBDRAWYGUI_EXPORT CustomElementsManager : public QObject
{
    Q_OBJECT
public:
    ~CustomElementsManager() override;
    static CustomElementsManager *self();

    void loadCustomElements();
    void saveCustomElements(const QString &fileName = {});

    [[nodiscard]] bool isEmpty() const;

    void exportToFile(const QString &fileName);
    void addItem(const std::shared_ptr<Item> &item);

    [[nodiscard]] CustomElementsModel *customElementsModel() const;

private:
    explicit CustomElementsManager(QObject *parent = nullptr);
    CustomElementsModel *const mCustomElementsModel;
};
