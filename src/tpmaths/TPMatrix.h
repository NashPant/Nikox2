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

#ifndef TP_MATRIX_H
#define TP_MATRIX_H

//#include "TPToolsUtils.h"
#include "../TpTools/TPMathException.h"
#include <vector>
#include <memory>


template <class T>
class TPMatrix
{
public:
    // typedef
    typedef typename T* iterator;
    typedef const typename  T* const_iterator;
    // Constructors.
    TPMatrix();
    TPMatrix(size_t n0, size_t n1, T val = T());
    ~TPMatrix()
    {
        for (size_t i = 0; i < size(0); ++i)
        {
            if (!(myData[i].empty()))
                myData[i].clear();
        }

        if (!(myData.empty()))
            myData.clear();
    }

    /*
      @brief. Initialise with row C-contiguous array as it comes from python
    */
    static TPMatrix<T> create(const T* rawarray, size_t rows, size_t columns);


    // Copy, assignment and clone.
    TPMatrix(const TPMatrix<T>& rhs);
    TPMatrix<T>& operator=(const TPMatrix<T>& rhs);
    TPMatrix<T>& operator+=(const TPMatrix<T>& rhs);
    TPMatrix<T>& operator/= (const T& c);
    TPMatrix<T>* clone(void) const;

    // Functions.
    bool    empty(void) const;
    void    clear(void);
    void    resize(size_t n0, size_t n1, T val = T());
    size_t  size(size_t dir) const;
    void    setAllElements(const T& t);
    void    zeroAllElements(void);

    void push_back(const std::vector<T>& slice, size_t dir);

    //
    bool hasZeroElements() const;
    // will check for zero element in submatrix Matrix(i1:i2, r1:r2) inclusive
    bool subMatrixhasZeroElements(const size_t& r1, const size_t& c1, const size_t& r2, const size_t& c2);

    // Accessors.
    const std::vector<std::vector<T> >& data(void) const;
    const T& at(size_t pos0, size_t pos1) const;
    T& at(size_t pos0, size_t pos1);
    T& operator()(size_t i, size_t j) { return at(i, j); }

    // Simple transpose
    TPMatrix<T> transpose() const;

    //! \name Iterator access
    //@{
    const_iterator begin() const;
    iterator begin();
    const_iterator end() const;
    iterator end();

    //
  //  T*	 pointerdata(void)	{ return (*myData.begin()).begin(); }
    //T*	 pointerdata(void) { return &myData[0][0]; }
    T* pointerdata(void) { return myData.data()->data(); }
    /// Double pointer to the array
    //const T*	 pointerdata(void)	const {return (*myData.begin()).begin();}
    const T* pointerdata(void)	const { return myData.data()->data(); }
    // Operators.
    /// This will basically will return pos-row
    const std::vector<T>& operator[](size_t pos) const { return myData[pos]; }
    std::vector<T>& operator[](size_t pos) { return myData[pos]; }

    /// Access Columns
    /// It will return pos-column
    std::vector<T> column(size_t pos) const;
    //	std::vector<T>& column(siz_t pos);

    size_t RowNo() const { return myData.size(); }
    size_t ColNo() const { return (RowNo() == 0) ? 0 : (*myData.begin()).size(); }

    // to match some quantlib functions
    size_t rows() const { return  myData.size(); }
    size_t columns() const { return (RowNo() == 0) ? 0 : (*myData.begin()).size(); }
    size_t size1() const { return rows(); }
    size_t size2() const { return columns(); }

private:
    // Data.
    std::vector<std::vector<T> >   myData;

