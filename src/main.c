
#ifndef DEFAULTDEVICE
#error DEFAULTDEVICE not set, check Makefile
#endif

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <dirent.h>

#ifdef GETOPTLONG
#include <getopt.h>
#endif /* GETOPTLONG */
#include <errno.h>
#ifndef ANDROID
#include <regex.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
//#include <sys/mtio.h>
#include <sys/mount.h>
#include <errno.h>

//#include "VBox.h"
#include "VBoxGuest.h"
#include <VBox/VMMDev.h>
#include <VBox/file.h>

# define VBOXGUEST_IOCTL_CODE_(Function, Size)      _IOC(_IOC_READ|_IOC_WRITE, 'V', (Function), (Size))
# define VBOXGUEST_IOCTL_CODE_FAST_(Function)       _IO(  'V', (Function))
# define VBOXGUEST_IOCTL_STRIP_SIZE(Code)           VBOXGUEST_IOCTL_CODE_(_IOC_NR((Code)), 0)

/** IOCTL to VBoxGuest to update the mouse status features. */
# define VBOXGUEST_IOCTL_SET_MOUSE_STATUS           VBOXGUEST_IOCTL_CODE_(10, sizeof(uint32_t))


#define RT_BIT(x)	(1<<(x))
/** @name Mouse capability bits (VMMDevReqMouseStatus::mouseFeatures).
 * @{ */
/** The guest can (== wants to) handle absolute coordinates.  */
#define VMMDEV_MOUSE_GUEST_CAN_ABSOLUTE                     RT_BIT(0)
/** The host can (== wants to) send absolute coordinates.
 * (Input not captured.) */
#define VMMDEV_MOUSE_HOST_WANTS_ABSOLUTE                    RT_BIT(1)
/** The guest can *NOT* switch to software cursor and therefore depends on the
 * host cursor.
 *
 * When guest additions are installed and the host has promised to display the
 * cursor itself, the guest installs a hardware mouse driver. Don't ask the
 * guest to switch to a software cursor then. */
#define VMMDEV_MOUSE_GUEST_NEEDS_HOST_CURSOR                RT_BIT(2)
/** The host does NOT provide support for drawing the cursor itself. */
#define VMMDEV_MOUSE_HOST_CANNOT_HWPOINTER                  RT_BIT(3)
/** The guest can read VMMDev events to find out about pointer movement */
#define VMMDEV_MOUSE_NEW_PROTOCOL                           RT_BIT(4)
/** If the guest changes the status of the
 * VMMDEV_MOUSE_GUEST_NEEDS_HOST_CURSOR bit, the host will honour this */
#define VMMDEV_MOUSE_HOST_RECHECKS_NEEDS_HOST_CURSOR        RT_BIT(5)
/** The host supplies an absolute pointing device.  The Guest Additions may
 * wish to use this to decide whether to install their own driver */
#define VMMDEV_MOUSE_HOST_HAS_ABS_DEV                       RT_BIT(6)
/** The mask of all VMMDEV_MOUSE_* flags */
#define VMMDEV_MOUSE_MASK                                   UINT32_C(0x0000007f)
/** The mask of guest capability changes for which notification events should
 * be sent */
#define VMMDEV_MOUSE_NOTIFY_HOST_MASK \
      (VMMDEV_MOUSE_GUEST_CAN_ABSOLUTE | VMMDEV_MOUSE_GUEST_NEEDS_HOST_CURSOR)
/** The mask of all capabilities which the guest can legitimately change */
#define VMMDEV_MOUSE_GUEST_MASK \
      (VMMDEV_MOUSE_NOTIFY_HOST_MASK | VMMDEV_MOUSE_NEW_PROTOCOL)
/** The mask of host capability changes for which notification events should
 * be sent */
#define VMMDEV_MOUSE_NOTIFY_GUEST_MASK \
      VMMDEV_MOUSE_HOST_WANTS_ABSOLUTE
/** The mask of all capabilities which the host can legitimately change */
#define VMMDEV_MOUSE_HOST_MASK \
      (  VMMDEV_MOUSE_NOTIFY_GUEST_MASK \
       | VMMDEV_MOUSE_HOST_CANNOT_HWPOINTER \
       | VMMDEV_MOUSE_HOST_RECHECKS_NEEDS_HOST_CURSOR \
       | VMMDEV_MOUSE_HOST_HAS_ABS_DEV)
/** @} */

/** @name Absolute mouse reporting range
 * @{ */
