// SPDX-FileCopyrightText: 2026 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "header.hpp"
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
Header::Header(QWidget *parent)
    : QWidget{parent}
    , m_layout{new QHBoxLayout(this)}
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void Header::initialize()
{
    // Toolbar
    auto context = ApplicationContext::instance();
    auto toolBar = context->uiContext()->toolBar();

    // menu button
    Frame *menuFrame = new Frame{this};
    QHBoxLayout *menuFrameLayout = new QHBoxLayout{menuFrame};

    int iconWidth{style()->pixelMetric(QStyle::PM_ToolBarIconSize)};
    QSize iconSize{iconWidth, iconWidth};

    QToolButton *menuButton = new QToolButton{menuFrame};
    menuButton->setIcon(QIcon::fromTheme(u"application-menu"_s));
    menuButton->setAutoRaise(true);
    menuButton->setIconSize(iconSize);
    menuButton->setToolTip(tr("Menu"));

    // separator
    QFrame *vLine{new QFrame{menuFrame}};
    vLine->setFrameShape(QFrame::VLine);
    vLine->setFixedHeight(menuButton->height());

    // save controls
    QToolButton *saveButton = new QToolButton{menuFrame};
    saveButton->setIcon(QIcon::fromTheme(u"document-save"_s));
    saveButton->setAutoRaise(true);
    saveButton->setIconSize(iconSize);
    saveButton->setToolTip(tr("Save"));

    connect(saveButton, &QToolButton::clicked, this, [context]() {
        auto actionManager{context->uiContext()->actionManager()};
        actionManager->saveToFile();
    });

    QToolButton *loadButton = new QToolButton{menuFrame};
    loadButton->setIcon(QIcon::fromTheme(u"document-open"_s));
    loadButton->setAutoRaise(true);
    loadButton->setIconSize(iconSize);
    loadButton->setToolTip(tr("Load"));

    connect(loadButton, &QToolButton::clicked, this, [context]() {
        auto actionManager{context->uiContext()->actionManager()};
        actionManager->loadFromFile();
    });

    menuFrameLayout->addWidget(menuButton);
    menuFrameLayout->addWidget(vLine);
    menuFrameLayout->addWidget(saveButton);
    menuFrameLayout->addWidget(loadButton);

    // undo/redo controls
    Frame *undoRedoFrame = new Frame{this};
    QHBoxLayout *undoRedoFrameLayout = new QHBoxLayout{undoRedoFrame};

    QToolButton *undoButton = new QToolButton{undoRedoFrame};
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

    QToolButton *redoButton = new QToolButton{undoRedoFrame};
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

    // center widgets
    m_centerWidget = new QWidget{this};
    QHBoxLayout *centerWidgetLayout = new QHBoxLayout{m_centerWidget};

    m_rightSpaceBalancingWidget = new QWidget{this};
    m_rightSpaceBalancingWidget->setFixedWidth(menuFrame->width());

    centerWidgetLayout->setContentsMargins(0, 0, 0, 0);
    centerWidgetLayout->addWidget(toolBar);
    centerWidgetLayout->addWidget(undoRedoFrame);

    m_layout->addWidget(menuFrame, 0, Qt::AlignLeft);
    m_layout->addWidget(m_centerWidget, 1, Qt::AlignCenter);
    m_layout->addWidget(m_rightSpaceBalancingWidget, 0, Qt::AlignRight);

    m_isInitialized = true;
}

void Header::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (!m_isInitialized) {
        return;
    }

    const bool isCompact{width() < 768};
    if (isCompact) {
        m_layout->setAlignment(m_centerWidget, Qt::AlignRight);
        m_rightSpaceBalancingWidget->hide();
    } else {
        m_layout->setAlignment(m_centerWidget, Qt::AlignCenter);
        m_rightSpaceBalancingWidget->show();
    }
}

bool Header::isInitialized() const
{
    return m_isInitialized;
}
