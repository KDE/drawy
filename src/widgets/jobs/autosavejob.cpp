/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "autosavejob.hpp"
#include "autosavejobutil.hpp"
#include "context/applicationcontext.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "data-structures/quadtree.hpp"
#include "drawy_autosave_debug.h"
#include "drawyglobalconfig.h"
#include "jobs/saveasjob.hpp"
#include "serializer/serializerutils.hpp"
#include <QTimer>

// #define TEST_TIMER

AutoSaveJob::AutoSaveJob(QObject *parent)
    : QObject{parent}
{
}

AutoSaveJob::~AutoSaveJob() = default;

void AutoSaveJob::start()
{
    constexpr int mseconds = 60 * 1000;
    QTimer::singleShot(DrawyGlobalConfig::self()->delay() * mseconds, this, &AutoSaveJob::saveFile);
}

void AutoSaveJob::saveFile()
{
    const QString fileName = DrawyGlobalConfig::self()->path();
    auto saveAsJob = new SaveAsJob(this);
    auto context{ApplicationContext::instance()};

    const SaveAsJob::SaveAsInfo info{.filePath = fileName,
                                     .offsetPos = context->spatialContext()->offsetPos(),
                                     .zoomFactor = context->renderingContext()->zoomFactor(),
                                     .items = context->spatialContext()->quadtree().getAllItems(),
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
