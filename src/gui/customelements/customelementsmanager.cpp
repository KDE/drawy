/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "customelementsmanager.hpp"
#include <QJsonObject>
using namespace Qt::Literals::StringLiterals;
CustomElementsManager::CustomElementsManager(QObject *parent)
    : QObject{parent}
{
}

CustomElementsManager::~CustomElementsManager() = default;

CustomElementsManager *CustomElementsManager::self()
{
    static CustomElementsManager s_self;
    return &s_self;
}

void CustomElementsManager::loadCustomElements()
{
    // TODO
}

void CustomElementsManager::saveCustomElements()
{
    // TODO
}

bool CustomElementsManager::isEmpty() const
{
    return mCustomElements.isEmpty();
}

#include "moc_customelementsmanager.cpp"