/** @todo Should these be here?  They are needed by both host and guest. */
/** The minumum value our pointing device can return. */
#define VMMDEV_MOUSE_RANGE_MIN 0
/** The maximum value our pointing device can return. */
#define VMMDEV_MOUSE_RANGE_MAX 0xFFFF
/** @} */


const char *programName;
static int g_File = -1;


/**
 * Implementation of VbglR3Init and VbglR3InitUser
 */
static int vbglR3Init(const char *pszDeviceName)
{

    /* The default implementation. (linux, solaris, freebsd, netbsd, haiku) */
    int rc = open(pszDeviceName, O_RDWR );
    if (rc < 0)
        return rc;
    g_File = rc;
    return 0;
}

/**
 * Open the VBox R3 Guest Library.  This should be called by system daemons
 * and processes.
 */
int VbglR3Init(void)
{
    return vbglR3Init(VBOXGUEST_DEVICE_NAME);
}


/**
 * Open the VBox R3 Guest Library.  Equivalent to VbglR3Init, but for user
 * session processes.
 */
int VbglR3InitUser(void)
{
    return vbglR3Init(VBOXGUEST_USER_DEVICE_NAME);
}

void VbglR3Term(void)
{
	close(g_File);
}

/**
 * Internal wrapper around various OS specific ioctl implementations.
 *
 * @returns VBox status code as returned by VBoxGuestCommonIOCtl, or
 *          an failure returned by the OS specific ioctl APIs.
 *
 * @param   iFunction   The requested function.
 * @param   pvData      The input and output data buffer.
 * @param   cbData      The size of the buffer.
 *
 * @remark  Exactly how the VBoxGuestCommonIOCtl is ferried back
 *          here is OS specific. On BSD and Darwin we can use errno,
 *          while on OS/2 we use the 2nd buffer of the IOCtl.
 */
int vbglR3DoIOCtl(unsigned iFunction, void *pvData, size_t cbData)
{
    return ioctl(g_File, iFunction, pvData);
}

int vbglR3GRPerform(VMMDevRequestHeader *pReq)
{
    return vbglR3DoIOCtl(VBOXGUEST_IOCTL_VMMREQUEST(pReq->size), pReq, pReq->size);
}

#if 0
/**
* Retrieve mouse coordinates and features from the host.
*
* @returns VBox status code.
*
* @param   pfFeatures  Where to store the mouse features.
* @param   px          Where to store the X co-ordinate.
* @param   py          Where to store the Y co-ordinate.
*/
int VbglR3GetMouseStatus(uint32_t *pfFeatures, uint32_t *px, uint32_t *py)
{
    VMMDevReqMouseStatus Req;
    vmmdevInitRequest(&Req.header, VMMDevReq_GetMouseStatus);
    Req.mouseFeatures = 0;
    Req.pointerXPos = 0;
    Req.pointerYPos = 0;
    int rc = vbglR3GRPerform(&Req.header);
    if (RT_SUCCESS(rc))
    {
        if (pfFeatures)
            *pfFeatures = Req.mouseFeatures;
        if (px)
            *px = Req.pointerXPos;
        if (py)
            *py = Req.pointerYPos;
    }
    return rc;
}
#endif

/**
 * Send mouse features to the host.
 *
 * @returns VBox status code.
 *
 * @param   fFeatures  Supported mouse pointer features.  The main guest driver
 *                     will mediate different callers and show the host any
 *                     feature enabled by any guest caller.
 */
int setMouseStatus(int fd, uint32_t fFeatures)
{
    return ioctl(fd, VBOXGUEST_IOCTL_SET_MOUSE_STATUS, &fFeatures);
}



/* main program */
int main(int argc, char **argv)
{
	const char *defaultDevice = DEFAULTDEVICE;  /* default if no name passed by user */
	int fd; 	   /* file descriptor for device */
	int rc;

	/* program name is global variable used by other procedures */
	programName = strdup(argv[0]);

	/* parse the command line arguments */
	//parse_args(argc, argv, &device);



	/* open device */
	fd = open(defaultDevice, O_RDWR);
	if(fd < 0)
		return 1;

	// VMMDEV_MOUSE_NEW_PROTOCOL - включить Mouse Integration
	rc = setMouseStatus(fd, VMMDEV_MOUSE_GUEST_NEEDS_HOST_CURSOR |
							VMMDEV_MOUSE_GUEST_CAN_ABSOLUTE );
	if(rc < 0)
		perror("vboxctl: set mouse state: ");
	close(fd);
	return 0;
}

