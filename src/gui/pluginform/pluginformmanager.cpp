/*
   SPDX-FileCopyrightText: 2025-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "pluginformmanager.hpp"

#include "pluginform.hpp"
#include <KLocalizedString>
#include <KPluginFactory>
#include <KPluginMetaData>
#include <QFileInfo>
#include <QSet>
using namespace Qt::Literals::StringLiterals;
PluginFormManager::PluginFormManager(QObject *parent)
    : QObject(parent)
{
    initializePluginList();
}

PluginFormManager::~PluginFormManager() = default;

PluginFormManager *PluginFormManager::self()
{
    static PluginFormManager s_self;
    return &s_self;
}

void PluginFormManager::initializePluginList()
{
    const QList<KPluginMetaData> plugins = KPluginMetaData::findPlugins(u"drawypluginforms"_s);

    // qDebug() << "plugins  " << plugins;
    QListIterator<KPluginMetaData> i(plugins);
    i.toBack();
    QSet<QString> unique;
    while (i.hasPrevious()) {
        PluginFormManagerInfo info;
        const KPluginMetaData data = i.previous();

        // 1) get plugin data => name/description etc.
        info.pluginData = createPluginMetaData(data);
        // 2) look at if plugin is activated
        info.metaDataFileNameBaseName = QFileInfo(data.fileName()).baseName();
        info.metaDataFileName = data.fileName();
        info.data = data;
        // only load plugins once, even if found multiple times!
        if (unique.contains(info.metaDataFileNameBaseName)) {
            continue;
        }
        info.plugin = nullptr;
        mPluginList.push_back(info);
        unique.insert(info.metaDataFileNameBaseName);
    }
    const QList<PluginFormManagerInfo>::iterator end(mPluginList.end());
    for (QList<PluginFormManagerInfo>::iterator it = mPluginList.begin(); it != end; ++it) {
        loadPlugin(&(*it));
    }
}

void PluginFormManager::loadPlugin(PluginFormManagerInfo *item)
{
    if (auto plugin = KPluginFactory::instantiatePlugin<PluginForm>(item->data, this, QVariantList() << item->metaDataFileNameBaseName).plugin) {
        item->plugin = plugin;
        mPluginDataList.append(item->pluginData);
    }
}

QList<PluginUtilData> PluginFormManager::pluginDataList() const
{
    return mPluginDataList;
}

PluginForm *PluginFormManager::pluginFromIdentifier(const QString &identifier) const
{
    const auto it = std::find_if(mPluginList.constBegin(), mPluginList.constEnd(), [identifier](const PluginFormManagerInfo &info) {
        return info.pluginData.mIdentifier == identifier;
    });
    if (it != mPluginList.end()) {
        return (*it).plugin;
    }
    return nullptr;
}

bool PluginFormManager::isEmpty() const
{
    return mPluginDataList.isEmpty();
}

QList<PluginForm *> PluginFormManager::pluginsList() const
{
    QList<PluginForm *> lst;
    lst.reserve(mPluginList.count());
    const QList<PluginFormManagerInfo>::ConstIterator end(mPluginList.constEnd());
    for (QList<PluginFormManagerInfo>::ConstIterator it = mPluginList.constBegin(); it != end; ++it) {
        if (auto plugin = (*it).plugin) {
            lst << plugin;
        }
    }
    return lst;
}

PluginUtilData PluginFormManager::createPluginMetaData(const KPluginMetaData &metaData)
{
    PluginUtilData pluginData;
    pluginData.mDescription = metaData.description();
    pluginData.mName = metaData.name();
    pluginData.mIdentifier = metaData.pluginId();
    return pluginData;
}

std::unique_ptr<ItemFactory> PluginFormManager::generateItemFactory(const PluginForm::PluginFormInfo &pluginInfo)
{
    for (const auto &p : std::as_const(mPluginList)) {
        if (p.plugin->pluginName() == pluginInfo.pluginName) {
            return p.plugin->generateItemFactory(pluginInfo);
        }
    }
    return {};
}

std::shared_ptr<Item> PluginFormManager::createItem(const QString &pluginName, const QString &customPluginName)
{
    for (const auto &p : std::as_const(mPluginList)) {
        if (p.plugin->pluginName() == pluginName) {
            return p.plugin->createItem(customPluginName);
        }
    }
    return {};
}

#include "moc_pluginformmanager.cpp"
