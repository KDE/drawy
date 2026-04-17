# SPDX-FileCopyrightText: Prayag Jain <prayagjain2@gmail.com>
# SPDX-License-Identifier: GPL-3.0-or-later

function get_files
{
    echo application-x-drawy.xml
}

function po_for_file
{
    case "$1" in
       application-x-drawy.xml)
           echo drawy_xml_mimetypes.po
       ;;
    esac
}

function tags_for_file
{
    case "$1" in
       application-x-drawy.xml)
           echo comment
       ;;
    esac
}
