# vboxsf
Utility for Android to mount VirtualBox shared folders 
## Theory
To access a shared folder, a guest OS must have:
1. The `vboxsf` and `vboxguest` drivers from guest additions.
2. Utility (mount.vboxsf) to mount shared folders. Sources are available on the http://www.virtualbox.org.
The `src` directory of this project contains a port of the mount.vboxsf utility for Android.
3. Some glue to mount shared folders on boot. For marshmallow this can be done by patching the init.rc script. The `vboxsf.sh` script mounts shared folders at the `/mnt/media_rw/vboxsf` directory on a guest.

## Prerequisits
1. Download android_x86 sources from http://www.android_x86.org.
2. Download VBoxGuestAdditions.iso from https://download.virtualbox.org/virtualbox/. If VirtualBox is installed the iso image is located in the /usr/share/virtualbox directory.
3. Install 7z package.

## Build android_x86.iso with VirtualBox shared folders and mouse integration support
1. Copy guest additions sources to the android-x86 tree.
```bash 
$ ./scripts/guest-copy VBoxGuestAdditions.iso android-x86
```
2. For marshmallow apply patch from `patches/marshmallow` (not needed for nougat)
```bash 
$ patch -p1 < path/to/this/project/patches/marshmallow/mount-vbox-shared-folders.patch
```
3. Apply mouse integration patch (for both marshmallow and nougat)
```bash 
$ patch -p1 < path/to/this/project/patches/mouse-integration.patch
```
4. Build `android_x86.iso` image (see http://www.android_x86.org for instructions).
5. Done!

## Scripts to build iso image with additions for Android
1. Extract guest drivers sources from VBoxGuestAdditions.iso. Sources will be placed in the `work` directory.
```bash 
$ ./scripts/guest-extract VBoxGuestAdditions.iso
```
2. Create iso image
```bash 
$ ./scripts/guest-build android-x86
```
## Notes 
1. Shared folder name must be `vboxsf`. Edit file `vboxsf.sh` if another name is used.
2. Shared folder mount point is `/mnt/media_rf_vboxsf` directory. Edit file `vboxsf.sh` to change
mount point.

## Todo
- [x] port the `mount.vboxsf` utility for Android
- [x] scripts to extract and build guest drivers from VBoxGuestAdditions.iso
- [x] add mount.vboxsf to VBoxGuestAdditionsAndroid.iso
- [x] mouse integration
- [ ] query VirtualBox for a list of configured shared folders

## Links
* Build guest drivers https://github.com/coyun/android-virtualbox/issues/18
* Mouse integration https://github.com/coyun/android-virtualbox/issues/2
* Building AOSP 7.x Nougat https://github.com/mikecriggs/fuckjack, http://www.2net.co.uk/blog/jack-server.html
