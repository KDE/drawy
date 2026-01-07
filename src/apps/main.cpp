// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KCrash>
#include <QApplication>
#include <QCommandLineParser>

#include "drawycommandlineparser.h"
#include "window/window.hpp"
using namespace Qt::Literals::StringLiterals;
int main(int argc, char *argv[])
{
    QApplication a{argc, argv};
    QApplication::setStyle(u"Fusion"_s);
    a.setWindowIcon(QIcon(u":/drawy/drawy.svg"_s));

    KCrash::initialize();

    QCommandLineParser parser;
    const DrawyCommandLineParser commandLineParser(&parser);
    parser.addPositionalArgument(QStringLiteral("file"), QObject::tr("An optional argument 'file' "), QStringLiteral("[file]"));

    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription(QObject::tr("Your handy, infinite, brainstorming tool!"));

    parser.process(a);

    MainWindow w{};
    if (parser.isSet(commandLineParser.optionParserFromEnum(DrawyCommandLineParser::OptionParser::FullScreen))) {
        w.viewFullScreen(true);
    }
    w.show();
    return a.exec();
}
