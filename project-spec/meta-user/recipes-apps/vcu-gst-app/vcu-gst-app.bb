#
# VCU GStreamer application recipe
#
SUMMARY = "VCU GStreamer application"
SECTION = "PETALINUX/apps"
DESCRIPTION = "This is a command line multi threaded application to run the Gstreamer pipeline. It requires an input configuration file to be provided in the plain text."
LICENSE = "LGPLv2"
LIC_FILES_CHKSUM = "file://LICENSE.md;md5=b60ab04828851b8b8d8ad651889ac94d"

BRANCH ?= "zynqmp_vcu_trd_v2021.2"
REPO   ?= "git://github.com/Xilinx/vcu_gst_app.git;protocol=https"
SRCREV ?= "af95f233f9cce1bf684bbb7e045709eb92e8e1fe"

BRANCHARG = "${@['nobranch=1', 'branch=${BRANCH}'][d.getVar('BRANCH', True) != '']}"
SRC_URI = "${REPO};${BRANCHARG}"

S  = "${WORKDIR}/git"

CFLAGS_prepend = "-I${S}/include/"

DEPENDS = "vcu-apm-lib vcu-gst-lib"

inherit pkgconfig autotools

do_install() {
    install -d ${D}/${bindir}
    install -m 0755 ${B}/vcu_gst_app ${D}/${bindir}
}
