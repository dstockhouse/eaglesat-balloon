#!/bin/bash

echo -e "powersave" | sudo tee /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
echo -e "powersave" | sudo tee /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor
echo -e "powersave" | sudo tee /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor
echo -e "powersave" | sudo tee /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor

