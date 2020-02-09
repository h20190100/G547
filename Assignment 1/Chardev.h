

#ifndef CHARDEV_H
#define CHARDEV_H

#include <linux/ioctl.h>


#define MAJOR_NUM 220

/*
 * Set the message of the device driver
 */
#define IOCTL_SET_CHANNEL _IOW(MAJOR_NUM, 0, unsigned int)
#define IOCTL_SET_ALIGNMENT _IOW(MAJOR_NUM, 1, unsigned int)

#endif
