// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KCrash>
#include <QApplication>
#include <QCommandLineParser>

#include <KIconTheme>
#include <KStyleManager>

#include "config-drawy.hpp"
#include "context/aboutdata.hpp"
#include "drawycommandlineparser.hpp"
#include "window/window.hpp"

// signal handler for SIGINT & SIGTERM
#ifdef Q_OS_UNIX
#include <KSignalHandler>
#include <signal.h>
#include <unistd.h>
#endif

using namespace Qt::Literals::StringLiterals;
int main(int argc, char *argv[])
{
    KIconTheme::initTheme();
    QApplication a{argc, argv};
    a.setWindowIcon(QIcon(u":/drawy/drawy.svg"_s));

    KStyleManager::initStyle();
    AboutData aboutData;

    KCrash::initialize();

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    const DrawyCommandLineParser commandLineParser(&parser);
    parser.addPositionalArgument(QStringLiteral("file"), QObject::tr("An optional argument 'file' "), QStringLiteral("[file]"));

    parser.addVersionOption();
    parser.setApplicationDescription(QObject::tr("Your handy, infinite brainstorming tool!"));

    parser.process(a);
    aboutData.processCommandLine(&parser);

    MainWindow w;
    if (parser.isSet(commandLineParser.optionParserFromEnum(DrawyCommandLineParser::OptionParser::FullScreen))) {
        w.viewFullScreen(true);
    }
    const QStringList &args = parser.positionalArguments();

    if (!args.isEmpty()) {
        w.loadFile(args.constFirst());
    }

#ifdef Q_OS_UNIX
    /**
     * Set up signal handler for SIGINT and SIGTERM
     */
    KSignalHandler::self()->watchSignal(SIGINT);
    KSignalHandler::self()->watchSignal(SIGTERM);
    QObject::connect(KSignalHandler::self(), &KSignalHandler::signalReceived, &a, [&w](int signal) {
        if (signal == SIGINT || signal == SIGTERM) {
            printf("Shutting down...\n");
            w.close();
        }
    });
#endif

    w.show();
    return a.exec();
}
