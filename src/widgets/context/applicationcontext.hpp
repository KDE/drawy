// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QWidget>
class RenderingContext;
class SpatialContext;
class UIContext;
class SelectionContext;

/**
 * @note: This class was made a Singleton recently, so in many places it is still accessed by
 * passing its pointer instead of using the `instance` method.
 */
class ApplicationContext : public QObject
{
public:
    ~ApplicationContext() override;

    static ApplicationContext *instance(QWidget *parent = nullptr);

    void setContexts();

    [[nodiscard]] QWidget *parentWidget() const;

    [[nodiscard]] RenderingContext *renderingContext() const;
    [[nodiscard]] SpatialContext *spatialContext() const;
    [[nodiscard]] UIContext *uiContext() const;
    [[nodiscard]] SelectionContext *selectionContext() const;

    void reset(); // resets the canvas to a blank state

private:
    explicit ApplicationContext(QWidget *parent = nullptr);

    ApplicationContext(const ApplicationContext &) = delete;
    ApplicationContext(ApplicationContext *) = delete;

    QWidget *const m_parentWidget;

    RenderingContext *const m_renderingContext;
    SpatialContext *const m_spatialContext;
    UIContext *const m_uiContext;
    SelectionContext *const m_selectionContext;

    static ApplicationContext *m_instance;
};
