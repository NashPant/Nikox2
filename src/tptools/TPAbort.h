////////////////////////////////////////////////////////////////////////////
//
//  This program is the CONFIDENTIAL and PROPRIETARY property of'
//   Athanasios Pantas. Unauthorized use and reproduction'
//  of this program is strictly prohibited.
//
// Copyright (C) 2003  - Nikopolis
//  Copyright (C) 2003 - Athanasios Pantas.
//  ALL RIGHTS RESERVED.
/////////////////////////////////////////////////////////////////////////////

#ifndef TP_ABORT_H
#define TP_ABORT_H

#include <TPNikopolisUtils.h>
#include <float.h>
#include <string>
#include <cmath>
#include <vector>
#include <list>

//
// Class for the purpose of registering callbacks, e.g. into Excel, in order to enquire if the user has requested to abort a calculation.
//
class TPNIKOPOLIS_IMPEXP TPAbort
{
public:

    TPAbort(bool(*f)(void));
    ~TPAbort();
    static bool abortHasBeenRequested(void);
    static bool isRegistered(bool(*f)(void));

protected:

    static void registerCallBack(bool(*f)(void));
    static void unRegisterCallBack(bool(*f)(void));

private:
    static std::list<bool(*)(void)> ourListOfCallBacks;
    bool (*myf)(void);
};

#endif // TP_ABORT_H
