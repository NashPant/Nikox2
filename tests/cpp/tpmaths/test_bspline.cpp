#include <gsl/gsl_linalg.h> 
#include <gtest/gtest.h>
#include <tpmaths/interpolation/curve.h>
#include <vector>


using namespace TP::maths;



TEST(CurveOrder1, InRangeInterpolation)
{
	const std::vector<double> xs1{ 0.0, 1.0, 2.0 };
	const std::vector<double> ys1{ 0.0, 10.0, 20.0 };
	BSCurve<double, 1> c(xs1, ys1);
	// at 0.5 -> 5, at 1.5 -> 15 
	EXPECT_DOUBLE_EQ(c.value(0.5), 5.0);
	EXPECT_DOUBLE_EQ(c.value(1.5), 15.0);
}
TEST(CurveOrder1, ExtrapolateLeftAndRight)
{
	const std::vector<double> xs1{ 0.0, 1.0, 2.0 }; const std::vector<double> ys1{ 0.0, 10.0, 20.0 };
	BSCurve<double, 1> c(xs1, ys1);
	// slope	= 10
	EXPECT_NEAR(c.value(-1.0), 0.0 + 10.0 * (-1.0 - 0.0), 1e-12);
	EXPECT_NEAR(c.value(3.0), 20.0 + 10.0 * (3.0 - 2.0), 1e-12);
}
TEST(CurveOrder1, FlatExtrapolateLeftAndRight)
{
	const std::vector<double> xs1{ 0.0, 1.0, 2.0 }; 
	const std::vector<double> ys1{ 0.0, 10.0, 20.0 };
	BSCurve<double, 1> c(
		xs1, ys1, ExtrapolationType::CONSTANT, ExtrapolationType::CONSTANT);
	EXPECT_NEAR(c.value(-1.0), 0.0, 1e-12);
	EXPECT_NEAR(c.value(3.0), 20.0, 1e-12);
}


TEST(CurveOrder2, InRangeExactQuadratic)
{
	const std::vector<double> xs2{ 0.0, 1.0, 2.0 };
	const std::vector<double> ys2{ 0.0, 1.0, 4.0 };
	BSCurve<double, 2> c(xs2, ys2, ExtrapolationType::CONSTANT, ExtrapolationType::LINEAR);

	// Verify it exactly hits the node values 
	EXPECT_NEAR(c.value(0.0), 0.0, 1e-12);
	EXPECT_NEAR(c.value(1.0), 1.0, 1e-12);
	EXPECT_NEAR(c.value(2.0), 4.0, 1e-12);
	// Quadratic interpolation of x^2 is exact on these nodes 
	EXPECT_NEAR(c.value(0.5), 0.25, 1e-12);
	EXPECT_NEAR(c.value(1.5), 2.25, 1e-12);
	std::array<Boundary<double>, 1> boundaries{	{TP::maths::Node::FIRST, 1, 0.0 }
	};

	BSCurve<double, 2> c2(xs2, ys2, boundaries);

	// Verify it exactly hits the node values 
	EXPECT_NEAR(c2.value(0.0), 0.0, 1e-12);
	EXPECT_NEAR(c2.value(1.0), 1.0, 1e-12);
	EXPECT_NEAR(c2.value(2.0), 4.0, 1e-12);

	// Quadratic interpolation of x^2 is exact on these nodes
	EXPECT_NEAR(c2.value(0.5), 0.25, 1e-12);
	EXPECT_NEAR(c2.value(1.5), 2.25, 1e-12);
}
	
