// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "actionmanager.hpp"

#include <QDir>
#include <QFileDialog>
#include <memory>

#include "command/commandhistory.hpp"
#include "command/deselectcommand.hpp"
#include "command/groupcommand.hpp"
#include "command/removeitemcommand.hpp"
#include "command/selectcommand.hpp"
#include "command/ungroupcommand.hpp"
#include "common/constants.hpp"
#include "components/propertybar.hpp"
#include "components/toolbar.hpp"
#include "context/applicationcontext.hpp"
#include "context/coordinatetransformer.hpp"
#include "context/renderingcontext.hpp"
#include "context/selectioncontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "data-structures/cachegrid.hpp"
#include "data-structures/quadtree.hpp"
#include "jobs/loadjobutil.hpp"
#include "jobs/saveasjob.hpp"
#include "serializer/serializerutils.hpp"
using namespace Qt::Literals::StringLiterals;
ActionManager::ActionManager(ApplicationContext *context)
    : QObject(context)
    , m_context{context}
{
}

void ActionManager::undo()
{
    m_context->spatialContext()->commandHistory()->undo();
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::redo()
{
    m_context->spatialContext()->commandHistory()->redo();
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::zoomIn()
{
    m_context->renderingContext()->updateZoomFactor(1);
}

void ActionManager::zoomOut()
{
    m_context->renderingContext()->updateZoomFactor(-1);
}

void ActionManager::switchToTool(Tool::Type type)
{
    m_context->uiContext()->toolBar()->changeTool(type);
}

void ActionManager::alignItems([[maybe_unused]] ActionManager::AlignType type)
{
}

void ActionManager::switchToMoveTool()
{
    m_context->uiContext()->toolBar()->changeTool(Tool::Type::Move);
}

void ActionManager::groupItems()
{
    auto &selectedItems{m_context->selectionContext()->selectedItems()};
    if (selectedItems.size() <= 1)
        return;

    QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<GroupCommand>(items));
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::ungroupItems()
{
    auto &selectedItems{m_context->selectionContext()->selectedItems()};
    if (selectedItems.empty())
        return;

    QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<UngroupCommand>(items));
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::deleteSelection()
{
    auto &selectedItems{m_context->selectionContext()->selectedItems()};
    auto commandHistory{m_context->spatialContext()->commandHistory()};

    QList<std::shared_ptr<Item>> items{selectedItems.begin(), selectedItems.end()};
    commandHistory->insert(std::make_shared<RemoveItemCommand>(items));

    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();

    QList<std::shared_ptr<Item>> selectedItemsVector{selectedItems.begin(), selectedItems.end()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<DeselectCommand>(selectedItemsVector));
}

void ActionManager::selectAll()
{
    switchToTool(Tool::Type::Selection);

    auto allItems{m_context->spatialContext()->quadtree().getAllItems()};
    m_context->spatialContext()->commandHistory()->insert(std::make_shared<SelectCommand>(allItems));

    m_context->uiContext()->propertyBar()->updateToolProperties();
    m_context->renderingContext()->markForRender();
    m_context->renderingContext()->markForUpdate();
}

void ActionManager::saveToFile()
{
    const QDir homeDir{QDir::home()};
    QString text = QObject::tr("Untitled.%1").arg(Common::drawyFileExt);
    const QString defaultFilePath = homeDir.filePath(text);
    text = QObject::tr("Drawy (*.%1)").arg(Common::drawyFileExt);
    const QString fileName{QFileDialog::getSaveFileName(nullptr, QObject::tr("Save File"), defaultFilePath, text)};
    if (fileName.isEmpty()) {
        return;
    }
    auto job = new SaveAsJob(this);
    const SaveAsJob::SaveAsInfo info{
        .filePath = fileName,
        .offsetPos = m_context->spatialContext()->offsetPos(),
        .zoomFactor = m_context->renderingContext()->zoomFactor(),
        .items = m_context->spatialContext()->quadtree().getAllItems(),
    };
    job->setSaveAsInfo(info);
    connect(job, &SaveAsJob::saveFileDone, this, [fileName](const QJsonObject &obj) {
        SerializerUtils::saveInFile(obj, fileName);
        qDebug() << " save done ";
    });
    job->start();
}

void ActionManager::loadFromFile()
{
    const QString filter = QObject::tr("Drawy (*.%1)").arg(Common::drawyFileExt);

    // ask for file (handle cancel)
    const QDir homeDir{QDir::home()};
    const QString fileName = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open File"), homeDir.path(), filter);
    if (fileName.isEmpty())
        return;
    loadFile(fileName);
}

void ActionManager::loadFile(const QString &fileName)
{
    auto job = new LoadJob(this);
    job->setFileName(fileName);
    m_context->setCurrentFileName(fileName);
    connect(job, &LoadJob::loadDone, this, &ActionManager::slotLoadDone);
    job->start();
}

void ActionManager::slotLoadDone(const LoadJob::LoadInfo &info)
{
    LoadJobUtil::loadFile(info);
}
#include "moc_actionmanager.cpp"
