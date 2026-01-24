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
class LIBDRAWYWIDGETS_TESTS_EXPORT SaveAsJob : public QObject
{
    Q_OBJECT
public:
    struct SaveAsInfo {
        QString filePath;
        QPointF offsetPos{0, 0};
        qreal zoomFactor{1.0};
        QList<std::shared_ptr<Item>> items;
    };

    explicit SaveAsJob(QObject *parent = nullptr);
    ~SaveAsJob() override;

    [[nodiscard]] bool canStart() const;

    void start();

    [[nodiscard]] SaveAsInfo saveAsInfo() const;
    void setSaveAsInfo(const SaveAsInfo &newSaveAsInfo);

Q_SIGNALS:
    void saveFileDone(const QJsonObject &obj);

private:
    void slotSerializeDone(const QJsonObject &obj);
    SaveAsInfo mSaveAsInfo;
};
LIBDRAWYWIDGETS_EXPORT QDebug operator<<(QDebug d, const SaveAsJob::SaveAsInfo &t);
