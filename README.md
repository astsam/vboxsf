# vboxsf
Utility for Android to mount VirtualBox shared folders 
## Theory
To access a shared folder, a guest OS must have:
1. The driver vboxsf from guest additions (coming soon, see https://github.com/coyun/android-virtualbox/issues/18).
2. Utility (mount.vboxsf) to mount shared folders. Sources are available on the http://www.virtualbox.org.
The `src` directory contains a port of the mount.vboxsf utility for Android.
3. Some glue to mount shared folders on boot. For marshmallow this can be done by patching the init.rc script. Shared folders will be mount at /mnt/media_rw/vboxsf directory on a guest.

## Prerequisits
1. Download android_x86 sources from http://www.android_x86.org.
2. Download VBoxGuestAdditions.iso from https://download.virtualbox.org/virtualbox/. If VirtualBox is installed the iso image is located in the /usr/share/virtualbox directory.
3. Install 7z package.

## Build android_x86.iso with VirtualBox shared folders support
1. Copy guest additions sources to the android-x86 tree.
```bash 
$ ./scripts/guest-copy VBoxGuestAdditions.iso android-x86
```
2. For marshmallow apply patch from `patches/marshmallow` (not needed for nougat)
```bash 
$ patch -p1 < path/to/this/project/patches/marshmallow/mount-vbox-shared-folders.patch
```
3. Build `android_x86.iso` image (see http://www.android_x86.org for instructions).

## Build iso image with additions for Android (in progress)
1. Extract guest drivers sources from VBoxGuestAdditions.iso. Sources will be placed in the `work` directory.
```bash 
$ ./scripts/guest-extract VBoxGuestAdditions.iso
```
2. Create iso image
```bash 
$ ./scripts/guest-build android-x86
```
## Todo
- [x] port the `mount.vboxsf` utility for Android
- [x] scripts to extract and build guest drivers from VBoxGuestAdditions.iso
- [ ] add mount.vboxsf to VBoxGuestAdditionsAndroid.iso
- [ ] mouse integration

## Links
* Build guest drivers https://github.com/coyun/android-virtualbox/issues/18
* Mouse integration https://github.com/coyun/android-virtualbox/issues/2