TEST(CurveOrder2, ExtrapolateDefaultLinear)
{
	const std::vector<double> xs2{ 0.0, 1.0, 2.0 };
	const std::vector<double> ys2{ 0.0, 1.0, 4.0 };
	std::array<Boundary<double>, 1> boundaries{
		{TP::maths::Node::FIRST, 1, 0.0 }
	};
	BSCurve<double, 2> c(xs2, ys2, boundaries);
	// Derivative at ends of x^2: at 0 -> 0, at 2 -> 4
	EXPECT_NEAR(c.value(-1.0), 0.0 + 0.0 * (-1.0 - 0.0), 1e-12);
	EXPECT_NEAR(c.value(3.0), 4.0 + 4.0 * (3.0 - 2.0), 1e-12);
}
TEST(CurveOrder3, InRangeExactCubic)
{
	const std::vector<double> xs3{ 0.0, 1.0, 2.0, 3.0 };
	const std::vector<double> ys3{ 0.0, 1.0, 8.0, 27.0 };
	Boundary<double> b1{Node::FIRST, 2, 0.0 };
	Boundary<double> b2{Node::LAST, 2, 18.0 };
	std::array<Boundary<double>, 2> boundaries{ b1, b2 };
	BSCurve<double, 3> c(xs3, ys3, boundaries);
	// Verify it exactly hits the node values EXPECT_NEAR(c.value(0.0), 0.0, 1e-12); EXPECT_NEAR(c.value(1.0), 1.0, 1e-12);
	EXPECT_NEAR(c.value(2.0), 8.0, 1e-12);
	EXPECT_NEAR(c.value(3.0), 27.0, 1e-12);
	// Cubic interpolation of x^3 is exact on these nodes
	EXPECT_NEAR(c.value(0.5), 0.125, 1e-12);
	EXPECT_NEAR(c.value(1.5), 3.375, 1e-12);
	// 0.5^3
	// 1.5^3
	EXPECT_NEAR(c.value(2.5), 15.625, 1e-12); // 2.5^3
}

TEST(CurveOrder3, ExtrapolateDefaultLinear)
{
	const std::vector<double> xs3{ 0.0, 1.0, 2.0, 3.0 };
	const std::vector<double> ys3{ 0.0, 1.0, 8.0, 27.0 };
	Boundary<double> b1{TP::maths::Node::FIRST, 1, 0.0 };
	Boundary<double> b2{TP::maths::Node::LAST, 1, 27.0 };
	std::array<Boundary<double>, 2> boundaries{ b1, b2 };
	BSCurve<double, 3> c(xs3, ys3, boundaries);
	// Derivative of x^3 is 3x^2: at 0 -> 0, at 3 -> 27
	EXPECT_NEAR(c.value(-1.0), 0.0 + 0.0 * (-1.0 - 0.0), 1e-12); // 0
	EXPECT_NEAR(c.value(4.0), 27.0 + 27.0 * (4.0 - 3.0), 1e-12); // 54
}

TEST(CurveOrder4, InRangeExactQuartic)
{
	constexpr size_t order = 4;
	const std::vector<double> xs{ 0.0, 1.0, 2.0, 3.0, 4.0 }; 
	auto f = [](double x) { return x*x*x*x; }; // x^4 
	std::vector<double> ys(xs.size());
	std::transform(xs.begin(), xs.end(), ys.begin(), f);
	Boundary<double> b1{TP::maths::Node::FIRST, 1, 0.0 };
	Boundary<double> b2{TP::maths::Node::FIRST, 2, 0.0 };
	Boundary<double> b3{TP::maths::Node::LAST, 2, 192.0 };
	std::array<Boundary<double>, order -1> boundaries{ b1, b2, b3 };
	BSCurve<double, order> c(xs, ys, boundaries);
	// Verify it exactly hits the node values 
	EXPECT_NEAR(c.value(0.0), f(0.0), 1e-12); 
	EXPECT_NEAR(c.value(1.0), f(1.0), 1e-12);
	EXPECT_NEAR (c.value(2.0), f(2.0), 1e-12);
	EXPECT_NEAR(c.value(3.0), f(3.0), 1e-12);
	EXPECT_NEAR(c.value(4.0), f(4.0), 1e-12);
	// interpolation should be exact 
	EXPECT_NEAR(c.value(0.5), f(0.5), 1e-12);
	EXPECT_NEAR(c.value(1.5), f(1.5), 1e-12);
	EXPECT_NEAR(c.value(2.5), f(2.5), 1e-12);
	EXPECT_NEAR(c.value(3.5), f(3.5), 1e-12);
}



