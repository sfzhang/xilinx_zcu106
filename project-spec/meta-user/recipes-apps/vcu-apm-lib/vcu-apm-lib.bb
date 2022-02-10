#
# VCU APM library recipe
#
SUMMARY = "VCU APM library"
SECTION = "PETALINUX/libs"
DESCRIPTION = "The VCU AXI Performance Monitor library provides an interface to the application for measuring VCU performance numbers."
LICENSE = "LGPLv2"
LIC_FILES_CHKSUM = "file://LICENSE.md;md5=b60ab04828851b8b8d8ad651889ac94d"

BRANCH ?= "zynqmp_vcu_trd_v2021.2"
REPO   ?= "git://github.com/Xilinx/vcu_apm_lib.git;protocol=https"
SRCREV ?= "3c33dab6cbb46903f6ace87adf76eb0c914ef0a7"

BRANCHARG = "${@['nobranch=1', 'branch=${BRANCH}'][d.getVar('BRANCH', True) != '']}"
SRC_URI = "${REPO};${BRANCHARG}"

S  = "${WORKDIR}/git"

CFLAGS_prepend = "-I${S}/include/"

inherit pkgconfig autotools

do_install() {
    oe_runmake install DESTDIR=${D} INCLUDEDIR=${includedir}
    install -d ${D}${libdir}
    oe_libinstall -C ${WORKDIR}/build/.libs/ -so libvcu_apm ${D}/${libdir}/
    install -d ${D}${libdir}/pkgconfig
    install -D -m 0644 ${S}/vcu-apm-lib.pc ${D}${libdir}/pkgconfig/
}
