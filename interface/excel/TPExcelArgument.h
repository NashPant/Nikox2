#ifndef TP_EXCEL_ARGUMENT_H
#define TP_EXCEL_ARGUMENT_H

#include "TPXLInterfaceUtils.h"
#include <string>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// stores data for a single argument of a single excel function
enum TP_ExcelType
{
    TP_ExcelBool,
    TP_ExcelDouble,
    TP_ExcelString,
    TP_ExcelInt,
    TP_ExcelLong,
    TP_ExcelEmpty
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
class TPXLINTERFACE_IMPEXP TPExcelArgument
{
public:
    TPExcelArgument(TP_ExcelType type = TP_ExcelEmpty, 
                    std::string name = "", 
                    std::string desc = "");

    bool operator==(const TPExcelArgument& rhs) const;
    bool isEmpty() { return myType == TP_ExcelEmpty; }

    // Accessor Functions
    TP_ExcelType  type(void) const {return myType;}
    void          type(TP_ExcelType t) {myType = t;}
    const std::string& name(void) const {return myName;}
    void          name(const std::string& str) {myName = str;}
    const std::string& description(void) const {return myDescription;}
    void          description(const std::string& str) {myDescription = str;}

private:
    TP_ExcelType myType;
    std::string myName;
    std::string myDescription;
};
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// inline member functions
inline
TPExcelArgument::TPExcelArgument(TP_ExcelType type, 
                                 std::string name, 
                                 std::string desc)
	            : myType(type),
                  myName(name),
                  myDescription(desc) 
{
}

///////////////////////////////////////////////////////////////////////////////
inline
bool TPExcelArgument::operator==(const TPExcelArgument& rhs) const
{
    return (   myType == rhs.myType 
            && myName == rhs.myName 
            && myDescription == rhs.myDescription);
}
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#endif