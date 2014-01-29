#include <nbody/Vector3.h>
#include <gtest/gtest.h>

TEST(vectorTest, dotProduct) {
    auto v = Vector3f{ 1, 0, 0 };
    auto v_perp = Vector3f{ 0, 1, -1 };
    
    ASSERT_FLOAT_EQ(dot(v, v_perp ) , 0.0f );
}

TEST(vectorTest, crossProduct) {
    auto v = Vector3f{ 1, 2, 3 };
    auto i = Vector3f{ 1, 0, 0 };
    auto j = Vector3f{ 0, 1, 0 };
    auto k = Vector3f{ 0, 0, 1 };
    ASSERT_FLOAT_EQ(cross(v, v ).norm(), 0.0f );
    ASSERT_FLOAT_EQ((cross(i, j ) - k ).norm(), 0.0f );
    ASSERT_FLOAT_EQ((cross(j, k ) - i ).norm(), 0.0f );
    ASSERT_FLOAT_EQ((cross(k, i ) - j ).norm(), 0.0f );
}

//test norm() and normsq() for ints, floats, and doubles
TEST(vectorTest, norm) {
    auto vf = Vector3f{ 6, 3, 2 };
    auto vd = Vector3d{ 6, 3, 2 };
    auto vi = Vector3i{ 6, 3, 2 };

    ASSERT_FLOAT_EQ(vf.normsq(), 49.0f);
    ASSERT_EQ(vd.normsq(), 49.0);
    ASSERT_EQ(vi.normsq(), 49);

    ASSERT_FLOAT_EQ(vf.norm(), 7.0f);
    ASSERT_EQ(vd.norm(), 7.0);
    ASSERT_EQ(vi.norm(), 7);
}

//test overloaded addition and subtraction operators
TEST(vectorTest, addSubtract) {
    auto v = Vector3f{ 5.2f, 4.1f, 3.0f };
    auto w = Vector3f{ 2.5f, 4.6f, 6.1983743f };
    auto sum = v + w;
    auto diff = v - w;

    ASSERT_FLOAT_EQ( sum.x(), 7.7f);
    ASSERT_FLOAT_EQ( sum.y(), 8.7f);
    ASSERT_FLOAT_EQ( sum.z(), 9.1983743f);

    ASSERT_FLOAT_EQ( diff.x(), 2.7f);
    ASSERT_FLOAT_EQ( diff.y(), -0.5f);
    ASSERT_FLOAT_EQ( diff.z(), -3.1983743f);
}

//test overloaded * operator for scalar * vector and vector * scalar
TEST(vectorTest, multiplyScalar) {
    auto v = Vector3i{ -3, -5, -7 };
    auto svproduct = 2 * v;
    auto vsproduct = v * -3;

    ASSERT_EQ( svproduct.x(), -6);
    ASSERT_EQ( svproduct.y(), -10);
    ASSERT_EQ( svproduct.z(), -14);

    ASSERT_EQ( vsproduct.x(), 9);
    ASSERT_EQ( vsproduct.y(), 15);
    ASSERT_EQ( vsproduct.z(), 21);
}

//test overloaded / operator
TEST(vectorTest, divideScalar) {
    auto v = Vector3f{ 9, 18, 27 };
    auto quotient = v / 6.0f;

    ASSERT_FLOAT_EQ( quotient.x(), 1.5f);
    ASSERT_FLOAT_EQ( quotient.y(), 3.0f);
    ASSERT_FLOAT_EQ( quotient.z(), 4.5f);

}

//test cube() for float, int, double
TEST(vectorTest, cube) {
    float f = cube(3.0f);
    double d = cube(3.0);
    int i = cube(3);

    ASSERT_FLOAT_EQ(f, 27.0f);
    ASSERT_EQ(d, 27.0);
    ASSERT_EQ(i, 27);
}