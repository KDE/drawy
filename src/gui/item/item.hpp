// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawygui_export.h"
#include <QPainter>
#include <QRect>

#include "properties/property.hpp"
#include "transformhandlerutils.hpp"

class QDebug;
class QJsonObject;
class LIBDRAWYGUI_EXPORT Item
{
    Q_GADGET
public:
    Item();
    virtual ~Item();

    virtual bool intersects(const QRectF &rect) = 0;

    virtual void draw(QPainter &painter, const QPointF &offset) = 0;
    virtual void erase(QPainter &painter, const QPointF &offset) const;

    [[nodiscard]] virtual QRectF boundingBox() const;
    [[nodiscard]] virtual QRectF normalizedBoundingBox() const;
    [[nodiscard]] virtual QPolygonF displayBoundingBox() const; // could be rotated as well

    [[nodiscard]] virtual Property property(const Property::Type propertyType) const;
    virtual void setProperty(const Property::Type propertyType, Property newObj);
    virtual QList<Property> properties() const;
    virtual QList<Property::Type> allPropertyTypes() const;
    virtual QList<Property::Type> propertyTypes() const;

    enum class FormType : int8_t {
        Freeform,
        Rectangle,
        Ellipse,
        Line,
        Arrow,
        Text,
        Group,
        Diamond,
        Image,
        Custom,
        Invalid,
    };
    Q_ENUM(FormType)

    enum class StrokeType : int8_t {
        Solid,
        DashLine,
        DotLine,
        Invalid,
    };
    Q_ENUM(StrokeType)

    enum class BackgroundType : int8_t {
        Solid,
        CrossDiagonal,
        Diagonal,
        None,
        Invalid,
    };
    Q_ENUM(BackgroundType)

    enum class CornerRectangleType : int8_t {
        Rounded,
        Pointed,
    };
    Q_ENUM(CornerRectangleType)

    [[nodiscard]] virtual FormType formType() const = 0;

    virtual void updateAfterProperty();

    [[nodiscard]] virtual QJsonObject serialize(int zorder) const = 0;
    virtual void deserialize(const QJsonObject &obj) = 0;

    [[nodiscard]] bool operator==(const Item &other) const;

    [[nodiscard]] virtual const QTransform &transformObj() const;
    virtual void setTransform(const QTransform &transformObj);

    virtual void translate(const QPointF &amount);
    virtual void rotate(const qreal angleDeg, const QPointF pivot);
    virtual void resize(const QTransform operation);
    virtual void commitTransformation() = 0;

    virtual QList<TransformHandlerUtils::Type> transformHandlers() const;

    [[nodiscard]] virtual bool needsCaching() const;
    [[nodiscard]] bool isDirty() const;
    void setDirty(bool value);

    [[nodiscard]] virtual bool hasProperty(Property::Type propertyType) const;
    [[nodiscard]] virtual bool needsPropertyUpdate(const Property &property) const;

    [[nodiscard]] QByteArray id() const;
    void setId(const QByteArray &newId);

    [[nodiscard]] bool locked() const;
    void setLocked(bool newLocked);

    [[nodiscard]] virtual bool lockAspectRatioWhenResizing() const;

protected:
    [[nodiscard]] bool isFilled() const;
    QByteArray m_id;
    QRectF m_boundingBox;
    std::unordered_map<Property::Type, Property> m_properties;

    QTransform m_transform;
    bool m_isDirty{true};
    bool m_locked = false;

    virtual void drawItem(QPainter &painter, const QPointF &offset) const = 0;
};
LIBDRAWYGUI_EXPORT QDebug operator<<(QDebug d, const Item &t);
