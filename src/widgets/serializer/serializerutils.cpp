/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "serializerutils.hpp"
#include "common/utils/compression.hpp"
#include "drawy_debug.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPointF>
using namespace Qt::Literals::StringLiterals;
int SerializerUtils::version()
{
    return 1;
}

QJsonObject SerializerUtils::toJson(const QPointF &point)
{
    QJsonObject result{};
    result[u"x"_s] = QJsonValue(point.x());
    result[u"y"_s] = QJsonValue(point.y());

    return result;
}

QByteArray SerializerUtils::compressData(const QJsonObject &obj)
{
    QJsonDocument doc{obj};
    const auto data{doc.toJson(QJsonDocument::Compact)};
    const auto compressedData{Common::Utils::Compression::compressData(data)};
    return compressedData;
}

void SerializerUtils::saveInFile(const QJsonObject &obj, const QString &fileName)
{
    QFile file{fileName};
    if (!file.open(QIODevice::WriteOnly)) {
        qCWarning(DRAWY_LOG) << "Error: There was an error opening the save file.";
        return;
    }
    const QByteArray compressedData = SerializerUtils::compressData(obj);
    const qint64 written = file.write(compressedData);
    file.close();

    if (written != compressedData.size()) {
        qCWarning(DRAWY_LOG) << "Warning: not all bytes were written";
    }

    qCDebug(DRAWY_LOG) << "Saved to file: " << fileName;
}
