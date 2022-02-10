#
# This is the pcie transcode application recipe
#
#
SUMMARY = "pcie_transcode application"
SECTION = "PETALINUX/apps"
DESCRIPTION = "PCIe application which communicats between host to device and vice versa. Pull the data from host, performs encode,decode or transcode use case and push processed data to host."
LICENSE = "LGPLv2"
LIC_FILES_CHKSUM = "file://LICENSE.md;md5=b60ab04828851b8b8d8ad651889ac94d"

BRANCH ?= "zynqmp_vcu_trd_v2021.2"
REPO   ?= "git://github.com/Xilinx/pcie_transcode.git;protocol=https"
SRCREV ?= "9c209b3eb501edc6b9d0b32a0b9122c130ef73a7"

BRANCHARG = "${@['nobranch=1', 'branch=${BRANCH}'][d.getVar('BRANCH', True) != '']}"
SRC_URI = "${REPO};${BRANCHARG}"

S  = "${WORKDIR}/git"

CFLAGS_prepend = "-I${S}/include/"

DEPENDS = "gstreamer1.0 gstreamer1.0-plugins-base v4l-utils libdrm pcie-lib"

# inherit line
inherit pkgconfig autotools

do_install() {
	install -d ${D}/${bindir}
	oe_runmake install DESTDIR=${D}
	install -m 0755 ${B}/pcie_transcode ${D}/${bindir}
}