    // Housekeeping.
    void copyFrom(const TPMatrix<T>& rhs);
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


template <class T>
inline
TPMatrix<T>
TPMatrix<T>::create(const T* rawarray, size_t rows, size_t cols)
{
    TPMatrix m;
    // we traverse rows and for each row we got to the appropriate element
    for (auto i = 0; i < rows; ++i)
    {
        std::vector<T> row; row.assign(rawarray + i * cols, rawarray + i * cols + cols);
        m.myData.emplace_back(row);
    }

    return m;
}

template <class T>
inline
TPMatrix<T>::TPMatrix()
{
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
inline
TPMatrix<T>::TPMatrix(size_t n0, size_t n1, T val)
{
    resize(n0, n1, val);
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
inline
TPMatrix<T>::TPMatrix(const TPMatrix<T>& rhs)
{
    copyFrom(rhs);
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
inline
TPMatrix<T>& TPMatrix<T>::operator=(const TPMatrix<T>& rhs)
{
    if (this != &rhs)
    {
        copyFrom(rhs);
    }
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
inline
TPMatrix<T>& TPMatrix<T>::operator+=(const TPMatrix<T>& rhs)
{
    //TP_ASSERT( size(0) == rhs.size(0) && size(1) == rhs.size(1) );
    unsigned long i, j, m = size(0), n = size(1);
    for (i = 0; i < m; ++i)
        for (j = 0; j < n; ++j)
            myData[i][j] += rhs.myData[i][j];
    return *this;
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
inline
TPMatrix<T>* TPMatrix<T>::clone(void) const
{
    return new TPMatrix<T>(*this);
}

template <class T>
inline typename TPMatrix<T>::const_iterator TPMatrix<T>::begin() const {
    return this->pointerdata();
}

template <class T>
inline typename TPMatrix<T>::iterator TPMatrix<T>::begin() {
    return this->pointerdata();
}

template <class T>
inline typename TPMatrix<T>::const_iterator TPMatrix<T>::end() const {
    return this->pointerdata() + this->RowNo() * this->ColNo();
}

template <class T>
inline typename TPMatrix<T>::iterator TPMatrix<T>::end() {
    return this->pointerdata() + this->RowNo() * this->ColNo();
}



///////////////////////////////////////////////////////////////////////////////
template <class T>
inline
void TPMatrix<T>::copyFrom(const TPMatrix<T>& rhs)
{
    myData = rhs.myData;
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
inline
bool TPMatrix<T>::empty(void) const
{
    bool rtn = myData.empty();

    if (!rtn)
    {
        rtn &= myData[0].empty();
    }

    return rtn;
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
inline
void TPMatrix<T>::clear(void)
{
    myData.clear();
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
inline
void TPMatrix<T>::resize(size_t n0, size_t n1, T val)
{
    myData.resize(n0);

    for (size_t i = 0; i < size(0); ++i)
    {
        myData[i].resize(n1, val);
    }
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
size_t TPMatrix<T>::size(size_t dir) const
{
    //TP_ASSERT( dir == 0 || dir == 1 );

    size_t rtn = 0;

    if (!empty())
    {
        if (dir == 0)
        {
            rtn = myData.size();
        }
        else
        {
            rtn = myData[0].size();
        }
    }

    return rtn;
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
void TPMatrix<T>::push_back(const std::vector<T>& slice, size_t dir)
{
    //TP_ASSERT( dir == 0 || dir == 1 );

    if (dir == 0)
    {
        // A 0-slice.
        // - tensor-1 and slice must match.
        if (empty())
        {
            resize(1, slice.size());
        }
        else if ((slice.size() == size(1)))
        {
            resize(size(0) + 1, size(1));
        }
        else
        {
            THROW("TPMatrix: The slice and tensor sizes must match.");
        }

        for (size_t i = 0; i < size(1); ++i)
        {
            myData[size(0) - 1][i] = slice[i];
        }
    }
    else
    {
        // A 1-slice.
        // - tensor-0 and slice must match.
        if (empty())
        {
            resize(slice.size(), 1);
        }
        else if (slice.size() == size(0))
        {
            resize(size(0), size(1) + 1);
        }
        else
        {
            THROW("TPMatrix: The slice and tensor sizes must match.");
        }

        for (size_t i = 0; i < size(0); ++i)
        {
            myData[i][size(1) - 1] = slice[i];
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
inline
const std::vector<std::vector<T> >& TPMatrix<T>::data(void) const
{
    return myData;
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
inline
const T& TPMatrix<T>::at(size_t pos0, size_t pos1) const
{
    return myData[pos0][pos1];
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
inline
T& TPMatrix<T>::at(size_t pos0, size_t pos1)
{
    return myData[pos0][pos1];
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
inline
void TPMatrix<T>::setAllElements(const T& t)
{
    unsigned long i, j, m = size(0), n = size(1);
    for (i = 0; i < m; ++i)
        for (j = 0; j < n; ++j)
            myData[i][j] = t;
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
inline
void TPMatrix<T>::zeroAllElements(void)
{
    unsigned long i, j, m = size(0), n = size(1);
    for (i = 0; i < m; ++i)
        for (j = 0; j < n; ++j)
            myData[i][j] = 0;
}

///////
//  Check if the matrix has Zero Elements.
////////////////////////////////////
template <class T>
inline
bool  TPMatrix<T>::hasZeroElements() const
{
    unsigned long i, j, m = size(0), n = size(1);
    bool haszero = false;
    for (i = 0; i < m; ++i)
        for (j = 0; j < n; ++j)
            if (myData[i][j] == 0) return true;

    return haszero;
}

///////
//  Same thing but for a sub-matrix as well.
////////////////////////////////////

template <class T>
inline
bool  TPMatrix<T>::subMatrixhasZeroElements(const size_t& r1, const size_t& c1, const size_t& r2, const size_t& c2)
{
    unsigned long i, j, m = size(0), n = size(1);
    //
    //
    TP_MATH_REQUIRE(r1 >= 0, "subMatrixhasZeroElements::Lower Index outside bounds", 9, false);
    TP_MATH_REQUIRE(r2 < m, "subMatrixhasZeroElements::Upper Index outside bounds", 9, false);
    TP_MATH_REQUIRE(c1 >= 0, "subMatrixhasZeroElements::Lower Index outside bounds", 9, false);
    TP_MATH_REQUIRE(c2 < n, "subMatrixhasZeroElements::Upper Index outside bounds", 9, false);
    //
    //
    //if(r1 < 0) r1=0;
    //if(r2 >= m) r2 = m-1;
    //if(c1 < 0) c1=0;
    //if(c2 >= n) c2 =n-1 ;

    bool haszero = false;
    for (i = r1; i <= r2; ++i)
        for (j = c1; j <= c2; ++j)
            if (myData[i][j] == 0) return true;

    return haszero;
}

template <class T>
inline
std::vector<T> TPMatrix<T>::column(size_t pos) const
{
    // 
    std::vector<T>	res;
    typename std::vector< std::vector<T> >::const_iterator it = myData.begin();
    for (; it != myData.end(); it++)
        res.push_back((*it)[pos]);

    return res;
}

template <class T>
inline
TPMatrix<T>& TPMatrix<T>::operator /= (const T& c)
{
    unsigned long i, j, m = size(0), n = size(1);
    for (i = 0; i < m; ++i)
        for (j = 0; j < n; ++j)
            myData[i][j] /= c;
    return *this;
}


// Internal tranpose
template <class T>
TPMatrix<T> TPMatrix<T>::transpose() const
{
    TPMatrix<T> temp(this->ColNo(), this->RowNo());

    for (size_t i = 0; i < this->RowNo(); i++)
        for (size_t j = 0; j < this->ColNo(); j++)
        {
            T x = this->at(i, j);
            temp[j][i] = x;
        }
    return temp;
}

/*
// unary transpose operator
template <class T>
inline TPMatrix<T>
operator ^(const TPMatrix<T>& m)
{
    TPMatrix<T> temp(m.ColNo(), m.RowNo());

    for (size_t i = 0; i < m.RowNo(); i++)
        for (size_t j = 0; j < m.ColNo(); j++)
        {
            T x = m[i][j];
            temp[j][i] = x;
        }
    return temp;
}
*/

#endif 
