////////////////////////////////////////////////////////////////////////////
//
//  This program is the  property of'
//   Athanasios Pantas. Feel free to use, but no guarantees from me!
//  
//
// Copyright (C) 2003  - Nikopolis
//  Copyright (C) 2003 - Athanasios Pantas.
//  ALL RIGHTS RESERVED.
/////////////////////////////////////////////////////////////////////////////

#ifndef TP_IDENTIFIER
#define TP_IDENTIFIER


#include "TPGlobals.h"
#include <string>

class TPIdentifier
{

public:
		TPIdentifier():string_(),version_(0){}; 
        TPIdentifier(const TPIdentifier& rhs);
        TPIdentifier(const std::string& s);
		TPIdentifier(const std::string& str, unsigned ver):string_(str),version_(ver){}
        ~TPIdentifier(){}

        const TPIdentifier& operator=(const TPIdentifier& rhs);

        std::string          getIdentifier(void) const;
        TPIdentifier    nextVersion(void) const;


        unsigned        getVersion(void) const          {return version_;}
        void            setVersion(unsigned version)    {version_ = version;}

        const std::string&   getString(void) const           {return string_;}
        void            setString(const std::string& s)      {string_ = s;}

		bool operator==(const TPIdentifier& rhs) const;
		bool operator<(const TPIdentifier& rhs) const;
      
private:
        std::string          string_;
        unsigned        version_;
};

inline bool TPIdentifier::operator==(const TPIdentifier& rhs) const
{
	if ( (version_ == rhs.version_) && (string_ == rhs.string_)) return true;
	else return false;
}

inline bool TPIdentifier::operator<(const TPIdentifier& rhs) const
{


	return (string_ < rhs.string_);
/*
#ifdef WIN32
	return _stricmp(string_.c_str(),rhs.string_.c_str()) < 0 ; 
#else
	return strcasecmp(string_.c_str(),rhs.string_.c_str()) < 0 ; 
#endif
*/
}

inline const TPIdentifier& TPIdentifier::operator=(const TPIdentifier& rhs)
{
	if (this != &rhs)
	{
		string_  = rhs.string_;
        version_ = rhs.version_;
	}
	return *this;
}


inline TPIdentifier::TPIdentifier(const TPIdentifier& rhs)
{
        string_  = rhs.string_;
        version_ = rhs.version_;
}

inline TPIdentifier::TPIdentifier(const std::string& s)
{
  if(!s.empty())
  {
    size_t	pos = s.find_last_of('.');
    if (pos == std::string::npos)
    {
        string_ = s;
	    version_ = 0;
    }
    else
    {
	    string_ = s.substr(0,pos);
	    version_ = atoi(s.substr(pos, s.size()-pos).data());
    }
  }
}

inline TPIdentifier  TPIdentifier::nextVersion(void) const
{
        return  TPIdentifier(getString(), getVersion()+1);
}

inline std::string TPIdentifier::getIdentifier(void) const
{
	//return getString()+"."+ ::ToString((int)getVersion());
	return getString() + "." + std::to_string(getVersion());
}


#endif
