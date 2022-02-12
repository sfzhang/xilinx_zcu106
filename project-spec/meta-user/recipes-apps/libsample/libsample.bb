#
# This file is the libsample recipe.
#

SUMMARY = "Simple libsample application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://libsample.c \
       file://libsample.h \
	   file://Makefile \
		  "

S = "${WORKDIR}"

PACKAGE_ARCH = "${MACHINE_ARCH}"
PROVIDES = "libsample"
TARGET_CC_ARCH = "${LDFLAGS}"

DEBUG_FLAGS = "-g -O0"

# Specifies to build packages with debugging information
DEBUG_BUILD = "1"

# Do not remove debug symbols
INHIBIT_PACKAGE_STRIP = "1"

# OPTIONAL: Do not split debug symbols in a separate file
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

do_install() {
	     install -d ${D}${libdir}
         install -d ${D}${includedir}
	     oe_libinstall -so libsample ${D}${libdir}
         install -d -m 0755 ${D}${includedir}/sample
         install -m 0644 ${S}/*.h ${D}${includedir}/sample
}

FILES_${PN} = "${libdir}/*.so.* ${includedir}/*"
FILES_${PN}-dev = "${libdir}/*.so"
