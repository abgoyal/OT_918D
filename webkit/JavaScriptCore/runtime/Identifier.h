/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

/*
 *  Copyright (C) 2003, 2006, 2007, 2008, 2009 Apple Inc. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef Identifier_h
#define Identifier_h

#include "JSGlobalData.h"
#include "ThreadSpecific.h"
#include "UString.h"

namespace JSC {

    class ExecState;

    class Identifier {
        friend class Structure;
    public:
        Identifier() { }

        Identifier(ExecState* exec, const char* s) : _ustring(add(exec, s)) { } // Only to be used with string literals.
        Identifier(ExecState* exec, const UChar* s, int length) : _ustring(add(exec, s, length)) { }
        Identifier(ExecState* exec, UString::Rep* rep) : _ustring(add(exec, rep)) { } 
        Identifier(ExecState* exec, const UString& s) : _ustring(add(exec, s.rep())) { }

        Identifier(JSGlobalData* globalData, const char* s) : _ustring(add(globalData, s)) { } // Only to be used with string literals.
        Identifier(JSGlobalData* globalData, const UChar* s, int length) : _ustring(add(globalData, s, length)) { }
        Identifier(JSGlobalData* globalData, UString::Rep* rep) : _ustring(add(globalData, rep)) { } 
        Identifier(JSGlobalData* globalData, const UString& s) : _ustring(add(globalData, s.rep())) { }

        // Special constructor for cases where we overwrite an object in place.
        Identifier(PlacementNewAdoptType) : _ustring(PlacementNewAdopt) { }
        
        const UString& ustring() const { return _ustring; }
        
        const UChar* data() const { return _ustring.data(); }
        int size() const { return _ustring.size(); }
        
        const char* ascii() const { return _ustring.ascii(); }
        
        static Identifier from(ExecState* exec, unsigned y) { return Identifier(exec, UString::from(y)); }
        static Identifier from(ExecState* exec, int y) { return Identifier(exec, UString::from(y)); }
        static Identifier from(ExecState* exec, double y) { return Identifier(exec, UString::from(y)); }
        
        bool isNull() const { return _ustring.isNull(); }
        bool isEmpty() const { return _ustring.isEmpty(); }
        
        uint32_t toUInt32(bool* ok) const { return _ustring.toUInt32(ok); }
        uint32_t toUInt32(bool* ok, bool tolerateEmptyString) const { return _ustring.toUInt32(ok, tolerateEmptyString); };
        uint32_t toStrictUInt32(bool* ok) const { return _ustring.toStrictUInt32(ok); }
        unsigned toArrayIndex(bool* ok) const { return _ustring.toArrayIndex(ok); }
        double toDouble() const { return _ustring.toDouble(); }
        
        friend bool operator==(const Identifier&, const Identifier&);
        friend bool operator!=(const Identifier&, const Identifier&);

        friend bool operator==(const Identifier&, const char*);
        friend bool operator!=(const Identifier&, const char*);
    
        static void remove(UString::Rep*);

        static bool equal(const UString::Rep*, const char*);
        static bool equal(const UString::Rep*, const UChar*, int length);
        static bool equal(const UString::Rep* a, const UString::Rep* b) { return JSC::equal(a, b); }

        static PassRefPtr<UString::Rep> add(ExecState*, const char*); // Only to be used with string literals.
        static PassRefPtr<UString::Rep> add(JSGlobalData*, const char*); // Only to be used with string literals.

    private:
        UString _ustring;
        
        static bool equal(const Identifier& a, const Identifier& b) { return a._ustring.rep() == b._ustring.rep(); }
        static bool equal(const Identifier& a, const char* b) { return equal(a._ustring.rep(), b); }

        static PassRefPtr<UString::Rep> add(ExecState*, const UChar*, int length);
        static PassRefPtr<UString::Rep> add(JSGlobalData*, const UChar*, int length);

        static PassRefPtr<UString::Rep> add(ExecState* exec, UString::Rep* r)
        {
            if (r->isIdentifier()) {
#ifndef NDEBUG
                checkSameIdentifierTable(exec, r);
#endif
                return r;
            }
            return addSlowCase(exec, r);
        }
        static PassRefPtr<UString::Rep> add(JSGlobalData* globalData, UString::Rep* r)
        {
            if (r->isIdentifier()) {
#ifndef NDEBUG
                checkSameIdentifierTable(globalData, r);
#endif
                return r;
            }
            return addSlowCase(globalData, r);
        }

        static PassRefPtr<UString::Rep> addSlowCase(ExecState*, UString::Rep* r);
        static PassRefPtr<UString::Rep> addSlowCase(JSGlobalData*, UString::Rep* r);

        static void checkSameIdentifierTable(ExecState*, UString::Rep*);
        static void checkSameIdentifierTable(JSGlobalData*, UString::Rep*);
    };
    
    inline bool operator==(const Identifier& a, const Identifier& b)
    {
        return Identifier::equal(a, b);
    }

    inline bool operator!=(const Identifier& a, const Identifier& b)
    {
        return !Identifier::equal(a, b);
    }

    inline bool operator==(const Identifier& a, const char* b)
    {
        return Identifier::equal(a, b);
    }

    inline bool operator!=(const Identifier& a, const char* b)
    {
        return !Identifier::equal(a, b);
    }

    IdentifierTable* createIdentifierTable();
    void deleteIdentifierTable(IdentifierTable*);

    struct ThreadIdentifierTableData {
        ThreadIdentifierTableData()
            : defaultIdentifierTable(0)
            , currentIdentifierTable(0)
        {
        }

        IdentifierTable* defaultIdentifierTable;
        IdentifierTable* currentIdentifierTable;
    };

    extern WTF::ThreadSpecific<ThreadIdentifierTableData>* g_identifierTableSpecific;
    void createIdentifierTableSpecific();

    inline IdentifierTable* defaultIdentifierTable()
    {
        if (!g_identifierTableSpecific)
            createIdentifierTableSpecific();
        ThreadIdentifierTableData& data = **g_identifierTableSpecific;

        return data.defaultIdentifierTable;
    }

    inline void setDefaultIdentifierTable(IdentifierTable* identifierTable)
    {
        if (!g_identifierTableSpecific)
            createIdentifierTableSpecific();
        ThreadIdentifierTableData& data = **g_identifierTableSpecific;

        data.defaultIdentifierTable = identifierTable;
    }

    inline IdentifierTable* currentIdentifierTable()
    {
        if (!g_identifierTableSpecific)
            createIdentifierTableSpecific();
        ThreadIdentifierTableData& data = **g_identifierTableSpecific;

        return data.currentIdentifierTable;
    }

    inline IdentifierTable* setCurrentIdentifierTable(IdentifierTable* identifierTable)
    {
        if (!g_identifierTableSpecific)
            createIdentifierTableSpecific();
        ThreadIdentifierTableData& data = **g_identifierTableSpecific;

        IdentifierTable* oldIdentifierTable = data.currentIdentifierTable;
        data.currentIdentifierTable = identifierTable;
        return oldIdentifierTable;
    }

    inline void resetCurrentIdentifierTable()
    {
        if (!g_identifierTableSpecific)
            createIdentifierTableSpecific();
        ThreadIdentifierTableData& data = **g_identifierTableSpecific;

        data.currentIdentifierTable = data.defaultIdentifierTable;
    }

} // namespace JSC

#endif // Identifier_h
