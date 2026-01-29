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
    virtual bool intersects(const QLineF &rect) = 0;

    virtual void draw(QPainter &painter, const QPointF &offset) = 0;
    virtual void erase(QPainter &painter, const QPointF &offset) const;

    virtual const QRectF boundingBox() const;

    [[nodiscard]] int boundingBoxPadding() const;

    virtual void setProperty(const Property::Type propertyType, Property newObj);
    virtual const Property property(const Property::Type propertyType) const;
    virtual const QList<Property> properties() const;
    virtual const QList<Property::Type> propertyTypes() const;

    enum class Type : int8_t {
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
    Q_ENUM(Type)

    enum class StrokeType : int8_t {
        Solid,
        DashLine,
        DotLine,
        Invalid,
    };
    Q_ENUM(StrokeType)

    [[nodiscard]] virtual Type type() const = 0;

    virtual void updateAfterProperty();

    [[nodiscard]] virtual QJsonObject serialize() const = 0;
    virtual void deserialize(const QJsonObject &obj) = 0;

    [[nodiscard]] bool operator==(const Item &other) const;

    const QTransform &transformObj() const;

    virtual void translate(const QPointF &amount) = 0;

    [[nodiscard]] virtual bool needsCaching() const;
    [[nodiscard]] bool isDirty() const;
    void setDirty(bool value);

    [[nodiscard]] bool hasProperty(Property::Type propertyType) const;

    [[nodiscard]] QByteArray id() const;
    void setId(const QByteArray &newId);

    [[nodiscard]] bool locked() const;
    void setLocked(bool newLocked);

    [[nodiscard]] int angle() const;
    void setAngle(int newAngle);

protected:
    int m_angle = 0;
    QByteArray m_id;
    QRectF m_boundingBox{};
    std::unordered_map<Property::Type, Property> m_properties{};

    QTransform m_transform{};
    bool m_isDirty{true};
    bool m_locked = false;

    virtual void drawItem(QPainter &painter, const QPointF &offset) const = 0;
};
LIBDRAWYWIDGETS_EXPORT QDebug operator<<(QDebug d, const Item &t);
