/*
 * SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once
#include "libdrawywidgets_private_export.h"
#include <QWidget>
class AutoSaveWidget;
class CanvasColorWidget;
class DefaultColorsWidget;

class LIBDRAWYWIDGETS_TESTS_EXPORT ConfigureGeneralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigureGeneralWidget(QWidget *parent = nullptr);
    ~ConfigureGeneralWidget() override;

    void save();
    void load();
    void restoreToDefaults();

private:
    AutoSaveWidget *const m_autoSaveWidget;
    CanvasColorWidget *const m_canvasColorWidget;
    DefaultColorsWidget *const m_defaultColorsWidget;
};
