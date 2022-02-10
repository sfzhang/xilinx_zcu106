#
# This is vcu video library recipe
#
SUMMARY = "VCU Video library"
SECTION = "PETALINUX/libs"
DESCRIPTION = "The VCU Video Library parses and configures media graph of media devices for V4L2 video pipelines."
LICENSE = "LGPLv2"
LIC_FILES_CHKSUM = "file://LICENSE.md;md5=b60ab04828851b8b8d8ad651889ac94d"

BRANCH ?= "zynqmp_vcu_trd_v2021.2"
REPO   ?= "git://github.com/Xilinx/vcu_video_lib.git;protocol=https"
SRCREV ?= "60bc097ca69f0cd6e37ff91a80eb06f3998f7c49"

BRANCHARG = "${@['nobranch=1', 'branch=${BRANCH}'][d.getVar('BRANCH', True) != '']}"
SRC_URI = "${REPO};${BRANCHARG}"

S  = "${WORKDIR}/git"

CFLAGS_prepend = "-I${S}/include/"

DEPENDS = "v4l-utils libdrm media-ctl glib-2.0 alsa-lib"

inherit pkgconfig autotools

do_install() {
    oe_runmake install DESTDIR=${D} INCLUDEDIR=${includedir}
    install -d ${D}${libdir}
    oe_libinstall -C ${WORKDIR}/build/.libs/ -so libvcu_video ${D}/${libdir}/
    install -d ${D}${libdir}/pkgconfig
    install -D -m 0644 ${S}/vcu-video-lib.pc ${D}${libdir}/pkgconfig/
}
