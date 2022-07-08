#pragma once

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <string>

class ExportClass
{
public:
    ExportClass(std::string name) { mName = name; }

    std::string getName() { return mName; }

private:
    std::string mName;
};
