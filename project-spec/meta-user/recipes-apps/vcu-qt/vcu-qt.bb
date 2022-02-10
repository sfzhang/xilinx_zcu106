#
# This is the vct-qt application recipe
#

SUMMARY = "vcu-qt demo application"
SECTION = "PETALINUX/apps"
DESCRIPTION = "This QT based Graphical User Interface application provides control and monitoring interface to run multimedia usecases."
LICENSE = "LGPLv2"
LIC_FILES_CHKSUM = "file://LICENSE.md;md5=b60ab04828851b8b8d8ad651889ac94d"

BRANCH ?= "zynqmp_vcu_trd_v2021.2"
REPO   ?= "git://github.com/Xilinx/vcu_qt.git;protocol=https"
SRCREV ?= "3bea2c127a39359f0ecf58ef15f6e860c149b9c9"

BRANCHARG = "${@['nobranch=1', 'branch=${BRANCH}'][d.getVar('BRANCH', True) != '']}"
SRC_URI = "${REPO};${BRANCHARG}"

SRC_URI += "file://configure_qos.sh;subdir=src \
		file://qt_env.sh;subdir=src \
		file://run_vcu.sh;subdir=src "

EXTRA_QMAKEVARS_POST = "-r -spec linux-oe-g++"
S  = "${WORKDIR}/git"
DEPENDS = "qtdeclarative qtgraphicaleffects qtsvg qtmultimedia qtcharts vcu-video-lib vcu-apm-lib vcu-gst-lib"
VCU_QT_BIN_DIR = "opt/vcu_qt/bin"

do_configure_prepend() {
	mv ${S}/include_mv ${S}/include
	export SDKTARGETSYSROOT=${PKG_CONFIG_SYSROOT_DIR}
}

require recipes-qt/qt5/qt5.inc

do_configure_prepend() {
	mv ${S}/include ${S}/include_mv
}

do_compile_prepend() {
	export SDKTARGETSYSROOT=${PKG_CONFIG_SYSROOT_DIR}
}

do_install() {
	install -d ${D}/${VCU_QT_BIN_DIR}
	oe_runmake install INSTALL_ROOT=${B}
	install -m 0755 ${B}/vcu_qt ${D}/${VCU_QT_BIN_DIR}
	install -m 0755 ${WORKDIR}/src/configure_qos.sh ${D}/${VCU_QT_BIN_DIR}
	install -m 0755 ${WORKDIR}/src/qt_env.sh ${D}/${VCU_QT_BIN_DIR}
	install -m 0755 ${WORKDIR}/src/run_vcu.sh ${D}/${VCU_QT_BIN_DIR}
}

FILES_${PN} += "/opt/vcu_qt/bin/vcu_qt \
		/opt/vcu_qt/bin/configure_qos.sh \
		/opt/vcu_qt/bin/qt_env.sh \
		/opt/vcu_qt/bin/run_vcu.sh "

RDEPENDS_${PN} = "qtdeclarative-qmlplugins qtgraphicaleffects-qmlplugins bash"
