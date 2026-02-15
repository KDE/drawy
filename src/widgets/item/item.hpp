// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawywidgets_private_export.h"
#include <QPainter>
#include <QRect>

#include "properties/property.hpp"
class QDebug;
class QJsonObject;
class LIBDRAWYWIDGETS_TESTS_EXPORT Item
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

    [[nodiscard]] int boundingBoxPadding() const;

    virtual void setProperty(const Property::Type propertyType, Property newObj);
    [[nodiscard]] virtual Property property(const Property::Type propertyType) const;
    virtual QList<Property> properties() const;
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
        Invalid,
    };
    Q_ENUM(BackgroundType)

    [[nodiscard]] virtual FormType formType() const = 0;

    virtual void updateAfterProperty();

    [[nodiscard]] virtual QJsonObject serialize(int zorder) const = 0;
    virtual void deserialize(const QJsonObject &obj) = 0;

    [[nodiscard]] bool operator==(const Item &other) const;

    [[nodiscard]] virtual const QTransform &transformObj() const;
    void setTransform(const QTransform &transformObj);

    virtual void translate(const QPointF &amount);
    virtual void rotate(const int angleDeg, const QPointF pivot);
    virtual void resize(const qreal amountX, const qreal amountY, const QPointF center);
    virtual void normalize();

    [[nodiscard]] virtual bool needsCaching() const;
    [[nodiscard]] bool isDirty() const;
    void setDirty(bool value);

    [[nodiscard]] bool hasProperty(Property::Type propertyType) const;

    [[nodiscard]] QByteArray id() const;
    void setId(const QByteArray &newId);

    [[nodiscard]] bool locked() const;
    void setLocked(bool newLocked);

protected:
    QByteArray m_id;
    QRectF m_boundingBox{};
    std::unordered_map<Property::Type, Property> m_properties{};

    QTransform m_transform{};
    bool m_isDirty{true};
    bool m_locked = false;

    virtual void drawItem(QPainter &painter, const QPointF &offset) const = 0;
};
LIBDRAWYWIDGETS_EXPORT QDebug operator<<(QDebug d, const Item &t);
