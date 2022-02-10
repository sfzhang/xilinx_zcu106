#!/bin/bash

#CONFIG_QOS_DIR="/media/card/vcu"
#BIN_DIR="/media/card/bin"
CONFIG_QOS_DIR="/opt/vcu_qt/bin"
BIN_DIR="/opt/vcu_qt/bin"

# Config VCU QOS
source ${CONFIG_QOS_DIR}/configure_qos.sh

# Kill and previous instance of vcu_qt/Xorg
killall vcu_qt
killall Xorg

# Disable global alpha before starting GUI
modetest -D fd4a0000.display -s 43:3840x2160-30@BG24 -w 40:"g_alpha_en":0

function detect_screen() {
    MONITOR_DP="/sys/devices/platform/axi/fd4a0000.display/drm/card*/card*/status"
    MONITOR_HDMI="/sys/devices/platform/amba/a0070000.v_mix/drm/card*/card*/status"

    if [ $(cat ${MONITOR_DP} 2>/dev/null) = "connected" ]; then
        echo 0
    elif [ $(cat ${MONITOR_HDMI} 2>/dev/null) = "connected" ]; then
        echo 1
    fi
}

if [ -z "${device}" ]; then
	device=$(detect_screen)

	if [ -z "${device}" ]; then
		echo "ERROR: No display device found"
		exit 2
	fi
fi

if [ $device = 1 ]; then
    exit 2
fi

sed -i -e "s/\(Option\s\+\"DRICard\"\s\+\)\"[0-9]\+\"/\1\"${device}\"/" /etc/X11/xorg.conf

echo "Starting application..."

source ${BIN_DIR}/qt_env.sh

# Set allegro lib path
echo  "Set  allegro path "
export OMX_ALLEGRO_PATH=/usr/lib/

echo "device = ${device}"
${BIN_DIR}/vcu_qt
sleep 2
