//#include <glog/logging.h> ✓ 
#include <gtest/gtest.h> 
#include <set>
#include <type_traits>

#include <Eigen/Dense>
#include "tpmaths/TPMatrix.h"
#include <vector>

// Test fixture for TPMatrix tests
class TPMatrixTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize a 2x3 matrix with known values
        testMatrix = TPMatrix<double>(2, 3);
        testMatrix(0, 0) = 1.0; testMatrix(0, 1) = 2.0; testMatrix(0, 2) = 3.0;
        testMatrix(1, 0) = 4.0; testMatrix(1, 1) = 5.0; testMatrix(1, 2) = 6.0;
    }

    TPMatrix<double> testMatrix;
};

TEST_F(TPMatrixTest, DefaultConstructor) {
    TPMatrix<double> m;
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.rows(), 0);
    EXPECT_EQ(m.columns(), 0);
}

TEST_F(TPMatrixTest, ParameterizedConstructor) {
    TPMatrix<double> m(3, 4, 2.5);
    EXPECT_EQ(m.rows(), 3);
    EXPECT_EQ(m.columns(), 4);
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.columns(); ++j) {
            EXPECT_EQ(m(i, j), 2.5);
        }
    }
}

TEST_F(TPMatrixTest, CreateFromRawArray) {
    double raw[] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
    TPMatrix<double> m = TPMatrix<double>::create(raw, 2, 3);
    EXPECT_EQ(m.rows(), 2);
    EXPECT_EQ(m.columns(), 3);
    EXPECT_EQ(m(0, 0), 1.0);
    EXPECT_EQ(m(0, 1), 2.0);
    EXPECT_EQ(m(0, 2), 3.0);
    EXPECT_EQ(m(1, 0), 4.0);
    EXPECT_EQ(m(1, 1), 5.0);
    EXPECT_EQ(m(1, 2), 6.0);
}

TEST_F(TPMatrixTest, CopyConstructor) {
    TPMatrix<double> m(testMatrix);
    EXPECT_EQ(m.rows(), testMatrix.rows());
    EXPECT_EQ(m.columns(), testMatrix.columns());
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.columns(); ++j) {
            EXPECT_EQ(m(i, j), testMatrix(i, j));
        }
    }
}

TEST_F(TPMatrixTest, AssignmentOperator) {
    TPMatrix<double> m;
    m = testMatrix;
    EXPECT_EQ(m.rows(), testMatrix.rows());
    EXPECT_EQ(m.columns(), testMatrix.columns());
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.columns(); ++j) {
            EXPECT_EQ(m(i, j), testMatrix(i, j));
        }
    }
}

TEST_F(TPMatrixTest, AdditionOperator) {
    TPMatrix<double> m = testMatrix;
    m += testMatrix;
    EXPECT_EQ(m.rows(), 2);
    EXPECT_EQ(m.columns(), 3);
    EXPECT_EQ(m(0, 0), 2.0);
    EXPECT_EQ(m(0, 1), 4.0);
    EXPECT_EQ(m(0, 2), 6.0);
    EXPECT_EQ(m(1, 0), 8.0);
    EXPECT_EQ(m(1, 1), 10.0);
    EXPECT_EQ(m(1, 2), 12.0);
}

//TEST_F(TPMatrixTest, DivisionOperator) {
//    TPMatrix<double> m = testMatrix;
//    m /= 2.0;
//    EXPECT_EQ(m(0, 0), 0.5);
//    EXPECT_EQ(m(0, 1), 1.0);
//    EXPECT_EQ(m(0, 2), 1.5);
//    EXPECT_EQ(m(1, 0), 2.0);
//    EXPECT_EQ(m(1, 1), 2.5);
//    EXPECT_EQ(m(1, 2), 3.0);
//}

TEST_F(TPMatrixTest, Transpose) {
    TPMatrix<double> m = testMatrix.transpose();
    EXPECT_EQ(m.rows(), 3);
    EXPECT_EQ(m.columns(), 2);
    EXPECT_EQ(m(0, 0), 1.0);
    EXPECT_EQ(m(0, 1), 4.0);
    EXPECT_EQ(m(1, 0), 2.0);
    EXPECT_EQ(m(1, 1), 5.0);
    EXPECT_EQ(m(2, 0), 3.0);
    EXPECT_EQ(m(2, 1), 6.0);
}

