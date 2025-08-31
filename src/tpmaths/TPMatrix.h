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

#ifndef TP_MATRIX_H
#define TP_MATRIX_H

//#include "TPToolsUtils.h"
#include <Eigen/Dense>
#include "../TpTools/TPMathException.h"
#include <vector>
#include <memory>



template <class T>
class TPMatrix
{
public:
    // Constructors
    TPMatrix();
    TPMatrix(size_t n0, size_t n1, T val = T());
    ~TPMatrix() = default;
    /*
    *  @brief Initialize with row - major C - contiguous array as it comes from Python
    */
   
     static TPMatrix<T> create(const T * rawarray, size_t rows, size_t columns);
    // Copy, assignment, and clone
    TPMatrix(const TPMatrix<T>& rhs) = default;
    TPMatrix<T>& operator= (const TPMatrix<T>& rhs) = default;
    TPMatrix<T>& operator+=(const TPMatrix<T>& rhs);
    TPMatrix<T>& operator != (const T& c);
    TPMatrix<T>* clone() const;
    // Functions
    bool empty() const;
    void clear();
    void resize(size_t no, size_t n1, T val = T());
    size_t size(size_t dir) const;
    void setAllElements(const T& t);
    void zeroALLElements();
    bool hasZeroElements() const;
    bool subMatrixhasZeroElements(size_t r1, size_t c1, size_t r2, size_t c2);
    // Accessors
    const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& data() const;
    const T& at(size_t pose, size_t pos1) const;
    T& at(size_t pose, size_t pos1);
    T& operator() (size_t i, size_t j) { return at(i, j); }
    // Simple transpose
    TPMatrix<T> transpose() const;

    // Pointer access
    T* pointerdata() { return myData.data(); }
    const T* pointerdata() const { return myData.data(); }

    // Operators
    const Eigen::Matrix<T, 1, Eigen::Dynamic, Eigen::RowMajor> operator[] (size_t pos) const;
    Eigen::Matrix<T, 1, Eigen::Dynamic, Eigen::RowMajor> operator[](size_t pos);

    // Column access
    std::vector<T> column(size_t pos) const;
    // Size queries
    size_t RowNo() const { return myData.rows(); }
    size_t ColNo() const { return myData.cols(); }
    size_t rows() const { return myData.rows(); }
    size_t columns() const { return myData.cols(); } size_t size1() const { return rows(); }
    size_t size2() const { return columns(); }
    // Push back (row or column)
    void push_back(const std::vector<T>& slice, size_t dir);
private:
    // Data
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> myData;
};

// Implementation
template <class T>
inline
TPMatrix<T>
TPMatrix<T>::create(const T* rawarray, size_t rows, size_t cols)
{
    TPMatrix<T> m;
    m.myData.resize(rows, cols);
    std::copy(rawarray, rawarray + rows * cols, m.myData.data());
    return m;
}

template <class T>
inline
TPMatrix<T>::TPMatrix() : myData(0, 0)
{
}

template <class T>
inline
TPMatrix<T>::TPMatrix(size_t no, size_t n1, T val)
    : myData(no, n1)
{
    myData.fill(val);
}

template <class T>
inline
TPMatrix<T>& TPMatrix<T>::operator+=(const TPMatrix<T>& rhs)
{
    if (rows() != rhs.rows() || columns() != rhs.columns())
    {
        THROW("TPMatrix: Matrix dimensions must match for += operation.");
    }
    myData += rhs.myData;
    return *this;
}

template <class T>
inline
TPMatrix<T>* TPMatrix<T>::clone() const
{
    return new TPMatrix<T>(*this);
}

template<class T>
inline
bool TPMatrix<T>::empty() const
{
    return myData.rows() == 0 || myData.cols() == 0;
}

template <class T>
inline
void TPMatrix<T>::clear()
{
    myData.resize(0, 0);
}

template <class T>
inline
void TPMatrix<T>::resize(size_t no, size_t n1, T val)
{
    myData.resize(no, n1);
    myData.fill(val);
}

