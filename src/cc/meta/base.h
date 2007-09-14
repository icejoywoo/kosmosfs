/*!
 * $Id: //depot/SOURCE/OPENSOURCE/kfs/src/cc/meta/base.h#3 $
 *
 * \file base.h
 * \brief Base class for KFS metadata nodes.
 * \author Blake Lewis (Kosmix Corp.)
 *
 * Copyright (C) 2006 Kosmix Corp.
 *
 * This file is part of Kosmos File System (KFS).
 *
 * KFS is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation under version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#if !defined(KFS_BASE_H)
#define KFS_BASE_H

#include <string>
#include "kfstypes.h"

using std::string;

namespace KFS {

typedef long long KeyData;	//!< "opaque" key data

/*!
 * \brief search key
 *
 * Key values for tree operations.  Q: does this need to
 * be polymorphic?
 */
class Key {
	MetaType kind;	//!< for what kind of metadata
	KeyData kdata1;	//!< associated identification
	KeyData kdata2; //!< and more identification
public:
	static const KeyData MATCH_ANY = -1;
	Key(MetaType k, KeyData d): kind(k), kdata1(d), kdata2(0) { }
	Key(MetaType k, KeyData d1, KeyData d2):
		kind(k), kdata1(d1), kdata2(d2) { }
	Key(): kind(KFS_UNINIT), kdata1(0), kdata2(0) { }
	int compare(const Key &test) const;
	bool operator < (const Key &test) const { return compare(test) < 0; }
	bool operator == (const Key &test) const { return compare(test) == 0; }
	bool operator != (const Key &test) const { return compare(test) != 0; }
};

// MetaNode flag values
static const int META_CPBIT = 1;//!< CP parity bit
static const int META_NEW = 2;	//!< new since start of CP
static const int META_ROOT = 4;	//!< root node
static const int META_LEVEL1 = 8; //!< children are leaves
static const int META_SKIP = 16; //!< exclude from current CP

/*!
 * \brief base class for both internal and leaf nodes
 */
class MetaNode {
	MetaType type;
	int flagbits;
public:
	MetaNode(MetaType t): type(t), flagbits(0) { }
	MetaNode(MetaType t, int f): type(t), flagbits(f) { }
	virtual ~MetaNode() { }
	virtual const Key key() const = 0;	//!< cons up key value for node
	virtual const string show() const = 0;	//!< print out contents
	int flags() const { return flagbits; }
	void setflag(int bit) { flagbits |= bit; }
	void clearflag(int bit) { flagbits &= ~bit; }
	bool testflag(int bit) const { return (flagbits & bit) != 0; }
};

}
#endif	// !defined(KFS_BASE_H)