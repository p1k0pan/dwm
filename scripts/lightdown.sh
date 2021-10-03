originBri=`cat /sys/class/backlight/intel_backlight/brightness`
currentBri=$((`cat /sys/class/backlight/intel_backlight/brightness`- 50))
maxBri=$(cat /sys/class/backlight/intel_backlight/max_brightness)
if [ $currentBri -ge 0 ] ; then
    echo $currentBri > /sys/class/backlight/intel_backlight/brightness
    echo "Current brightness : $currentBri"
elif [ $currentBri -le 0 ] ; then
    echo "failed: The limit of brightness: 0 - $maxBri"
    echo "Current brightness : $originBri"
fi