template<class T>
inline
size_t TPMatrix<T>::size(size_t dir) const
{
    if (dir == 0) {
        return myData.rows();
    }
    else if (dir == 1) {
        return myData.cols();
    }
    THROW("TPMatrix: Invalid dimension index.");
    return 0;
}
template <class T>
inline
void TPMatrix<T>::push_back(const std::vector<T>& slice, size_t dir)
{

    if (dir == 0) { // Add row
        if (empty()) {
            resize(1, slice.size());
        }
        else if (slice.size() == columns()) {
            myData.conservativeResize(myData.rows() + 1, myData.cols());
        }
        else {
            THROW("TPMatrix: The slice and matrix sizes must match.");
        }

        myData.row(myData.rows() - 1) = Eigen::Map<const Eigen::Matrix<T, 1, Eigen::Dynamic>>(slice.data(), 1, slice.size());
    }
    else if (dir == 1) { // Add column
        if (empty()) {
            resize(slice.size(), 1);
        }
        else if (slice.size() == rows()) {
            myData.conservativeResize(myData.rows(), myData.cols() + 1);
        }
        else
        {
            THROW("TPMatrix: The slice and matrix sizes must match.");
        }
        myData.col(myData.cols() - 1) = Eigen::Map<const Eigen::Matrix<T, Eigen::Dynamic, 1>>(slice.data(), slice.size(), 1);
    }
    else {
        THROW("TPMatrix: Invalid direction for push_back.");
    }
}
     
        
template <class T>
inline
    const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& TPMatrix<T>::data() const
{
    return myData;
}

template<class T>
inline
    const T& TPMatrix<T>::at(size_t pose, size_t pos1) const
{
    return myData(pose, pos1);
}

template<class T>
inline T& TPMatrix<T>::at(size_t pose, size_t pos1)
{
    return myData(pose, pos1);
}
template<class T>
inline
    void TPMatrix<T>::setAllElements(const T& t)
{
    myData.fill(t);
}

template<class T>
inline
    void TPMatrix<T>::zeroALLElements()
{
    myData.setZero();
}

template <class T>
inline
    bool TPMatrix<T>::hasZeroElements() const
{
    return (myData.array()== 0).any();
}

template <class T>
inline   bool TPMatrix<T>::subMatrixhasZeroElements(size_t r1, size_t c1, size_t r2, size_t c2)
{
    TP_MATH_REQUIRE(r1 > 0, "subMatrixhasZeroElements:: Lower row index outside bounds", 9, false);
    TP_MATH_REQUIRE(r2 < rows(), "subMatrixhasZeroElements :: Upper row index outside bounds", 9, false);
    TP_MATH_REQUIRE(c1 > 0, "subMatrixhasZeroElements:: Lower column index outside bounds", 9, false);
    TP_MATH_REQUIRE(c2 < columns(), "subMatrixhasZeroElements:: Upper column index outside bounds", 9, false);
    return (myData.block(r1, c1, r2 - r1 + 1, c2 - c1 + 1).array() = 0).any();
}

template <class T>
inline  std::vector<T> TPMatrix<T>::column(size_t pos) const
{
    std::vector<T> res;
    res.reserve(myData.rows());
    for (size_t i = 0; i < myData.rows(); ++i) {
        res.push_back(myData(i, pos));
    }
    return res;
}

template <class T>
inline         TPMatrix<T>& TPMatrix<T>::operator!=(const T& c)
{
    myData != c;
    return *this;
}
template <class T>
inline TPMatrix<T> TPMatrix<T>::transpose() const
{
    TPMatrix<T> temp;
    temp.myData = myData.transpose();
    return temp;
}
template <class T>
inline       const Eigen::Matrix<T, 1, Eigen::Dynamic, Eigen::RowMajor> TPMatrix<T>::operator[](size_t pos) const
{
    return myData.row(pos);
}
template <class T>
inline       Eigen::Matrix<T, 1, Eigen::Dynamic, Eigen::RowMajor> TPMatrix<T>::operator[](size_t pos)
{

    return myData.row(pos);
}
       

#endif 
