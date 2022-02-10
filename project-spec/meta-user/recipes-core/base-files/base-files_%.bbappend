FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
dirs755 += "/media/card"

do_install_append () {
	echo "/dev/mmcblk0p1 /media/card auto defaults  0  0" >> ${D}${sysconfdir}/fstab
	echo "export QT_QPA_PLATFORM=eglfs" >> ${D}${sysconfdir}/profile
	echo "export QT_QPA_GENERIC_PLUGINS=libinput" >> ${D}${sysconfdir}/profile
	echo "export QT_QPA_ENABLE_TERMINAL_KEYBOARD=1" >> ${D}${sysconfdir}/profile
	echo "export QT_QPA_EGLFS_INTEGRATION=eglfs_x11" >> ${D}${sysconfdir}/profile
}
