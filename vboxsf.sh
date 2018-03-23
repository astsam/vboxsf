#!/system/bin/sh
# 1015 - sdcard_rw, 1023 - media_rw, 3003 - inet
# 1023: /mnt/media_rw
#GID=3003
#UID=3003
#dir=/mnt/vboxsf
GID=1023
UID=1023
dir=/mnt/media_rw/vboxsf
modprobe vboxsf
mkdir $dir
chown $UID.$GID $dir
/system/bin/mount.vboxsf -o uid=$UID,gid=$GID,dmode=0777 vboxsf $dir

