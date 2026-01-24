/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "item/item.hpp"
#include "libdrawywidgets_private_export.h"
#include <QObject>
#include <QPointF>
class QDebug;
class QJsonObject;
class LIBDRAWYWIDGETS_TESTS_EXPORT SerializeJob : public QObject
{
    Q_OBJECT
public:
    struct SerializeInfo {
        QPointF offsetPos{0, 0};
        qreal zoomFactor{1.0};
        QList<std::shared_ptr<Item>> items;
    };

    explicit SerializeJob(QObject *parent = nullptr);
    ~SerializeJob() override;

    [[nodiscard]] bool canStart() const;

    void start();

    [[nodiscard]] SerializeInfo serializeInfo() const;
    void setSerializeInfo(const SerializeInfo &newSerializeInfo);

Q_SIGNALS:
    void serializeDone(const QJsonObject &obj);

private:
    void serializeItems();
    SerializeInfo mSerializeInfo;
};
LIBDRAWYWIDGETS_EXPORT QDebug operator<<(QDebug d, const SerializeJob::SerializeInfo &t);
