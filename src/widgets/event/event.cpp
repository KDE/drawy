// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "event.hpp"

Event::Event() = default;

void Event::setPos(const QPoint &point, const qreal scale)
{
    m_pos = point * scale;
}

void Event::setButton(Qt::MouseButton btn)
{
    m_button = btn;
}

void Event::setPressure(qreal pressure)
{
    m_pressure = pressure;
}

qreal Event::pressure() const
{
    return m_pressure;
}

const QPoint &Event::pos() const
{
    return m_pos;
}

Qt::MouseButton Event::button() const
{
    return m_button;
}

QString Event::text() const
{
    return m_text;
}
QString Event::preeditString() const
{
    return m_preeditText;
}

QList<QInputMethodEvent::Attribute> Event::attributes() const
{
    return m_attributes;
}

QString Event::commitString() const
{
    return m_commitText;
}

int Event::replacementStart() const
{
    return m_replacementStart;
}

int Event::replacementLength() const
{
    return m_replacementLength;
}

int Event::key() const
{
    return m_key;
}

Qt::KeyboardModifiers Event::modifiers() const
{
    return m_modifiers;
}

void Event::setKey(int key)
{
    m_key = key;
}

void Event::setText(const QString &text)
{
    m_text = text;
}

void Event::setPreeditString(const QString &preeditString, const QList<QInputMethodEvent::Attribute> &attributes)
{
    m_preeditText = preeditString;
    m_attributes = attributes;
}

void Event::setCommitString(const QString &commitString, const int replacementStart, const int replacementLength)
{
    m_commitText = commitString;
    m_replacementStart = replacementStart;
    m_replacementLength = replacementLength;
}

void Event::setModifiers(Qt::KeyboardModifiers modifiers)
{
    m_modifiers = modifiers;
}
