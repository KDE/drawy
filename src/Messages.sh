#! /usr/bin/env bash
$EXTRACT_TR_STRINGS `find -name \*.cpp -o -name \*.hpp` -o $podir/drawy_qt.pot
