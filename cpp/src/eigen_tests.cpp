#include <iostream>
#include "Eigen/Core"

int eigen_test()
{
    Eigen::Vector2f a = {0, 2};
    Eigen::Vector2f b = {1, 0};
    Eigen::Matrix2f A;
    A.row(0) = a;
    A.row(1) = b;

    std::cout << a;
    return 0;
}