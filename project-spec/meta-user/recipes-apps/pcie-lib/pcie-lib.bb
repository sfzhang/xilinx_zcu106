#
# This is the pcie library recipe
#
#
SUMMARY = "pcie library"
SECTION = "PETALINUX/libs"
DESCRIPTION = "The PCIe library provides an interface to the application to communicate with the PCIe device."
LICENSE = "LGPLv2"
LIC_FILES_CHKSUM = "file://LICENSE.md;md5=b60ab04828851b8b8d8ad651889ac94d"

BRANCH ?= "zynqmp_vcu_trd_v2021.2"
REPO   ?= "git://github.com/Xilinx/pcie_lib.git;protocol=https"
SRCREV ?= "ffd1781dc0dda3629df06fffa7d45ba1ab8eba4c"

BRANCHARG = "${@['nobranch=1', 'branch=${BRANCH}'][d.getVar('BRANCH', True) != '']}"
SRC_URI = "${REPO};${BRANCHARG}"

S  = "${WORKDIR}/git"

CFLAGS_prepend = "-I${S}/include/"

# inherit line
inherit pkgconfig autotools

do_install() {
    oe_runmake install DESTDIR=${D} INCLUDEDIR=${includedir}
    install -d ${D}${libdir}
    oe_libinstall -C ${WORKDIR}/build/.libs/ -so libvcu_pcie ${D}/${libdir}/
    install -d ${D}${libdir}/pkgconfig
    install -D -m 0644 ${S}/pcie-lib.pc ${D}${libdir}/pkgconfig/
}

