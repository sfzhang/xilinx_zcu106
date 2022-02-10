DESCRIPTION = "HDMI Rx Custom EDID Binary"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " file://xilinx-hdmi-rx-edid.bin"

S = "${WORKDIR}"

do_install () {
    install -d ${D}/lib/firmware/xilinx/
	install -m 0644 xilinx-hdmi-rx-edid.bin ${D}/lib/firmware/xilinx/
}

FILES_${PN} = "lib/firmware/xilinx/*"
