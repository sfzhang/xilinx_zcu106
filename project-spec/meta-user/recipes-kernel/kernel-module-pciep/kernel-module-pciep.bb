SUMMARY = "Out-of-tree PCIe platform driver"
SECTION = "kernel/modules"
DESCRIPTION = "The PCIe module driver allocates coherent memory in either PL or PS depending on the selection. It also triggers DEV_TO_MEM or MEM_TO_DEVICE DMA depending on the use-case."
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE.md;md5=a185b30a90cce88ba1cdafaaac5d8a40"

inherit module

BRANCH ?= "zynqmp_vcu_trd_v2021.2"
REPO   ?= "git://github.com/Xilinx/pcie-modules.git;protocol=https"
SRCREV ?= "6922b02d996af35a62ff5e967c23ccf4dd151bb2"

BRANCHARG = "${@['nobranch=1', 'branch=${BRANCH}'][d.getVar('BRANCH', True) != '']}"
SRC_URI = "${REPO};${BRANCHARG}"

S  = "${WORKDIR}/git"

EXTRA_OEMAKE += "O=${STAGING_KERNEL_BUILDDIR}"
COMPATIBLE_MACHINE = "^$"
COMPATIBLE_MACHINE_zynqmp = "zynqmp"

PACKAGE_ARCH = "${SOC_FAMILY_ARCH}"
