// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "loader.hpp"

#include "drawy_debug.h"
#include <QDir>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>

#include "../common/constants.hpp"
#include "../common/utils/compression.hpp"
#include "../context/applicationcontext.hpp"
#include "../context/renderingcontext.hpp"
#include "../context/spatialcontext.hpp"
#include "../data-structures/cachegrid.hpp"
#include "../data-structures/quadtree.hpp"
#include "../item/arrow.hpp"
#include "../item/ellipse.hpp"
#include "../item/freeform.hpp"
#include "../item/item.hpp"
#include "../item/line.hpp"
#include "../item/rectangle.hpp"
#include "../item/text.hpp"

void Loader::loadFromFile(ApplicationContext *context)
{
    // file filter
    QString filter = QObject::tr("Drawy (*.%1)").arg(Common::drawyFileExt);

    // ask for file (handle cancel)
    QDir homeDir{QDir::home()};
    QString fileName = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open File"), homeDir.path(), filter);
    if (fileName.isEmpty())
        return;

    QFile file{fileName};
    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(DRAWY_LOG) << "Failed to open file:" << file.errorString();
        return;
    }

    QByteArray compressedByteArray = file.readAll();
    file.close();

    QByteArray byteArray;
    try {
        byteArray = Common::Utils::Compression::decompressData(compressedByteArray);
    } catch (const std::exception &ex) {
        qCWarning(DRAWY_LOG) << "Decompression failed:" << ex.what();

        QByteArray decoded = QByteArray::fromBase64(compressedByteArray);
        if (!decoded.isEmpty() && decoded.size() < compressedByteArray.size()) {
            try {
                byteArray = Common::Utils::Compression::decompressData(decoded);
            } catch (const std::exception &ex2) {
                qCWarning(DRAWY_LOG) << "Base64-decode fallback also failed:" << ex2.what();
                return;
            }
        } else {
            return;
        }
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray, &parseError);
    if (doc.isNull() || !doc.isObject()) {
        qCWarning(DRAWY_LOG) << "JSON parse failed:" << parseError.errorString() << "offset:" << parseError.offset;
        return;
    }

    QJsonObject docObj = doc.object();

    context->reset();
    QuadTree *quadtree{context->spatialContext()->quadtree()};

    QJsonArray itemsArray = array(value(docObj, "items"));
    for (const QJsonValueRef &v : itemsArray) {
        QJsonObject itemObj = object(v);
        std::shared_ptr<Item> item = createItem(itemObj);
        quadtree->insertItem(item);
    }

    qreal zoomFactor = value(docObj, "zoom_factor").toDouble();
    context->renderingContext()->setZoomFactor(zoomFactor);

    QPointF offsetPos = toPointF(value(docObj, "offset_pos"));
    context->spatialContext()->setOffsetPos(offsetPos);

    context->spatialContext()->cacheGrid()->markAllDirty();
    context->renderingContext()->markForRender();
    context->renderingContext()->markForUpdate();
}

std::shared_ptr<Item> Loader::createItem(const QJsonObject &obj)
{
    Item::Type type{static_cast<Item::Type>(value(obj, "type").toInt())};

    std::shared_ptr<Item> item;
    switch (type) {
    case Item::Freeform: {
        std::shared_ptr<FreeformItem> cur{std::make_shared<FreeformItem>()};
        QJsonArray points = array(value(obj, "points"));
        QJsonArray pressures = array(value(obj, "pressures"));

        qsizetype len{points.size()};
        for (qsizetype pos{0}; pos < len; pos++) {
            QPointF point{toPointF(points[pos])};

            cur->addPoint(point, pressures[pos].toDouble());
        }

        item = cur;
        break;
    }
    case Item::Rectangle: {
        std::shared_ptr<RectangleItem> cur{std::make_shared<RectangleItem>()};
        cur->setStart(toPointF(value(obj, "start")));
        cur->setEnd(toPointF(value(obj, "end")));

        item = cur;
        break;
    }
    case Item::Line: {
        std::shared_ptr<LineItem> cur{std::make_shared<LineItem>()};
        cur->setStart(toPointF(value(obj, "start")));
        cur->setEnd(toPointF(value(obj, "end")));

        item = cur;
        break;
    }
    case Item::Arrow: {
        std::shared_ptr<ArrowItem> cur{std::make_shared<ArrowItem>()};
        cur->setStart(toPointF(value(obj, "start")));
        cur->setEnd(toPointF(value(obj, "end")));

        item = cur;
        break;
    }
    case Item::Ellipse: {
        std::shared_ptr<EllipseItem> cur{std::make_shared<EllipseItem>()};
        cur->setStart(toPointF(value(obj, "start")));
        cur->setEnd(toPointF(value(obj, "end")));

        item = cur;
        break;
    }
    case Item::Text: {
        std::shared_ptr<TextItem> cur{std::make_shared<TextItem>()};
        QPointF topLeft = toPointF(value(obj, "bounding_box"));

        cur->createTextBox(topLeft);
        cur->insertText(value(obj, "text").toString());

        item = cur;
        break;
    }
    }

    QJsonArray properties = array(value(obj, "properties"));
    for (const QJsonValueRef &propertyValue : properties) {
        Property prop{createProperty(object(propertyValue))};
        item->setProperty(prop.type(), prop);
    }

    return item;
}

Property Loader::createProperty(const QJsonObject &obj)
{
    Property::Type type{static_cast<Property::Type>(value(obj, "type").toInt())};
    QVariant val{value(obj, "value").toVariant()};

    return Property{val, type};
}

QJsonValue Loader::value(const QJsonObject &obj, const QString &key)
{
    if (!obj.contains(key)) {
        qCWarning(DRAWY_LOG) << "Object does not contain key: " << key;
        return {};
    }

    return obj.value(key);
}

QJsonObject Loader::object(const QJsonValue &value)
{
    if (value.isUndefined() || !value.isObject()) {
        qCWarning(DRAWY_LOG) << "Value is not an object";
        return {};
    }

    return value.toObject();
}

QJsonArray Loader::array(const QJsonValue &value)
{
    if (value.isUndefined() || !value.isArray()) {
        qCWarning(DRAWY_LOG) << "Value is not an array";
        return {};
    }

    return value.toArray();
}

QPointF Loader::toPointF(const QJsonValue &val)
{
    QJsonObject obj = object(val);

    if (obj.contains("x") && obj.contains("y")) {
        return QPointF{obj["x"].toDouble(), obj["y"].toDouble()};
    } else {
        qCWarning(DRAWY_LOG) << "Given point does not contain x and y properties";
        return {};
    }

    qCWarning(DRAWY_LOG) << "Given point value is not an object";
    return {};
}
