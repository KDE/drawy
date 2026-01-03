// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "serializer.hpp"

#include <QDebug>
#include <QFileDialog>
#include <QJsonDocument>
#include <QStandardPaths>
#include <memory>

#include "common/constants.hpp"
#include "common/utils/compression.hpp"
#include "context/applicationcontext.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/quadtree.hpp"
#include "drawy_debug.h"
#include "item/freeform.hpp"
#include "item/item.hpp"
#include "item/polygon.hpp"
#include "item/text.hpp"

Serializer::Serializer()
{
}

void Serializer::serialize(ApplicationContext *context)
{
    QVector<std::shared_ptr<Item>> items{context->spatialContext()->quadtree()->getAllItems()};

    QJsonArray array{};
    for (auto &item : items) {
        array.push_back(toJson(item));
    }

    m_object[u"items"_s] = array;

    QPointF offsetPos{context->spatialContext()->offsetPos()};
    m_object[u"offset_pos"_s] = toJson(offsetPos);

    qreal zoomFactor{context->renderingContext()->zoomFactor()};
    m_object[u"zoom_factor"_s] = zoomFactor;
}

QJsonObject Serializer::toJson(const std::shared_ptr<Item> &item)
{
    QJsonObject obj{};

    obj[u"type"_s] = QJsonValue(static_cast<int>(item->type()));
    obj[u"bounding_box"_s] = toJson(item->boundingBox());
    obj[u"bounding_box_padding"_s] = QJsonValue(item->boundingBoxPadding());
    obj[u"properties"_s] = toJson(item->properties());

    switch (item->type()) {
    case Item::Freeform: {
        std::shared_ptr<FreeformItem> freeform{std::dynamic_pointer_cast<FreeformItem>(item)};
        obj[u"points"_s] = toJson(freeform->points());
        obj[u"pressures"_s] = toJson(freeform->pressures());
        break;
    }
    case Item::Rectangle:
    case Item::Ellipse:
    case Item::Arrow:
    case Item::Line: {
        std::shared_ptr<PolygonItem> polygon{std::dynamic_pointer_cast<PolygonItem>(item)};
        obj[u"start"_s] = toJson(polygon->start());
        obj[u"end"_s] = toJson(polygon->end());
        break;
    }
    case Item::Text: {
        std::shared_ptr<TextItem> text{std::dynamic_pointer_cast<TextItem>(item)};
        obj[u"text"_s] = QJsonValue(text->text());
    }
    }

    return obj;
}

QJsonObject Serializer::toJson(const Property &property)
{
    QJsonObject result{};

    result[u"type"_s] = property.type();
    result[u"value"_s] = QJsonValue::fromVariant(property.variant());

    return result;
}

QJsonObject Serializer::toJson(const QRectF &rect)
{
    QJsonObject result{};
    result[u"x"_s] = QJsonValue(rect.x());
    result[u"y"_s] = QJsonValue(rect.y());
    result[u"width"_s] = QJsonValue(rect.width());
    result[u"height"_s] = QJsonValue(rect.height());

    return result;
}

QJsonObject Serializer::toJson(const QPointF &point)
{
    QJsonObject result{};
    result[u"x"_s] = QJsonValue(point.x());
    result[u"y"_s] = QJsonValue(point.y());

    return result;
}

void Serializer::saveToFile()
{
    QJsonDocument doc{m_object};

    qCDebug(DRAWY_LOG) << "Saving...";

    QDir homeDir{QDir::home()};

    auto text = QObject::tr("Untitled.%1").arg(Common::drawyFileExt);
    const QString defaultFilePath = homeDir.filePath(text);

    text = QObject::tr("Drawy (*.%1)").arg(Common::drawyFileExt);
    QString fileName{QFileDialog::getSaveFileName(nullptr, QObject::tr("Save File"), defaultFilePath, text)};

    auto data{doc.toJson(QJsonDocument::Compact)};
    auto compressedData{Common::Utils::Compression::compressData(data)};

    QFile file{fileName};
    if (!file.open(QIODevice::WriteOnly)) {
        qCWarning(DRAWY_LOG) << "Error: There was an error opening the save file.";
        return;
    }

    qint64 written = file.write(compressedData);
    file.close();

    if (written != compressedData.size()) {
        qCWarning(DRAWY_LOG) << "Warning: not all bytes were written";
    }

    qCDebug(DRAWY_LOG) << "Saved to file: " << fileName;
}
