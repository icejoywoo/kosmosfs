//---------------------------------------------------------- -*- Mode: C++ -*-
// $Id: //depot/SOURCE/OPENSOURCE/kfs/src/cc/chunk/Utils.h#2 $
//
// Created 2006/09/27
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
//----------------------------------------------------------------------------

#ifndef CHUNKSERVER_UTILS_H
#define CHUNKSERVER_UTILS_H

#include "libkfsIO/IOBuffer.h"
#include <string>
using std::string;

///
/// Given some data in a buffer, determine if we have a received a
/// valid op---one that ends with "\r\n\r\n".  
/// @param[in]  iobuf : buffer containing data
/// @param[out] msgLen : if we do have a valid command, return the length of
/// the command
/// @retval True if we have a valid command; False otherwise.
///
extern bool IsMsgAvail(IOBuffer *iobuf, int *msgLen);

///
/// \brief bomb out on "impossible" error
/// \param[in] msg       panic text
///
extern void die(const string &msg);

#endif // CHUNKSERVER_UTILS_H