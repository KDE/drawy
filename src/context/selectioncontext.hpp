// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QWidget>
#include <unordered_set>
class Property;
class Item;
class ApplicationContext;

class SelectionContext : public QObject
{
public:
    explicit SelectionContext(ApplicationContext *context);
    ~SelectionContext() override;

    std::unordered_set<std::shared_ptr<Item>> &selectedItems();
    QRectF selectionBox() const;

    void reset();

public Q_SLOTS:
    void updatePropertyOfSelectedItems(const Property &property);

private:
    std::unordered_set<std::shared_ptr<Item>> m_selectedItems{};

    ApplicationContext *m_applicationContext;
};
