// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "mime/mimehandler.hpp"
#include <QObject>
#include <memory>
#include <vector>

class Item;
class QMimeData;
class ApplicationContext;

class MimeManager : public QObject
{
    Q_OBJECT
public:
    explicit MimeManager(ApplicationContext *parent = nullptr);
    ~MimeManager() override;

    void registerHandler(std::unique_ptr<MimeHandler> &&handler);

    [[nodiscard]] QList<std::shared_ptr<Item>> readData(const QMimeData &mimeData) const;
    void writeData(QMimeData &mimeData, const QList<std::shared_ptr<Item>> &selectedItems) const;

private:
    std::vector<std::unique_ptr<MimeHandler>> m_handlers;
};
