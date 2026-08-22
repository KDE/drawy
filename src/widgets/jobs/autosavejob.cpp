/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "autosavejob.hpp"
#include "context/applicationcontext.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/quadtree.hpp"
#include "drawy_autosave_debug.h"
#include "drawyglobalconfig.h"
#include "jobs/saveasjob.hpp"
#include "serializer/serializerutils.hpp"
#include <QTimer>

AutoSaveJob::AutoSaveJob(ApplicationContext *context, QObject *parent)
    : QObject{parent}
    , mApplicationContext(context)
{
}

AutoSaveJob::~AutoSaveJob() = default;

void AutoSaveJob::start() const
{
    constexpr int mseconds = 60 * 1000;
    QTimer::singleShot(DrawyGlobalConfig::self()->delay() * mseconds, this, &AutoSaveJob::saveFile);
}

void AutoSaveJob::saveFile()
{
    const QString fileName = DrawyGlobalConfig::self()->path();
    auto saveAsJob = new SaveAsJob(mApplicationContext, this);

    const SaveAsJob::SaveAsInfo info{.filePath = fileName,
                                     .offsetPos = mApplicationContext->spatialContext()->offsetPos(),
                                     .zoomFactor = mApplicationContext->renderingContext()->zoomFactor(),
                                     .items = mApplicationContext->spatialContext()->quadtree().getAllItems(),
                                     .isAutoSave = true};

    saveAsJob->setSaveAsInfo(info);
    connect(saveAsJob, &SaveAsJob::saveFileDone, this, [fileName, this](const QJsonObject &obj) {
        SerializerUtils::saveInFile(obj, fileName);
        qCDebug(DRAWY_AUTOSAVE_LOG) << "Autosave file done: " << fileName;

        // Restart timer
        start();
    });

    saveAsJob->start();
}

#include "moc_autosavejob.cpp"
