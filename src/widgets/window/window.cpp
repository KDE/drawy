// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "window.hpp"

#include <QButtonGroup>
#include <QFile>
#include <QFileDialog>
#include <QFontDatabase>
#include <QShortcut>

#include "boardlayout.hpp"
#include "canvas/canvas.hpp"
#include "common/constants.hpp"
#include "components/actionbar.hpp"
#include "components/propertybar.hpp"
#include "components/toolbar.hpp"
#include "context/applicationcontext.hpp"
#include "context/renderingcontext.hpp"
#include "context/spatialcontext.hpp"
#include "context/uicontext.hpp"
#include "controller/controller.hpp"
#include "data-structures/quadtree.hpp"
#include "drawy_debug.h"
#include "jobs/autosavejob.hpp"
#include "jobs/autosavejobutil.hpp"
#include "jobs/restoreautosavejob.hpp"
#include "jobs/saveasjob.hpp"
#include "keybindings/actionmanager.hpp"
#include "serializer/serializerutils.hpp"
#include <KMessageBox>
#include <qdir.h>
using namespace Qt::Literals::StringLiterals;
MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    applyCustomStyles();

    BoardLayout *layout{new BoardLayout(this)};
    Controller *controller{new Controller(this)};
    ApplicationContext *context{ApplicationContext::instance()};

    auto renderingContext{context->renderingContext()};
    UIContext *uiContext{context->uiContext()};

    renderingContext->canvas()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layout->setMargins(10);
    layout->setLeftWidget(uiContext->propertyBar());
    layout->setTopWidget(uiContext->toolBar());
    layout->setBottomWidget(uiContext->actionBar());
    layout->setCentralWidget(renderingContext->canvas());

    connect(renderingContext->canvas(), &Canvas::mousePressed, controller, &Controller::mousePressed);
    connect(renderingContext->canvas(), &Canvas::mouseMoved, controller, &Controller::mouseMoved);
    connect(renderingContext->canvas(), &Canvas::mouseReleased, controller, &Controller::mouseReleased);
    connect(renderingContext->canvas(), &Canvas::keyPressed, controller, &Controller::keyPressed);
    connect(renderingContext->canvas(), &Canvas::keyReleased, controller, &Controller::keyReleased);

    connect(renderingContext->canvas(), &Canvas::tablet, controller, &Controller::tablet);
    connect(renderingContext->canvas(), &Canvas::wheel, controller, &Controller::wheel);
    connect(renderingContext->canvas(), &Canvas::leave, controller, &Controller::leave);

    applyCustomStyles();
    auto restoreAutoSaveJob = new RestoreAutoSaveJob(context, this);
    restoreAutoSaveJob->setParentWidget(this);
    connect(restoreAutoSaveJob, &RestoreAutoSaveJob::restoreDone, this, [this, context]() {
        AutoSaveJobUtil::createAutoSaveStandardPath();
        // Autosave
        auto autoSaveJob = new AutoSaveJob(context, this);
        autoSaveJob->start();
    });
    restoreAutoSaveJob->start();
}

MainWindow::~MainWindow() = default;

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (!m_forceClose) {
        if (!ApplicationContext::instance()->spatialContext()->quadtree().getAllItems().isEmpty()) {
            const int choice = KMessageBox::questionTwoActionsCancel(this,
                                                                     tr("Do you want to save file?"),
                                                                     tr("Close"),
                                                                     KGuiItem(tr("Save And Close")),
                                                                     KGuiItem(tr("Ignore")),
                                                                     KStandardGuiItem::cancel());
            if (choice == KMessageBox::Cancel) {
                e->ignore();
            } else if (choice == KMessageBox::ButtonCode::PrimaryAction) {
                const QDir homeDir{QDir::home()};
                QString text = QObject::tr("Untitled.%1").arg(Common::drawyFileExt);
                const QString defaultFilePath = homeDir.filePath(text);
                text = QObject::tr("Drawy (*.%1)").arg(Common::drawyFileExt);
                const QString fileName{QFileDialog::getSaveFileName(nullptr, QObject::tr("Save File"), defaultFilePath, text)};
                if (fileName.isEmpty()) {
                    e->ignore();
                    return;
                }
                m_forceClose = true;
                auto job = new SaveAsJob(this);
                ApplicationContext *context{ApplicationContext::instance()};
                const SaveAsJob::SaveAsInfo info{
                    .filePath = fileName,
                    .offsetPos = context->spatialContext()->offsetPos(),
                    .zoomFactor = context->renderingContext()->zoomFactor(),
                    .items = context->spatialContext()->quadtree().getAllItems(),
                };
                job->setSaveAsInfo(info);
                connect(job, &SaveAsJob::saveFileDone, this, [fileName, this](const QJsonObject &obj) {
                    SerializerUtils::saveInFile(obj, fileName);
                    qDebug() << " save done ";
                    AutoSaveJobUtil::removeAutoSaveFile();
                    close();
                });
                job->start();
            } else {
                e->accept();
            }
            return;
        } else {
            e->accept();
        }
    } else {
        e->accept();
    }
}

void MainWindow::applyCustomStyles()
{
    QFile file(u":/styles/style.qss"_s);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        const QString qss = stream.readAll();
        setStyleSheet(qss);
    } else {
        qCWarning(DRAWY_LOG) << "Failed to load stylesheet.";
    }

    int fontID = QFontDatabase::addApplicationFont(u":/fonts/FuzzyBubbles.ttf"_s);
    if (fontID == -1) {
        qCWarning(DRAWY_LOG) << "Failed to load font: FuzzyBubbles";
    }

    fontID = QFontDatabase::addApplicationFont(u":/fonts/Inter.ttf"_s);
    if (fontID == -1) {
        qCWarning(DRAWY_LOG) << "Failed to load font: Inter";
    }
}

void MainWindow::viewFullScreen(bool fullScreen)
{
    if (fullScreen) {
        setWindowState(windowState() | Qt::WindowFullScreen);
    } else {
        setWindowState(windowState() & ~Qt::WindowFullScreen);
    }
}

void MainWindow::loadFile(const QString &fileName)
{
    auto actionManager{ApplicationContext::instance()->uiContext()->actionManager()};
    actionManager->loadFile(fileName);
}

#include "moc_window.cpp"
