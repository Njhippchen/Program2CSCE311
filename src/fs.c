#ifndef FS_H
#define FS_H

#include <stdint.h>

/* Files are registered at kernel compile/link-time as byte blobs.
   Each registered file gets an ID (index). Kernel exposes syscalls to read them. */

int fs_register(const void *data, int size); /* returns file id */
const void *fs_get_ptr(int id);
int fs_get_size(int id);

#endif
