// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KCrash>
#include <QCommandLineParser>

#ifdef Q_OS_ANDROID
#include <QGuiApplication>
#else
#include <QApplication>
#endif


#include <KIconTheme>
#include <KLocalizedString>
#include <KStyleManager>
#include <QTimer>

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

#if HAVE_KUSERFEEDBACK
#include "userfeedback/drawyuserfeedbackprovider.hpp"
#endif

using namespace Qt::Literals::StringLiterals;

#ifdef Q_OS_ANDROID
Q_DECL_EXPORT
#endif
int main(int argc, char *argv[])
{
    KIconTheme::initTheme();

#ifdef Q_OS_ANDROID
    QGuiApplication a(argc, argv);
#else
    QApplication a(argc, argv);
#endif

    a.setDesktopFileName(u"org.kde.drawy"_s);
    a.setWindowIcon(QIcon(u":/drawy/drawy.svg"_s));

    KStyleManager::initStyle();
    KLocalizedString::setApplicationDomain("drawy"_ba);

#if HAVE_WHATSNEWSNGSUPPORT
    KAboutData aboutData = KAboutData::fromAppStreamForApplication();
    aboutData.setCopyrightStatement(i18nc("Copyright text, keep the © symbol and the en dash for the year range", "© 2025–%1 Drawy authors", u"2026"_s));
    aboutData.setVersion(DRAWY_VERSION);
    aboutData.setComponentName(u"drawy"_s);
#else
    KAboutData aboutData = AboutData();
#endif
    aboutData.addAuthor(i18nc("@info:credit", "Prayag Jain"), i18n("Maintainer"), u"prayagjain2@gmail.com"_s);
    aboutData.addAuthor(i18nc("@info:credit", "Laurent Montel"), i18n("Developer"), u"montel@kde.org"_s);
    aboutData.addAuthor(i18nc("@info:credit", "Abdelhadi Wael"), i18n("GSoc 2026 developper (Improve text component)"), u"waelhadi380@gmail.com"_s);
    aboutData.addAuthor(i18nc("@info:credit", "Nikolay Kochulin"), i18n("Bug Fixing. Import/Export Files"), u"basiqueevangelist@yandex.ru"_s);

    KCrash::initialize();

    QCoreApplication::setApplicationVersion(QString::fromUtf8(DRAWY_VERSION));

    QCommandLineParser parser;
    aboutData.setupCommandLine(&parser);
    const DrawyCommandLineParser commandLineParser(&parser);

    parser.addPositionalArgument(QStringLiteral("file"), i18nc("@info:shell", "An optional argument 'file' "), QStringLiteral("[file]"));
    parser.setApplicationDescription(i18n("Your handy, infinite brainstorming tool!"));

    parser.process(a);
    aboutData.processCommandLine(&parser);

#if HAVE_KUSERFEEDBACK
    if (parser.isSet(commandLineParser.optionParserFromEnum(DrawyCommandLineParser::OptionParser::FeedBack))) {
        auto userFeedback = new DrawyUserFeedbackProvider;
        QTextStream(stdout) << userFeedback->describeDataSources() << '\n';
        delete userFeedback;
        return 0;
    }
#endif

    KAboutData::setApplicationData(aboutData);

    MainWindow w
#if HAVE_WHATSNEWSNGSUPPORT
        (aboutData.releases())
#endif
            ;
    if (parser.isSet(commandLineParser.optionParserFromEnum(DrawyCommandLineParser::OptionParser::FullScreen))) {
        w.viewFullScreen(true);
    }
    if (parser.isSet(commandLineParser.optionParserFromEnum(DrawyCommandLineParser::OptionParser::Debug))) {
        w.activeDebug();
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

    if (parser.isSet(DrawyCommandLineParser::optionParserFromEnum(DrawyCommandLineParser::OptionParser::SelfTest))) {
        QTimer::singleShot(std::chrono::milliseconds(250), &a, &QCoreApplication::quit);
    }
    return a.exec();
}
