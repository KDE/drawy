#! /usr/bin/env bash

# SPDX-FileCopyrightText: 2025 Prayag Jain <prayagjain2@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later

$EXTRACT_TR_STRINGS `find -name \*.cpp -o -name \*.hpp` -o $podir/drawy_qt.pot
