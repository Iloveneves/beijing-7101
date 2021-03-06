/*****************************************************************************
 *
 *  Module      : staudlx_core_ops.c
 *  Date        : 02-07-2005
 *  Author      : WALKERM
 *  Description :
 *  Copyright   : STMicroelectronics (c)2005 
 *****************************************************************************/

/* Requires   MODULE   defined on the command line */
/* Requires __KERNEL__ defined on the command line */
/* Requires __SMP__    defined on the command line for SMP systems */

#define _NO_VERSION_
#include <linux/module.h>  /* Module support */
#include <linux/version.h> /* Kernel version */
#include <linux/fs.h>      /* File operations (fops) defines */
#include <linux/ioport.h>  /* Memory/device locking macros   */
#include <linux/poll.h>    /* Poll and select support        */
#include <linux/errno.h>   /* Defines standard error codes */

#include "staudlx_core_types.h"       /* Modules specific defines */

/*** PROTOTYPES **************************************************************/


#include "staudlx_core_fops.h"


int staudlx_core_readdir(struct file *filp,  void *other, filldir_t othermore)
{
    return (0);
}


unsigned int staudlx_core_poll   (struct file *filp,  struct poll_table_struct *pollp)
{
    int err = 0;    
    unsigned int mask = 0;

                                                                         
    staudlx_core_dev_t *dev_p = (staudlx_core_dev_t*)filp->private_data;                  

    /*** Get the lock ***/
    
    if (down_interruptible(&(dev_p->lock))) {
    
        /* Call interrupted */
	mask = POLLERR | POLLIN | POLLOUT | POLLRDNORM | POLLWRNORM;
	/* Need to think about this. */
	/* Where should the error be put since read and write should return error */
	goto fault;
    }

    poll_wait(filp, &dev_p->read_queue, pollp);
    poll_wait(filp, &dev_p->write_queue, pollp);

    if( dev_p->write_idx != dev_p->read_idx ){
    
        mask |= POLLIN | POLLRDNORM; /* Readable */
    }
    
    if( (dev_p->write_idx+1)%DATA_BUFFER_SIZE != dev_p->read_idx ){
    
        mask |= POLLOUT | POLLWRNORM; /* Writeable */
    }
    
    /* Release the lock */
    up(&(dev_p->lock));

    return mask;
    
fault:
    
    return (err);
}


int staudlx_core_mmap   (struct file *filp,  struct vm_area_struct *memp)
{
    return (-EBUSY);
}


int staudlx_core_fsync  (struct inode *node, struct dentry *entry, int other)
{
    return (-EBUSY);
}




