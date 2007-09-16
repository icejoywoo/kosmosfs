//---------------------------------------------------------- -*- Mode: C++ -*-
// $Id: //depot/SOURCE/OPENSOURCE/kfs/src/cc/libkfsClient/KfsAttr.h#3 $
//
// Created 2006/06/09
// Author: Sriram Rao (Kosmix Corp.) 
//
// Copyright 2006 Kosmix Corp.
//
// This file is part of Kosmos File System (KFS).
//
// Licensed under the Apache License, Version 2.0
// (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
// implied. See the License for the specific language governing
// permissions and limitations under the License.
//
// 
// \file KfsAttr.h
// \brief Kfs File/Chunk attributes.
//
//----------------------------------------------------------------------------

#ifndef LIBKFSCLIENT_KFSATTR_H
#define LIBKFSCLIENT_KFSATTR_H

extern "C" {
#include <sys/time.h>
}

#include <string>
#include <vector>
using std::string;
using std::vector;

#include "common/kfstypes.h"
#include "common/kfsdecls.h"

namespace KFS {

struct ChunkAttr {
    /// where is this chunk hosted.  There is a minor convention here:
    /// for writes, chunkServerLoc[0] holds the location of the
    /// "master" server that runs the transaction for writes.
    vector<ServerLocation> chunkServerLoc;

    /// what is the id of this chunk
    kfsChunkId_t chunkId;
    // what is the version # of this chunk
    int64_t  chunkVersion;

    /// did we request an allocation for this chunk?
    bool didAllocation;
    /// what is the size of this chunk
    size_t	chunkSize;
    ChunkAttr(): chunkId(-1), chunkVersion(-1), didAllocation(false),
                 chunkSize(0) { }
};

///
/// \brief Attributes downloaded from the metadata server for a file.
///
struct KfsServerAttr {
    /// the id of the file
    kfsFileId_t fileId;
    struct timeval mtime; /// modification time
    struct timeval ctime; /// attribute change time
    struct timeval crtime; /// creation time
    /// how many chunks does it have
    long long	chunkCount;
    /// is this a directory?
    bool	isDirectory;
    /// what is the deg. of replication for this file
    int16_t numReplicas;
};


///
/// \brief Server attributes + chunk attributes
///
struct FileAttr {
    /// the id of this file
    kfsFileId_t fileId;
    struct timeval mtime; /// modification time
    struct timeval ctime; /// attribute change time
    struct timeval crtime; /// creation time
    /// is this a directory?
    bool	isDirectory;
    /// the size of this file in bytes---computed value
    ssize_t	fileSize;
    
    /// how many chunks does it have
    long long	chunkCount;
    
    /// what is the deg. of replication for this file
    int16_t numReplicas;

    FileAttr() {
        fileId = (kfsFileId_t) -1;
        fileSize = 0;
        chunkCount = 0;
        numReplicas = 0;
        isDirectory = false;
    }
    void Reset() {
        fileId = (kfsFileId_t) -1;
        fileSize = 0;
        chunkCount = 0;
        numReplicas = 0;
        isDirectory = false;
    }
    void Init(bool isDir) {
        isDirectory = isDir;
        gettimeofday(&mtime, NULL);
        ctime = mtime;
        crtime = mtime;
    }
    FileAttr& operator= (const KfsServerAttr &other) {
        fileId = other.fileId;
        chunkCount = other.chunkCount;
        numReplicas = other.numReplicas;
        fileSize = 0; // need to compute this
        isDirectory = other.isDirectory;
        mtime = other.mtime;
        ctime = other.ctime;
        crtime = other.crtime;
        return *this;
    }
};

///
/// \brief File attributes as usable by applications.
///
/// 
struct KfsFileAttr {
    /// the name of this file
    string filename;
    /// the id of this file
    kfsFileId_t fileId;
    struct timeval mtime; /// modification time
    struct timeval ctime; /// attribute change time
    struct timeval crtime; /// creation time
    /// is this a directory?
    bool	isDirectory;
    /// the size of this file in bytes
    ssize_t	fileSize;

    KfsFileAttr& operator= (const KfsServerAttr &other) {
        fileId = other.fileId;
        fileSize = 0; // compute this from other.chunkCount
        isDirectory = other.isDirectory;
        mtime = other.mtime;
        ctime = other.ctime;
        crtime = other.crtime;
        return *this;
    }

    KfsFileAttr& operator= (const FileAttr &other) {
        fileId = other.fileId;
        fileSize = other.fileSize;
        isDirectory = other.isDirectory;
        mtime = other.mtime;
        ctime = other.ctime;
        crtime = other.crtime;
        return *this;
    }

};

}

#endif // LIBKFSCLIENT_KFSATTR_H
