#include"test.h"

using namespace boost::python;
BOOST_PYTHON_MODULE(testpy)
{
    Py_Initialize();

    class_<ExportClass>("Exported", init<std::string>())
        .def("getName", &ExportClass::getName);
}