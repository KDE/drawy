/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "item/item.hpp"
#include "libdrawywidgets_private_export.h"
#include <QJsonObject>
#include <QObject>
#include <QPointF>
class LIBDRAWYWIDGETS_TESTS_EXPORT DeserializeJob : public QObject
{
    Q_OBJECT
public:
    struct DeserializeInfo {
        QPointF offsetPos;
        qreal zoomOffset = 0.0;
        QVector<std::shared_ptr<Item>> items;
    };
    explicit DeserializeJob(QObject *parent = nullptr);
    ~DeserializeJob() override;

    [[nodiscard]] bool canStart() const;

    void start();
    [[nodiscard]] QJsonObject jsonObject() const;
    void setJsonObject(const QJsonObject &newJsonObject);

Q_SIGNALS:
    void deserializeDone(const DeserializeJob::DeserializeInfo &info);

private:
    LIBDRAWYWIDGETS_NO_EXPORT std::shared_ptr<Item> createItem(Item::Type type);
    LIBDRAWYWIDGETS_NO_EXPORT void deserializeItems();
    QJsonObject mJsonObject;
};
