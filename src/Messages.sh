#! /usr/bin/env bash

# SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
# SPDX-FileCopyrightText: 2026 Laurent Montel <montel@kde.org>
#
# SPDX-License-Identifier: GPL-3.0-or-later

$XGETTEXT `find -name \*.cpp -o -name \*.hpp` -o $podir/drawy.pot

$EXTRACTRC `find -name \*.xml` >> rc.cpp
