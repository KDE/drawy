/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "item/item.hpp"
#include <QPointF>
#include <QString>
class ApplicationContext;
namespace LoadJobUtil
{
struct DeserializePageInfo {
    QString name;
    QPointF offsetPos{0, 0};
    qreal zoomFactor{1.0};
    QList<std::shared_ptr<Item>> items;
};

struct DeserializeInfo {
    int currentPage = 0;
    QList<DeserializePageInfo> pages;
};

void loadFile(ApplicationContext *context, const LoadJobUtil::DeserializeInfo &info);
};
