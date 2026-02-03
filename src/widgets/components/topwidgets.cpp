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
#include <QStyle>
#include <QToolButton>

using namespace Qt::StringLiterals;
TopWidgets::TopWidgets(QWidget *parent)
    : QWidget{parent}
    , m_layout{new QHBoxLayout(this)}
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_layout->setContentsMargins(0, 0, 0, 0);
}

void TopWidgets::initialize()
{
    // Toolbar
    auto context{ApplicationContext::instance()};
    auto toolBar{context->uiContext()->toolBar()};

    const int iconWidth{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    const QSize iconSize{iconWidth, iconWidth};

    // undo/redo controls
    auto undoRedoFrame{new Frame{this}};
    auto undoRedoFrameLayout{new QHBoxLayout{undoRedoFrame}};

    auto undoButton{new QToolButton{undoRedoFrame}};
    undoButton->setIcon(QIcon::fromTheme(u"edit-undo"_s));
    undoButton->setAutoRaise(true);
    undoButton->setIconSize(iconSize);
    undoButton->setToolTip(tr("Undo"));
    undoButton->setDisabled(true);

    connect(undoButton, &QToolButton::clicked, this, [context] {
        context->spatialContext()->commandHistory()->undo();
        context->renderingContext()->markForRender();
        context->renderingContext()->markForUpdate();
    });

    auto redoButton{new QToolButton{undoRedoFrame}};
    redoButton->setIcon(QIcon::fromTheme(u"edit-redo"_s));
    redoButton->setAutoRaise(true);
    redoButton->setIconSize(iconSize);
    redoButton->setToolTip(tr("Redo"));
    redoButton->setDisabled(true);

    connect(redoButton, &QToolButton::clicked, this, [context] {
        context->spatialContext()->commandHistory()->redo();
        context->renderingContext()->markForRender();
        context->renderingContext()->markForUpdate();
    });

    connect(context->spatialContext()->commandHistory(), &CommandHistory::undoRedoChanged, this, [undoButton, redoButton, context]() {
        undoButton->setEnabled(context->spatialContext()->commandHistory()->hasUndo());
        redoButton->setEnabled(context->spatialContext()->commandHistory()->hasRedo());
    });

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
