/*
  SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>

  SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once
#include "item/factory/itemfactory.hpp"
#include "libdrawywidgets_export.h"
#include "properties/property.hpp"
#include <QObject>

class LIBDRAWYWIDGETS_EXPORT PluginForm : public QObject
{
    Q_OBJECT
public:
    struct LIBDRAWYWIDGETS_EXPORT PluginFormInfo {
        QString pluginName;
        QString toolTip;
        QString name;
        QString iconName;
        QList<Property::Type> properties;
    };

    explicit PluginForm(QObject *parent = nullptr);
    ~PluginForm() override;

    [[nodiscard]] virtual int order() const = 0;

    [[nodiscard]] virtual QString title() const = 0;

    [[nodiscard]] virtual QString pluginName() const = 0;

    [[nodiscard]] QList<PluginFormInfo> pluginFormInfos() const;

    [[nodiscard]] virtual std::unique_ptr<ItemFactory> generateItemFactory(const PluginForm::PluginFormInfo &pluginInfo) = 0;

    [[nodiscard]] virtual std::shared_ptr<Item> createItem(const QString &customPluginName) = 0;

protected:
    QList<PluginForm::PluginFormInfo> mPluginFormInfoList;
};
