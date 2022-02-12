#
# This file is the sampleapp recipe.
#

SUMMARY = "Simple sampleapp application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://sampleapp.c \
	   file://Makefile \
		  "

S = "${WORKDIR}"

DEPENDS += " libsample"

DEBUG_FLAGS = "-g -O0"

# Specifies to build packages with debugging information
DEBUG_BUILD = "1"

# Do not remove debug symbols
INHIBIT_PACKAGE_STRIP = "1"

# OPTIONAL: Do not split debug symbols in a separate file
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

do_compile() {
	     oe_runmake
}

do_install() {
	     install -d ${D}${bindir}
	     install -m 0755 sampleapp ${D}${bindir}
}

FILES_${PN} += "sampleapp"
