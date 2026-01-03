// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "applicationcontext.hpp"

#include "coordinatetransformer.hpp"
#include "drawy_debug.h"
#include "renderingcontext.hpp"
#include "selectioncontext.hpp"
#include "spatialcontext.hpp"
#include "uicontext.hpp"

ApplicationContext *ApplicationContext::m_instance = nullptr;

ApplicationContext::ApplicationContext(QWidget *parent)
    : QObject{parent}
    , m_parentWidget{parent}
    , m_renderingContext(new RenderingContext(this))
    , m_spatialContext(new SpatialContext(this))
    , m_uiContext(new UIContext(this))
    , m_selectionContext(new SelectionContext(this))
{
}

ApplicationContext::~ApplicationContext()
{
    qCDebug(DRAWY_LOG) << "Object deleted: ApplicationContext";
}

void ApplicationContext::setContexts()
{
    m_renderingContext->setRenderingContext();
    m_uiContext->setUIContext();
    m_spatialContext->setSpatialContext();
    m_spatialContext->coordinateTransformer()->setCoordinateTransformer();
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

void ApplicationContext::reset()
{
    selectionContext()->reset();
    uiContext()->reset();
    selectionContext()->reset();
    spatialContext()->reset();
    renderingContext()->reset();
}
