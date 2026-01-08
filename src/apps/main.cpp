// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KAboutData>
#include <KCrash>
#include <QApplication>
#include <QCommandLineParser>

#include "config-drawy.hpp"
#include "drawycommandlineparser.h"
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
    QApplication a{argc, argv};
    QApplication::setStyle(u"Fusion"_s);
    a.setWindowIcon(QIcon(u":/drawy/drawy.svg"_s));

    KAboutData aboutData(u"drawy"_s,
                         QObject::tr("drawy"),
                         QStringLiteral(DRAWY_VERSION),
                         QObject::tr("Your handy, infinite, brainstorming tool!"),
                         KAboutLicense::GPL_V3,
                         QObject::tr("(c) 2025-%1 drawy authors").arg(u"2026"_s));
    aboutData.addAuthor(QObject::tr("Prayag Jain"), QObject::tr("Maintainer"), u"prayagjain2@gmail.com"_s);
    aboutData.addAuthor(QObject::tr("Laurent Montel"), QObject::tr("Developer"), u"montel@kde.org"_s);

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
