SRC_URI += "file://bsp.cfg \
            file://user_2022-02-10-06-03-00.cfg \
            "
KERNEL_FEATURES_append = " bsp.cfg"

SRC_URI_append = ""

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
