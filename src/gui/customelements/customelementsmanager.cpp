/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "customelementsmanager.hpp"
#include "customelements/customelementsutils.hpp"
#include "drawy_gui_debug.h"
#include "item/item.hpp"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
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
    const auto path = CustomElementsUtils::customElementsFilePath();
    if (QFile::exists(path)) {
        QFile file(path);
        if (!file.open(QFile::ReadOnly)) {
            qCWarning(DRAWY_GUI_LOG) << "Impossible to open file: " << file.errorString();
            return;
        }
        const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        const QJsonObject obj = doc.object();
        const int version = obj["version"_L1].toInt();
        if (version != 1) {
            qCWarning(DRAWY_GUI_LOG) << "Invalid version" << version;
            return;
        }
        mCustomElements.clear();
        const QJsonArray array = obj["librairies"_L1].toArray();
        for (const auto &r : array) {
            CustomElement e;
            e.load(r.toObject());
            if (e.isValid()) {
                mCustomElements.append(e);
            }
        }
    }
}

void CustomElementsManager::saveCustomElements()
{
    QJsonObject obj;
    obj["version"_L1] = QString::number(1);
    QJsonArray elementsObj;
    for (const auto &element : std::as_const(mCustomElements)) {
        elementsObj.append(element.save());
    }
    obj["librairies"_L1] = elementsObj;
    const auto path = CustomElementsUtils::customElementsFilePath();
    QFile file(path);
    if (!file.open(QFile::WriteOnly)) {
        qCWarning(DRAWY_GUI_LOG) << "Impossible to open file: " << file.errorString();
        return;
    }
    const QJsonDocument doc{obj};
    const auto data{doc.toJson(QJsonDocument::Compact)};

    file.write(data);
    file.close();
}

void CustomElementsManager::exportToFile(const QString &fileName)
{
    Q_UNUSED(fileName);
    // TODO
}

void CustomElementsManager::addItem(const std::shared_ptr<Item> &item)
{
    CustomElement element;
    element.setItem(item);
    mCustomElements.append(element);
    saveCustomElements();
}

bool CustomElementsManager::isEmpty() const
{
    return mCustomElements.isEmpty();
}

#include "moc_customelementsmanager.cpp"