TEST(CurveOrder5, InRangeExactQuintic)
{
	const double tol = 1e-10;
	constexpr size_t order = 5;

	// x^5
	const std::vector<double> xs{ 0.0, 1.0, 2.0, 3.0, 4.0, 5.0 };
	auto g = [](double x) { return std::pow(x, 5); };
	std::vector<double> ys(xs.size());
	std::transform(xs.begin(), xs.end(), ys.begin(), g);
	Boundary<double> b1{ Node::FIRST, 1, 0.0 };
	Boundary<double> b2{ Node::FIRST, 2, 0.0 };
	Boundary<double> b3{ Node::LAST, 1, 5. * std::pow(5.0, 4) };
	Boundary<double> b4{ Node::LAST, 2, 20. * std::pow(5.0, 3) };
	std::array<Boundary<double>, order - 1> boundaries{ b1, b2, b3, b4 };
	BSCurve<double, order> c(xs, ys, boundaries);
	// Verify it exactly hits the node values EXPECT_NEAR(c.value(0.0), g(0.0), tol); EXPECT_NEAR(c.value(1.0), g(1.0), tol);
	EXPECT_NEAR(c.value(2.0), g(2.0), tol);
	EXPECT_NEAR(c.value(3.0), g(3.0), tol);
	EXPECT_NEAR(c.value(4.0), g(4.0), tol);
	EXPECT_NEAR(c.value(5.0), g(5.0), tol);
	// interpolation should be exact EXPECT_NEAR(c.value(0.5), g(0.5), tol); EXPECT_NEAR(c.value(1.5), g(1.5), tol);
	EXPECT_NEAR(c.value(2.5), g(2.5), tol);
	EXPECT_NEAR(c.value(3.5), g(3.5), tol);
	EXPECT_NEAR(c.value(4.5), g(4.5), tol);
	b3 = Boundary<double>{ Node::FIRST, 3, 0.0 }; std::array<Boundary<double>, order - 1> boundaries2{ b1, b2, b3, b4
	};
	BSCurve<double, order> c2(xs, ys, boundaries2);
	// Verify it exactly hits the node values EXPECT_NEAR(c2.value(0.0), g(0.0), tol);
	EXPECT_NEAR(c2.value(1.0), g(1.0), tol);
	EXPECT_NEAR(c2.value(2.0), g(2.0), tol);
	EXPECT_NEAR(c2.value(3.0), g(3.0), tol);
	EXPECT_NEAR(c2.value(4.0), g(4.0), tol);
	EXPECT_NEAR(c2.value(5.0), g(5.0), tol);
	// interpolation should be exact
	EXPECT_NEAR(c2.value(0.5), g(0.5), tol);
	EXPECT_NEAR(c2.value(1.5), g(1.5), tol);
	EXPECT_NEAR(c2.value(2.5), g(2.5), tol);
	EXPECT_NEAR(c2.value(3.5), g(3.5), tol);
	EXPECT_NEAR(c2.value(4.5), g(4.5), tol);
}


TEST(CurveOrder3, BSGeneratorDefaultLinear)
{
	const std::vector<double> xs3{ 0.0, 1.0, 2.0, 3.0 };
	const std::vector<double> ys3{ 0.0, 1.0, 8.0, 27.0 };
	Boundary<double> b1{ Node::FIRST, 1, 0.0 };
	Boundary<double> b2{ Node::LAST, 1, 27.0 };
	std::array<Boundary<double>, 2> boundaries{ b1, b2 };
	BSCurve<double, 3> c(xs3, ys3, boundaries);
	BSCurveGenerator<double, 3> bs_gen(xs3);
	auto c_gen = bs_gen.generator_BSpline(ys3, boundaries);
	for (size_t i = 0; i < 200; ++i)
	{
		double x = -0.02 + 0.03 * i; // 0.0 to 3.0
		EXPECT_NEAR(c.value(x), c_gen.value(x), 1e-12);
	}
}