// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "applicationcontext.hpp"

#include "command/commandhistory.hpp"
#include "common/constants.hpp"
#include "coordinatetransformer.hpp"
#include "drawy_debug.h"
#include "mime/mimemanager.hpp"
#include "renderingcontext.hpp"
#include "selectioncontext.hpp"
#include "spatialcontext.hpp"
#include "uicontext.hpp"

ApplicationContext::ApplicationContext(QWidget *parent)
    : QObject{parent}
    , m_currentFileName{Common::unsavedFileName}
    , m_parentWidget{parent}
    , m_renderingContext(new RenderingContext(this))
    , m_spatialContext(new SpatialContext(this))
    , m_uiContext(new UIContext(this))
    , m_selectionContext(new SelectionContext(this))
    , m_mimeManager(new MimeManager(this))
{
    m_spatialContext->coordinateTransformer().setCoordinateTransformer();

    connect(m_spatialContext->commandHistory(), &CommandHistory::undoRedoChanged, this, [this]() -> void {
        m_currentFileModified = true;
    });
    m_uiContext->initializeUIContext();
}

ApplicationContext::~ApplicationContext()
{
    qCDebug(DRAWY_LOG) << "Object deleted: ApplicationContext";
}

QString ApplicationContext::currentFileName() const
{
    return m_currentFileName;
}

bool ApplicationContext::fileNeedsName() const
{
    return !m_fileHasName;
}

void ApplicationContext::setCurrentFileName(const QString &newCurrentFileName)
{
    m_currentFileName = newCurrentFileName;
    m_fileHasName = true;
}

bool ApplicationContext::currentFileModified() const
{
    return m_currentFileModified;
}

void ApplicationContext::setCurrentFileModified(bool value)
{
    m_currentFileModified = value;
}

bool ApplicationContext::debug() const
{
    return mDebug;
}

void ApplicationContext::setDebug(bool newDebug)
{
    mDebug = newDebug;
}

QWidget *ApplicationContext::parentWidget() const
{
    return m_parentWidget;
}

RenderingContext *ApplicationContext::renderingContext() const
{
    return m_renderingContext;
}

SpatialContext *ApplicationContext::spatialContext() const
{
    return m_spatialContext;
}

UIContext *ApplicationContext::uiContext() const
{
    return m_uiContext;
}

SelectionContext *ApplicationContext::selectionContext() const
{
    return m_selectionContext;
}

MimeManager *ApplicationContext::mimeManager() const
{
    return m_mimeManager;
}

void ApplicationContext::reset()
{
    selectionContext()->reset();
    uiContext()->reset();
    selectionContext()->reset();
    spatialContext()->reset();
    renderingContext()->reset();

    m_currentFileName = Common::unsavedFileName;
    m_currentFileModified = false;
    m_fileHasName = false;
}

#include "moc_applicationcontext.cpp"
