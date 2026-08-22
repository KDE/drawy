/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "customelementsmanager.hpp"
#include "customelements/customelementsmodel.hpp"
#include "customelements/customelementsutils.hpp"
#include "drawy_gui_debug.h"
#include "item/item.hpp"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <utility>
using namespace Qt::Literals::StringLiterals;
CustomElementsManager::CustomElementsManager(QObject *parent)
    : QObject{parent}
    , mCustomElementsModel(new CustomElementsModel(this))
{
}

CustomElementsModel *CustomElementsManager::customElementsModel() const
{
    return mCustomElementsModel;
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
        QList<CustomElement> customElements;
        const QJsonArray array = obj["librairies"_L1].toArray();
        for (const auto &r : array) {
            CustomElement e;
            e.load(r.toObject());
            if (e.isValid()) {
                customElements.append(std::move(e));
            }
        }
        mCustomElementsModel->setCustomElements(customElements);
    }
}

void CustomElementsManager::saveCustomElements(const QString &fileName)
{
    QJsonObject obj;
    obj["version"_L1] = QString::number(1);
    QJsonArray elementsObj;
    for (const auto &element : mCustomElementsModel->customElements()) {
        elementsObj.append(element.save());
    }
    obj["librairies"_L1] = elementsObj;
    const auto path = fileName.isEmpty() ? CustomElementsUtils::customElementsFilePath() : fileName;
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
    if (mCustomElementsModel->customElements().isEmpty()) {
        qCWarning(DRAWY_GUI_LOG) << "Custom Elements list is empty";
        return;
    }
    saveCustomElements(fileName);
}

void CustomElementsManager::addItem(const std::shared_ptr<Item> &item)
{
    CustomElement element;
    element.setItem(item);
    mCustomElementsModel->addCustomElement(element);
    saveCustomElements();
}

bool CustomElementsManager::isEmpty() const
{
    return mCustomElementsModel->isEmpty();
}

#include "moc_customelementsmanager.cpp"
