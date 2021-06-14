#include "include/putc.hpp"
#include <time/version.hpp>
#include <pybind11/pybind11.h>

PYBIND11_MODULE(pytime, m)
{
    m.attr("__version__") = TIME_VERSION;
    m.attr("__doc__") = "A toolkit for manipulating UTC time.";

    PTime::initializeUTC(m);
}
