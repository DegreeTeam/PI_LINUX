TARGETS = fake-hwclock mountkernfs.sh hostname.sh udev keyboard-setup mountdevsubfs.sh checkroot.sh console-setup mountall.sh mountall-bootclean.sh mountnfs.sh mountnfs-bootclean.sh networking urandom alsa-utils x11-common kbd plymouth-log udev-mtab kmod mtab.sh bootmisc.sh procps checkroot-bootclean.sh checkfs.sh
INTERACTIVE = udev keyboard-setup checkroot.sh console-setup kbd checkfs.sh
udev: mountkernfs.sh
keyboard-setup: mountkernfs.sh udev
mountdevsubfs.sh: mountkernfs.sh udev
checkroot.sh: fake-hwclock keyboard-setup mountdevsubfs.sh hostname.sh
console-setup: mountall.sh mountall-bootclean.sh mountnfs.sh mountnfs-bootclean.sh kbd
mountall.sh: checkfs.sh checkroot-bootclean.sh
mountall-bootclean.sh: mountall.sh
mountnfs.sh: mountall.sh mountall-bootclean.sh networking
mountnfs-bootclean.sh: mountall.sh mountall-bootclean.sh mountnfs.sh
networking: mountkernfs.sh mountall.sh mountall-bootclean.sh urandom
urandom: mountall.sh mountall-bootclean.sh
alsa-utils: mountall.sh mountall-bootclean.sh mountnfs.sh mountnfs-bootclean.sh
x11-common: mountall.sh mountall-bootclean.sh mountnfs.sh mountnfs-bootclean.sh
kbd: mountall.sh mountall-bootclean.sh mountnfs.sh mountnfs-bootclean.sh
plymouth-log: mountall.sh mountall-bootclean.sh mountnfs.sh mountnfs-bootclean.sh
udev-mtab: udev mountall.sh mountall-bootclean.sh
kmod: checkroot.sh
mtab.sh: checkroot.sh
bootmisc.sh: mountall-bootclean.sh mountnfs-bootclean.sh checkroot-bootclean.sh mountall.sh mountnfs.sh udev
procps: mountkernfs.sh mountall.sh mountall-bootclean.sh udev
checkroot-bootclean.sh: checkroot.sh
checkfs.sh: checkroot.sh mtab.sh
