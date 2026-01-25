// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "jobs/loadjob.hpp"
#include "tools/tool.hpp"
#include <QObject>
class ApplicationContext;
class ActionManager : public QObject
{
    Q_OBJECT
public:
    explicit ActionManager(ApplicationContext *context);

    void zoomIn();
    void zoomOut();
    void undo();
    void redo();
    void switchToMoveTool();
    void selectAll();
    void deleteSelection();
    void groupItems();
    void ungroupItems();
    void saveToFile();
    void loadFromFile();

    void loadFile(const QString &fileName);

    void switchToTool(Tool::Type type);

private:
    void slotLoadDone(const LoadJob::LoadInfo &info);
    ApplicationContext *const m_context;
};
