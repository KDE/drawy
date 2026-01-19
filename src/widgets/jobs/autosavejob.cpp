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

AutoSaveJob::AutoSaveJob(ApplicationContext *context, QObject *parent)
    : QObject{parent}
    , m_context{context}
{
}

AutoSaveJob::~AutoSaveJob() = default;

void AutoSaveJob::start()
{
#ifdef TEST_TIMER
    constexpr int mseconds = 10 * 1000;
#else
    constexpr int mseconds = 60 * 1000;
#endif
    QTimer::singleShot(DrawyGlobalConfig::self()->delay() * mseconds, this, &AutoSaveJob::saveFile);
}

void AutoSaveJob::saveFile()
{
    const QString fileName = AutoSaveJobUtil::temporaryFileName();
    auto saveAsJob = new SaveAsJob(this);
    const SaveAsJob::SaveAsInfo info{
        .filePath = fileName,
        .offsetPos = m_context->spatialContext()->offsetPos(),
        .zoomFactor = m_context->renderingContext()->zoomFactor(),
        .items = m_context->spatialContext()->quadtree().getAllItems(),
    };
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
