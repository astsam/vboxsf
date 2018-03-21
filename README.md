# vboxsf
Utility for Android to mount VirtualBox shared folders 
## Theory
To access a shared folder, a guest OS must have:
1. The driver vboxsf from guest additions (coming soon, see https://github.com/coyun/android-virtualbox/issues/18).
2. Utility (mount.vboxsf) to mount shared folders. Sources are available on the http://www.virtualbox.org.
This project is a port of the mount.vboxsf for Android.
3. Some glue to mount folders on boot. This can be done by patching the init.rc script.
## Build
1. Download android_x86 sources from http://www.android_x86.org. Make the android_x86 directory current.
2. Create directory for the utility in the directory `external`.
```bash 
$ mkdir external/vboxsf
```
3. Clone the repository into the `external/vboxsf` directory.
4. Apply patch from `external/vboxsf/patches`
```bash 
$ patch -p1 < external/vboxsf/patches/mount-vbox-shared-folders.patch
```
5. Build android_x86 iso image (see http://www.android_x86.org for instructions).

## Todo
- [x] port the `mount.vboxsf` for Android
- [ ] extract and build guest drivers from VBoxGuestAdditions.iso
- [ ] mouse integration

## Links
* Build guest drivers https://github.com/coyun/android-virtualbox/issues/18
* Mouse integration https://github.com/coyun/android-virtualbox/issues/2
