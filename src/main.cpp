// SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <KCrash>
#include <QApplication>
#include <QCommandLineParser>

#include "window/window.hpp"
using namespace Qt::Literals::StringLiterals;
int main(int argc, char *argv[])
{
    QApplication a{argc, argv};
    QApplication::setStyle(u"Fusion"_s);
    KCrash::initialize();

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription(QObject::tr("Your handy, infinite, brainstorming tool!"));

    parser.process(a);

    MainWindow w{};
    w.show();
    return a.exec();
}
