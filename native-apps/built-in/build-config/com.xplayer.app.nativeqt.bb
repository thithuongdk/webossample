# Copyright (c) 2020-2022 LG Electronics, Inc.

SUMMARY = "X Player App"
SECTION = "webos/apps"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

WEBOS_VERSION = "1.0.0-20240701-215338"
PR = "r0"

DEPENDS = " \
        luna-service2 \
        glib-2.0 \
        libpbnjson \
        qtbase \
        qtdeclarative \
        umediaserver \
        "

inherit webos_submissions
inherit webos_qmake6
inherit webos_app
inherit webos_pkgconfig

OE_QMAKE_PATH_HEADERS = "${OE_QMAKE_PATH_QT_HEADERS}"

FILES:${PN} += "${webos_applicationsdir}"