TEST_F(TPMatrixTest, Resize) {
    TPMatrix<double> m;
    m.resize(2, 2, 1.0);
    EXPECT_EQ(m.rows(), 2);
    EXPECT_EQ(m.columns(), 2);
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.columns(); ++j) {
            EXPECT_EQ(m(i, j), 1.0);
        }
    }
}

TEST_F(TPMatrixTest, PushBackRow) {
    TPMatrix<double> m = testMatrix;
    std::vector<double> row = { 7.0, 8.0, 9.0 };
    m.push_back(row, 0);
    EXPECT_EQ(m.rows(), 3);
    EXPECT_EQ(m.columns(), 3);
    EXPECT_EQ(m(2, 0), 7.0);
    EXPECT_EQ(m(2, 1), 8.0);
    EXPECT_EQ(m(2, 2), 9.0);
}

TEST_F(TPMatrixTest, PushBackColumn) {
    TPMatrix<double> m = testMatrix;
    std::vector<double> col = { 7.0, 8.0 };
    m.push_back(col, 1);
    EXPECT_EQ(m.rows(), 2);
    EXPECT_EQ(m.columns(), 4);
    EXPECT_EQ(m(0, 3), 7.0);
    EXPECT_EQ(m(1, 3), 8.0);
}

TEST_F(TPMatrixTest, HasZeroElements) {
    TPMatrix<double> m(2, 2, 0.0);
    EXPECT_TRUE(m.hasZeroElements());
    m(0, 0) = 1.0;
    EXPECT_TRUE(m.hasZeroElements());
    m.setAllElements(1.0);
    EXPECT_FALSE(m.hasZeroElements());
}

//TEST_F(TPMatrixTest, SubMatrixHasZeroElements) {
//    TPMatrix<double> m(3, 3, 1.0);
//    m(1, 1) = 0.0;
//    EXPECT_TRUE(m.subMatrixhasZeroElements(1, 1, 1, 1));
//    EXPECT_FALSE(m.subMatrixhasZeroElements(0, 0, 0, 0));
//    // Test bounds checking
//    EXPECT_THROW(m.subMatrixhasZeroElements(0, 0, 3, 3), std::exception);
//}




TEST_F(TPMatrixTest, ColumnAccess) {
    auto col = testMatrix.column(1);
    EXPECT_EQ(col.size(), 2);
    EXPECT_EQ(col[0], 2.0);
    EXPECT_EQ(col[1], 5.0);
}

TEST_F(TPMatrixTest, SizeQueries) {
    EXPECT_EQ(testMatrix.size(0), 2);
    EXPECT_EQ(testMatrix.size(1), 3);
    EXPECT_EQ(testMatrix.RowNo(), 2);
    EXPECT_EQ(testMatrix.ColNo(), 3);
    EXPECT_EQ(testMatrix.size1(), 2);
    EXPECT_EQ(testMatrix.size2(), 3);
}

TEST_F(TPMatrixTest, Clear) {
    TPMatrix<double> m = testMatrix;
    m.clear();
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.rows(), 0);
    EXPECT_EQ(m.columns(), 0);
}

TEST_F(TPMatrixTest, SetAllElements) {
    TPMatrix<double> m = testMatrix;
    m.setAllElements(42.0);
    for (size_t i = 0; i < m.rows(); ++i) {
        for (size_t j = 0; j < m.columns(); ++j) {
            EXPECT_EQ(m(i, j), 42.0);
        }
    }
}



TEST_F(TPMatrixTest, PushBackInvalidSize) {
    std::vector<double> invalid_row = { 1.0, 2.0 }; // Wrong size
    EXPECT_THROW(testMatrix.push_back(invalid_row, 0), std::exception);
    std::vector<double> invalid_col = { 1.0 }; // Wrong size
    EXPECT_THROW(testMatrix.push_back(invalid_col, 1), std::exception);
}

//TEST_F(TPMatrixTest, InvalidSizeAccess) {
//    EXPECT_THROW(testMatrix.size(2), std::exception);
//}

