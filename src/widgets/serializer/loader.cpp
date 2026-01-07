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

#include "common/constants.hpp"
#include "common/utils/compression.hpp"
#include "context/applicationcontext.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"
#include "item/arrow.hpp"
#include "item/ellipse.hpp"
#include "item/freeform.hpp"
#include "item/item.hpp"
#include "item/line.hpp"
#include "item/rectangle.hpp"
#include "item/text.hpp"
using namespace Qt::Literals::StringLiterals;

void Loader::loadFromFile(ApplicationContext *context, const QString &openFileName)
{
    QString fileName;
    if (openFileName.isEmpty()) {
        // file filter
        const QString filter = QObject::tr("Drawy (*.%1)").arg(Common::drawyFileExt);

        // ask for file (handle cancel)
        const QDir homeDir{QDir::home()};
        fileName = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open File"), homeDir.path(), filter);
        if (fileName.isEmpty())
            return;
    } else {
        fileName = openFileName;
    }

    QFile file{fileName};
    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(DRAWY_LOG) << "Failed to open file:" << file.errorString();
        return;
    }

    const QByteArray compressedByteArray = file.readAll();
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

    QJsonArray itemsArray = array(value(docObj, u"items"_s));
    for (const QJsonValueRef &v : itemsArray) {
        QJsonObject itemObj = object(v);
        std::shared_ptr<Item> item = createItem(itemObj);
        quadtree->insertItem(item);
    }

    qreal zoomFactor = value(docObj, u"zoom_factor"_s).toDouble();
    context->renderingContext()->setZoomFactor(zoomFactor);

    QPointF offsetPos = toPointF(value(docObj, u"offset_pos"_s));
    context->spatialContext()->setOffsetPos(offsetPos);

    context->spatialContext()->cacheGrid()->markAllDirty();
    context->renderingContext()->markForRender();
    context->renderingContext()->markForUpdate();
}

std::shared_ptr<Item> Loader::createItem(const QJsonObject &obj)
{
    Item::Type type{Item::convertStringToEnum(value(obj, u"type"_s).toString())};

    std::shared_ptr<Item> item;
    switch (type) {
    case Item::Type::Freeform: {
        std::shared_ptr<FreeformItem> cur{std::make_shared<FreeformItem>()};
        QJsonArray points = array(value(obj, u"points"_s));
        QJsonArray pressures = array(value(obj, u"pressures"_s));

        qsizetype len{points.size()};
        for (qsizetype pos{0}; pos < len; pos++) {
            QPointF point{toPointF(points[pos])};

            cur->addPoint(point, pressures[pos].toDouble());
        }

        item = cur;
        break;
    }
    case Item::Type::Rectangle: {
        std::shared_ptr<RectangleItem> cur{std::make_shared<RectangleItem>()};
        cur->setStart(toPointF(value(obj, u"start"_s)));
        cur->setEnd(toPointF(value(obj, u"end"_s)));

        item = cur;
        break;
    }
    case Item::Type::Line: {
        std::shared_ptr<LineItem> cur{std::make_shared<LineItem>()};
        cur->setStart(toPointF(value(obj, u"start"_s)));
        cur->setEnd(toPointF(value(obj, u"end"_s)));

        item = cur;
        break;
    }
    case Item::Type::Arrow: {
        std::shared_ptr<ArrowItem> cur{std::make_shared<ArrowItem>()};
        cur->setStart(toPointF(value(obj, u"start"_s)));
        cur->setEnd(toPointF(value(obj, u"end"_s)));

        item = cur;
        break;
    }
    case Item::Type::Ellipse: {
        std::shared_ptr<EllipseItem> cur{std::make_shared<EllipseItem>()};
        cur->setStart(toPointF(value(obj, u"start"_s)));
        cur->setEnd(toPointF(value(obj, u"end"_s)));

        item = cur;
        break;
    }
    case Item::Type::Text: {
        std::shared_ptr<TextItem> cur{std::make_shared<TextItem>()};
        QPointF topLeft = toPointF(value(obj, u"bounding_box"_s));

        cur->createTextBox(topLeft);
        cur->insertText(value(obj, u"text"_s).toString());

        item = cur;
        break;
    }
    }

    QJsonArray properties = array(value(obj, u"properties"_s));
    for (const QJsonValueRef &propertyValue : properties) {
        Property prop{createProperty(object(propertyValue))};
        item->setProperty(prop.type(), prop);
    }

    return item;
}

Property Loader::createProperty(const QJsonObject &obj)
{
    const Property::Type type{Property::convertStringToEnum(value(obj, u"type"_s).toString())};
    const QVariant val{value(obj, u"value"_s).toVariant()};

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

    if (obj.contains(u"x"_s) && obj.contains(u"y"_s)) {
        return QPointF{obj[u"x"].toDouble(), obj[u"y"].toDouble()};
    } else {
        qCWarning(DRAWY_LOG) << "Given point does not contain x and y properties";
        return {};
    }

    qCWarning(DRAWY_LOG) << "Given point value is not an object";
    return {};
}
