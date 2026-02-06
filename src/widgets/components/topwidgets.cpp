// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "topwidgets.hpp"
#include "command/commandhistory.hpp"
#include "components/frame.hpp"
#include "components/toolbar.hpp"
#include "context/applicationcontext.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "keybindings/actionmanager.hpp"
#include <QHBoxLayout>
#include <QStyle>
#include <QToolButton>

using namespace Qt::StringLiterals;
TopWidgets::TopWidgets(ApplicationContext *context, QWidget *parent)
    : QWidget{parent}
    , m_layout{new QHBoxLayout(this)}
    , m_context(context)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_layout->setContentsMargins(0, 0, 0, 0);
}

void TopWidgets::initialize()
{
    // Toolbar
    auto toolBar{m_context->uiContext()->toolBar()};
    auto actionManager{m_context->uiContext()->actionManager()};

    const int iconWidth{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    const QSize iconSize{iconWidth, iconWidth};

    // undo/redo controls
    auto undoRedoFrame{new Frame{this}};
    auto undoRedoFrameLayout{new QHBoxLayout{undoRedoFrame}};

    auto undoButton{new QToolButton{undoRedoFrame}};
    undoButton->setAutoRaise(true);
    undoButton->setIconSize(iconSize);
    undoButton->setDefaultAction(actionManager->action(KStandardActions::Undo));

    auto redoButton{new QToolButton{undoRedoFrame}};
    redoButton->setAutoRaise(true);
    redoButton->setIconSize(iconSize);
    redoButton->setDefaultAction(actionManager->action(KStandardActions::Redo));

    undoRedoFrameLayout->addWidget(undoButton);
    undoRedoFrameLayout->addWidget(redoButton);

    m_layout->addWidget(toolBar);
    m_layout->addWidget(undoRedoFrame);

    m_isInitialized = true;
}

bool TopWidgets::isInitialized() const
{
    return m_isInitialized;
}

#include "moc_topwidgets.cpp"
