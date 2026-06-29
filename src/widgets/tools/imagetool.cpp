/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "imagetool.hpp"
#include "command/commandhistory.hpp"
#include "command/insertitemcommand.hpp"
#include "components/toolbar.hpp"
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
    const QList<QByteArray> supportedImage = QImageReader::supportedImageFormats();
    for (const QByteArray &ba : supportedImage) {
        if (!m_filter.isEmpty()) {
            m_filter += u' ';
        }
        m_filter += "*."_L1 + QString::fromLatin1(ba);
    }
    m_filter = u"%1 (%2)"_s.arg(i18n("Image"), m_filter);
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
    const UIContext *uiContext{context->uiContext()};

    if (uiContext->appEvent()->button() == Qt::LeftButton) {
        const QPointF lastPoint = uiContext->appEvent()->pos();
        const QString fileName = QFileDialog::getOpenFileName(nullptr, i18nc("@title:window", "Insert Image"), {}, m_filter);
        if (fileName.isEmpty()) {
            return;
        }
        auto spatialContext{context->spatialContext()};
        auto renderingContext{context->renderingContext()};
        auto commandHistory{spatialContext->commandHistory()};
        CoordinateTransformer &transformer{spatialContext->coordinateTransformer()};
        QPixmap pixmap;
        pixmap.load(fileName);

        const std::shared_ptr<ImageItem> curItem = std::dynamic_pointer_cast<ImageItem>(m_itemFactory->create());
        curItem->setBox(QRectF(transformer.viewToWorld(lastPoint), transformer.viewToWorld(QSizeF(100, 100))));

        curItem->setProperty(Property::Type::Opacity, uiContext->propertyManager()->value(Property::Type::Opacity));
        curItem->setPixmap(pixmap);

        QList<std::shared_ptr<Item>> lst{curItem};
        commandHistory->push(std::make_shared<InsertItemCommand>(lst));

        renderingContext->markForRender();
        renderingContext->markForUpdate();
        context->uiContext()->toolBar()->changeTool(Tool::Type::Selection);
    }
}
