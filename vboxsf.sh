#!/system/bn/sh
GID=3003
UID=3003
modprobe vboxsf
mkdir /mnt/vboxsf
chown $UID.$GID /mnt/vboxsf
/system/bin/mount.vboxsf -o uid=$UID,gid=$GID,dmode=0777 vboxsf /mnt/vboxsf

