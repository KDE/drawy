// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
// SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "libdrawywidgets_export.h"
#include "tools/tool.hpp"

#include <QWidget>
class LIBDRAWYWIDGETS_EXPORT MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void viewFullScreen(bool fullScreen);

    void loadFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    LIBDRAWYWIDGETS_NO_EXPORT void contextMenuRequested(const QPoint &pos);
    LIBDRAWYWIDGETS_NO_EXPORT void loadCustomFonts();
    LIBDRAWYWIDGETS_NO_EXPORT void configureSettings();
    LIBDRAWYWIDGETS_NO_EXPORT void setupAction();
    LIBDRAWYWIDGETS_NO_EXPORT void createToolAction(const QString &actionName, const QString &name, Tool::Type type, const QList<QKeySequence> &key);
    LIBDRAWYWIDGETS_NO_EXPORT QAction *createAction(const QString &actionName, const QString &title, const QList<QKeySequence> &keys);
    bool m_forceClose = false;
    QAction *mFullScreenAction = nullptr;
    QAction *mConfigureSettingsAction = nullptr;
    QAction *mQuitAction = nullptr;
    QAction *mSaveAction = nullptr;
    QAction *mUndoAction = nullptr;
    QAction *mRedoAction = nullptr;
    QAction *mZoomInAction = nullptr;
    QAction *mZoomOutAction = nullptr;
    QAction *mLoadAction = nullptr;
    QAction *mSelectAllAction = nullptr;
};
