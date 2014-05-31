/*
 * standalone_stubs.c
 *
 *  Created on: 30/05/2014
 *      Author: dean
 *
 *      fix standalone linker issues with totally fake and non-working stubs.
 */

int _close(int fd) { return 0; }
int _fstat(int fd, struct stat *buf) { return 0; }
int _isatty(int fd) { return 0; }
//off_t _lseek(int fd, off_t offset, int whence) { return (off_t)-1; }
int _open(const char *buf, int flags, int mode) { return -1; }
