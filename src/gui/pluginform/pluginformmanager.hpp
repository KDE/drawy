/*
   SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once
#include "item/factory/itemfactory.hpp"
#include "pluginform/pluginform.hpp"
#include <KPluginMetaData>
#include <QList>
#include <QObject>

class PluginForm;
class PluginUtilData
{
public:
    PluginUtilData() = default;

    QStringList mExtraInfo;
    QString mDescription;
    QString mIdentifier;
    QString mName;
};

class PluginFormManagerInfo
{
public:
    PluginFormManagerInfo() = default;

    KPluginMetaData data;
    QString metaDataFileNameBaseName;
    QString metaDataFileName;
    PluginUtilData pluginData;
    PluginForm *plugin = nullptr;
    bool isEnabled = true;
};

class LIBDRAWYGUI_EXPORT PluginFormManager final : public QObject
{
    Q_OBJECT
public:
    ~PluginFormManager() override;
    static PluginFormManager *self();

    [[nodiscard]] QList<PluginForm *> pluginsList() const;

    [[nodiscard]] QList<PluginUtilData> pluginDataList() const;

    [[nodiscard]] PluginForm *pluginFromIdentifier(const QString &identifier) const;

    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] std::unique_ptr<ItemFactory> generateItemFactory(const PluginForm::PluginFormInfo &pluginInfo);
    [[nodiscard]] std::shared_ptr<Item> createItem(const QString &pluginName, const QString &customPluginName);

private:
    explicit PluginFormManager(QObject *parent = nullptr);
    LIBDRAWYGUI_NO_EXPORT void initializePluginList();
    LIBDRAWYGUI_NO_EXPORT void loadPlugin(PluginFormManagerInfo *item);
    [[nodiscard]] LIBDRAWYGUI_NO_EXPORT PluginUtilData createPluginMetaData(const KPluginMetaData &metaData);
    QList<PluginFormManagerInfo> mPluginList;
    QList<PluginUtilData> mPluginDataList;
};
