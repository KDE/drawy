/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "imagetool.hpp"
#include "command/commandhistory.hpp"
#include "command/insertitemcommand.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "event/event.hpp"
#include "item/factory/imagefactory.hpp"
#include "item/image.hpp"
#include "properties/widgets/propertymanager.hpp"
#include <KLocalizedString>
#include <QFileDialog>
#include <QImageReader>
#include <QList>

using namespace Qt::Literals::StringLiterals;

ImageTool::ImageTool(ApplicationContext *context)
    : DrawingTool(context)
{
    m_itemFactory = std::make_unique<ImageFactory>();
    m_properties = {Property::Type::Opacity};
}

QString ImageTool::tooltip() const
{
    return i18nc("@info:tooltip", "Image Tool");
}

QString ImageTool::icon() const
{
    return u"insert-image"_s;
}

Tool::Type ImageTool::type() const
{
    return Tool::Type::Image;
}

void ImageTool::mouseReleased(ApplicationContext *context)
{
    UIContext *uiContext{context->uiContext()};

    if (uiContext->appEvent()->button() == Qt::LeftButton) {
        const QPointF lastPoint = uiContext->appEvent()->pos();
        QString filter;
        const QList<QByteArray> supportedImage = QImageReader::supportedImageFormats();
        for (const QByteArray &ba : supportedImage) {
            if (!filter.isEmpty()) {
                filter += u' ';
            }
            filter += "*."_L1 + QString::fromLatin1(ba);
        }
        filter = u"%1 (%2)"_s.arg(i18n("Image"), filter);

        const QString fileName = QFileDialog::getOpenFileName(nullptr, i18nc("@title:window", "Insert Image"), {}, filter);
        if (fileName.isEmpty()) {
            return;
        }
        auto spatialContext{context->spatialContext()};
        auto renderingContext{context->renderingContext()};
        auto commandHistory{spatialContext->commandHistory()};
        CoordinateTransformer &transformer{spatialContext->coordinateTransformer()};
        QPixmap pixmap;
        pixmap.load(fileName);

        std::shared_ptr<ImageItem> curItem = std::dynamic_pointer_cast<ImageItem>(m_itemFactory->create());
        curItem->setBox(QRectF(transformer.viewToWorld(lastPoint), transformer.viewToWorld(QSizeF(100, 100))));

        curItem->setProperty(Property::Type::Opacity, uiContext->propertyManager()->value(Property::Type::Opacity));
        curItem->setPixmap(pixmap);

        QList<std::shared_ptr<Item>> lst;
        lst << curItem;
        commandHistory->insert(std::make_shared<InsertItemCommand>(lst));

        renderingContext->markForRender();
        renderingContext->markForUpdate();
    }
}
