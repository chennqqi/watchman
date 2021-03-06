/* Copyright 2017-present Facebook, Inc.
 * Licensed under the Apache License, Version 2.0 */
#pragma once
#include "FileInformation.h"
#include "FileDescriptor.h"
#include "Win32Handle.h"
#include "Result.h"

/** This header defines platform independent helper functions for
 * operating on the filesystem at a low level.
 * These functions are intended to be used to query information from
 * the filesystem, rather than implementing a full-fledged abstraction
 * for general purpose use.
 *
 * One of the primary features of this header is to provide an OS-Independent
 * alias for the OS-Dependent file descriptor type.
 *
 * The functions in this file generally return or operate on an instance of
 * that type.
 */

namespace watchman {

#ifndef _WIN32
using FileHandleType = FileDescriptor;
#else
using FileHandleType = Win32Handle;
#endif

/** Windows doesn't have equivalent bits for all of the various
 * open(2) flags, so we abstract it out here */
struct OpenFileHandleOptions {
  unsigned followSymlinks : 1;   // O_NOFOLLOW
  unsigned closeOnExec : 1;      // O_CLOEXEC
  unsigned metaDataOnly : 1;     // avoid accessing file contents
  unsigned readContents : 1;     // the read portion of O_RDONLY or O_RDWR
  unsigned writeContents : 1;    // the write portion of O_WRONLY or O_RDWR
  unsigned create : 1;           // O_CREAT
  unsigned exclusiveCreate : 1;  // O_EXCL
  unsigned truncate : 1;         // O_TRUNC

  OpenFileHandleOptions()
      : followSymlinks(0), closeOnExec(1), metaDataOnly(0), readContents(0),
        writeContents(0), create(0), exclusiveCreate(0), truncate(0) {}

  static inline OpenFileHandleOptions queryFileInfo() {
    OpenFileHandleOptions opts;
    opts.metaDataOnly = 1;
    return opts;
  }
};

/** equivalent to open(2)
 * This function is not intended to be used to create files,
 * just to open a file handle to query its metadata */
FileHandleType openFileHandle(const char *path,
                              const OpenFileHandleOptions &opts);
}
