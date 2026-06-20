// SPDX-FileCopyrightText: 2026 Nikolay Kochulin <basiqueevangelist@yandex.ru>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mime/mimemanager.hpp"
#include "context/applicationcontext.hpp"
#include "item/item.hpp"
#include "mime/drawymimehandler.hpp"
#include "mime/imagemimehandler.hpp"
#include "mime/rendermimehandler.hpp"
#include "mime/svgmimehandler.hpp"
#include "mime/textmimehandler.hpp"
#include <QMimeData>

MimeManager::MimeManager(ApplicationContext *parent)
    : QObject(parent)
{
    registerHandler(std::make_unique<DrawyMimeHandler>());
    registerHandler(std::make_unique<ImageMimeHandler>(parent));
    registerHandler(std::make_unique<TextMimeHandler>());
    registerHandler(std::make_unique<SvgMimeHandler>());
    registerHandler(std::make_unique<RenderMimeHandler>());
}

MimeManager::~MimeManager() = default;

void MimeManager::registerHandler(std::unique_ptr<MimeHandler> &&handler)
{
    m_handlers.push_back(std::move(handler));
}

QList<std::shared_ptr<Item>> MimeManager::readData(const QMimeData &mimeData) const
{
    for (auto &handler : m_handlers) {
        const QList<std::shared_ptr<Item>> items{handler->tryReadData(mimeData)};

        if (!items.isEmpty()) {
            return items;
        }
    }

    return {};
}

void MimeManager::writeData(QMimeData &mimeData, const QList<std::shared_ptr<Item>> &selectedItems) const
{
    for (auto &handler : m_handlers) {
        handler->contributeData(mimeData, selectedItems);
    }
}

#include "moc_mimemanager.cpp"
