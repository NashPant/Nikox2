// TPAbort.cpp
//

#include "TPAbort.h"

///////////////////////////////////////////////////////////////////////////////  
///////////////////////////////////////////////////////////////////////////////

std::list< bool (*)(void)> TPAbort::ourListOfCallBacks;

TPAbort::TPAbort(bool(*f)(void)) : myf(f)
{
    registerCallBack(f);
}

TPAbort::~TPAbort()
{
    unRegisterCallBack(myf);
}

void
TPAbort::registerCallBack(bool(*f)(void))
{
    if (! isRegistered(f))
        ourListOfCallBacks.push_back(f);
}

void
TPAbort::unRegisterCallBack(bool(*f)(void))
{
    std::list <bool(*)(void)>::iterator i = ourListOfCallBacks.begin();
    while ( i != ourListOfCallBacks.end() )
    {
        if (*i == f)
            i = ourListOfCallBacks.erase(i);
        else
            ++i;
    }
}

bool
TPAbort::isRegistered(bool(*f)(void))
{
    std::list<bool(*)(void)>::const_iterator i = ourListOfCallBacks.begin();
    while ( i != ourListOfCallBacks.end() )
    {
        if (*i == f)
            return true;
        ++i;
    }
    return false;
}

bool
TPAbort::abortHasBeenRequested(void)
{
    std::list<bool(*)(void)>::const_iterator i = ourListOfCallBacks.begin();
    while ( i != ourListOfCallBacks.end() )
    {
        if ((*i)())
            return true;
        ++i;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
