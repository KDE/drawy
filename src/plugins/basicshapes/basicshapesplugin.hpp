/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include "pluginform/pluginform.hpp"
#include <QVariant>
class BasicShapesPlugin : public PluginForm
{
    Q_OBJECT
public:
    enum class BasicShapesPluginType : int8_t {
        Triangle = 0,
        XBox,
    };
    Q_ENUM(BasicShapesPluginType)

    explicit BasicShapesPlugin(QObject *parent = nullptr, const QVariantList & = {});
    ~BasicShapesPlugin() override;

    [[nodiscard]] int order() const override;
    [[nodiscard]] QString title() const override;
    [[nodiscard]] QString pluginName() const override;
    [[nodiscard]] std::unique_ptr<ItemFactory> generateItemFactory(const PluginForm::PluginFormInfo &pluginInfo) override;
    [[nodiscard]] std::shared_ptr<Item> createItem(const QString &customPluginName) override;
};
