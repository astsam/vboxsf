/* $Id: vbsfmount.c 62527 2016-07-22 19:18:14Z vboxsync $ */
/** @file
 * vbsfmount - Commonly used code to mount shared folders on Linux-based
 *             systems.  Currently used by mount.vboxsf and VBoxService.
 */

/*
 * Copyright (C) 2010-2016 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#include "vbsfmount.h"

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <ctype.h>
#include <mntent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>

#pragma GCC diagnostic ignored "-Wunused-parameter"
/** @todo Use defines for return values! */
int vbsfmount_complete(const char *host_name, const char *mount_point,
                       unsigned long flags, struct vbsf_mount_opts *opts)
{
 	int rc = 3;  /* Could not add an entry to the mount table. */

    return rc;
}
