// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QInputMethodEvent>
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
    [[nodiscard]] QString preeditString() const;
    [[nodiscard]] QList<QInputMethodEvent::Attribute> attributes() const;
    [[nodiscard]] QString commitString() const;
    [[nodiscard]] int replacementStart() const;
    [[nodiscard]] int replacementLength() const;
    [[nodiscard]] int key() const;
    [[nodiscard]] Qt::KeyboardModifiers modifiers() const;

    void setPos(const QPoint &point, const qreal scale = 1.0);
    void setButton(Qt::MouseButton btn);
    void setPressure(qreal pressure);
    void setKey(int key);
    void setText(const QString &text);
    void setPreeditString(const QString &preeditString, const QList<QInputMethodEvent::Attribute> &attributes = {});
    void setCommitString(const QString &commitString, int replacementStart = -1, int replacementLength = -1);
    void setModifiers(Qt::KeyboardModifiers modifiers);

private:
    Qt::MouseButton m_button{Qt::NoButton};
    QPoint m_pos;
    qreal m_pressure{1.0};
    QString m_text;
    QString m_preeditText;
    QList<QInputMethodEvent::Attribute> m_attributes;
    QString m_commitText;
    int m_replacementStart{-1};
    int m_replacementLength{-1};
    int m_key{};
    Qt::KeyboardModifiers m_modifiers;
};
