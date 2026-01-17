// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <qglobal.h>

#include <QPoint>
#include <QString>

class Event
{
public:
    Event();
    const QPoint &pos() const;
    [[nodiscard]] Qt::MouseButton button() const;
    [[nodiscard]] qreal pressure() const;
    [[nodiscard]] QString text() const;
    [[nodiscard]] int key() const;
    [[nodiscard]] Qt::KeyboardModifiers modifiers() const;

    void setPos(const QPoint &point, qreal const scale = 1.0);
    void setButton(Qt::MouseButton btn);
    void setPressure(qreal pressure);
    void setKey(int key);
    void setText(const QString &text);
    void setModifiers(Qt::KeyboardModifiers modifiers);

private:
    Qt::MouseButton m_button{Qt::NoButton};
    QPoint m_pos;
    qreal m_pressure{1.0};
    QString m_text;
    int m_key{};
    Qt::KeyboardModifiers m_modifiers;
};
