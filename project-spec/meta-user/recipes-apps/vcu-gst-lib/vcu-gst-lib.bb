#
# VCU GStreamer library recipe
#
SUMMARY = "VCU GStreamer library"
SECTION = "PETALINUX/libs"
DESCRIPTION = "VCU Gstreamer library constructs video pipeline. It provides API interface to application for configuration and control."
LICENSE = "LGPLv2"
LIC_FILES_CHKSUM = "file://LICENSE.md;md5=b60ab04828851b8b8d8ad651889ac94d"

BRANCH ?= "zynqmp_vcu_trd_v2021.2"
REPO   ?= "git://github.com/Xilinx/vcu_gst_lib.git;protocol=https"
SRCREV ?= "5632bf1fd540fa44a992dad97af70cc348b72161"

BRANCHARG = "${@['nobranch=1', 'branch=${BRANCH}'][d.getVar('BRANCH', True) != '']}"
SRC_URI = "${REPO};${BRANCHARG}"

S  = "${WORKDIR}/git"

CFLAGS_prepend = "-I${S}/include/"

DEPENDS = "gstreamer1.0 gstreamer1.0-plugins-base vcu-video-lib"

inherit pkgconfig autotools

do_install() {
    oe_runmake install DESTDIR=${D} INCLUDEDIR=${includedir}
    install -d ${D}${libdir}
    oe_libinstall -C ${WORKDIR}/build/.libs/ -so libvcu_gst ${D}/${libdir}/
    install -d ${D}${libdir}/pkgconfig
    install -D -m 0644 ${S}/vcu-gst-lib.pc ${D}${libdir}/pkgconfig/
}
