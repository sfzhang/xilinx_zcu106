SRC_URI += "file://bsp.cfg"
KERNEL_FEATURES_append = " bsp.cfg"

SRC_URI_append = ""

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
