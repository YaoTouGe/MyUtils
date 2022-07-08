#include <stdio.h>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace p = boost::python;
namespace np = boost::python::numpy;

int boost_py()
{
    Py_Initialize();
    p::object mm, mns, sys_module, env_module;

    try
    {
        mm = p::import("__main__");
        mns = mm.attr("__dict__");
        sys_module = p::import("sys");

        p::str module_dir = (std::string(TEST_DIR) + "/py").c_str();
        sys_module.attr("path").attr("insert")(1, module_dir);

        p::exec("from test import ExportClass", mns);

        env_module = p::eval("ExportClass()", mns);

        p::object print = env_module.attr("print_msg");
        p::object setA = env_module.attr("set_a");
        p::object setB = env_module.attr("set_b");

        setA(1);
        print();

        setB(10);
        print();
    }
    catch (const p::error_already_set &)
    {
        PyErr_Print();
    }

    return 0;
}