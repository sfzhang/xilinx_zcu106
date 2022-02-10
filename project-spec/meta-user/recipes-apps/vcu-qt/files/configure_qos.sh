#!/bin/bash

/sbin/devmem 0xfd360008 w 0x3 #RDQoS for S_AXI_HPC0_FPD
/sbin/devmem 0xfd370008 w 0x3 #RDQoS for S_AXI_HPC1_FPD
/sbin/devmem 0xfd380008 w 0x3 #RDQoS for S_AXI_HP0_FPD
/sbin/devmem 0xfd390008 w 0x3 #RDQoS for S_AXI_HP1_FPD
/sbin/devmem 0xfd3a0008 w 0x3 #RDQoS for S_AXI_HP2_FPD
/sbin/devmem 0xfd3b0008 w 0x3 #RDQoS for S_AXI_HP3_FPD

/sbin/devmem 0xfd36001c w 0x3 #WRQoS for S_AXI_HPC0_FPD
/sbin/devmem 0xfd37001c w 0x3 #WRQoS for S_AXI_HPC1_FPD
/sbin/devmem 0xfd38001c w 0x3 #WRQoS for S_AXI_HP0_FPD
/sbin/devmem 0xfd39001c w 0x3 #WRQoS for S_AXI_HP1_FPD
/sbin/devmem 0xfd3a001c w 0x3 #WRQoS for S_AXI_HP2_FPD
/sbin/devmem 0xfd3b001c w 0x3 #WRQoS for S_AXI_HP3_FPD

/sbin/devmem 0xfd360004 w 0xF #RDISSUE for S_AXI_HPC0_FPD
/sbin/devmem 0xfd370004 w 0xF #RDISSUE for S_AXI_HPC1_FPD
/sbin/devmem 0xfd380004 w 0xF #RDISSUE for S_AXI_HP0_FPD
/sbin/devmem 0xfd390004 w 0xF #RDISSUE for S_AXI_HP1_FPD
/sbin/devmem 0xfd3A0004 w 0xF #RDISSUE for S_AXI_HP2_FPD
/sbin/devmem 0xfd3B0004 w 0xF #RDISSUE for S_AXI_HP3_FPD

/sbin/devmem 0xfd360018 w 0xF #WRISSUE for S_AXI_HPC0_FPD
/sbin/devmem 0xfd370018 w 0xF #WRISSUE for S_AXI_HPC1_FPD
/sbin/devmem 0xfd380018 w 0xF #WRISSUE for S_AXI_HP0_FPD
/sbin/devmem 0xfd390018 w 0xF #WRISSUE for S_AXI_HP1_FPD
/sbin/devmem 0xfd3A0018 w 0xF #WRISSUE for S_AXI_HP2_FPD
/sbin/devmem 0xfd3B0018 w 0xF #WRISSUE for S_AXI_HP3_FPD
