// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QCommandLineParser>

#include "window/window.hpp"

int main(int argc, char *argv[]) {
    QApplication a{argc, argv};
    QApplication::setStyle("Fusion");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();    
    parser.setApplicationDescription(QObject::tr("Your handy, infinite, brainstorming tool!"));

    parser.process(a);

    QTranslator translator{};
    const QStringList uiLanguages{QLocale::system().uiLanguages()};
    for (const QString &locale : uiLanguages) {
        const QString baseName{"drawy_" + QLocale(locale).name()};
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w{};
    w.show();
    return a.exec();
}
