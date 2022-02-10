#!/bin/bash
export QT_QPA_PLATFORM=eglfs
export QT_QPA_GENERIC_PLUGINS=libinput
export QT_QPA_ENABLE_TERMINAL_KEYBOARD=1
export QT_QPA_EGLFS_INTEGRATION=eglfs_x11
export DISPLAY=:0.0
export QT_SCALE_FACTOR=2
export OMX_ALLEGRO_PATH=/usr/lib/
export HACK_ENC_LATENCY=1
export HACK_DEC_LATENCY=5

Xorg -depth 24 &
