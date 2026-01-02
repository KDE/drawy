// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "tool.hpp"

#include "drawy_debug.h"

Tool::Tool()
{
}

Tool::~Tool()
{
    qCDebug(DRAWY_LOG) << "Object deleted: Tool";
}

const QCursor &Tool::cursor() const
{
    return m_cursor;
}

const QVector<Property::Type> Tool::properties() const
{
    return m_properties;
}

void Tool::mousePressed([[maybe_unused]] ApplicationContext *context)
{
}
void Tool::mouseMoved([[maybe_unused]] ApplicationContext *context)
{
}
void Tool::mouseReleased([[maybe_unused]] ApplicationContext *context)
{
}
void Tool::keyPressed([[maybe_unused]] ApplicationContext *context)
{
}
void Tool::keyReleased([[maybe_unused]] ApplicationContext *context)
{
}
void Tool::mouseDoubleClick([[maybe_unused]] ApplicationContext *context)
{
}
void Tool::mouseTripleClick([[maybe_unused]] ApplicationContext *context)
{
}
void Tool::leave([[maybe_unused]] ApplicationContext *context)
{
}
void Tool::cleanup()
{
}
