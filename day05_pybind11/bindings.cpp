#include <pybind11/pybind11.h>
#include "logger.hpp"

namespace py = pybind11;

PYBIND11_MODULE(edge_logger, m) {
    m.doc() = "Edge AI C++ logger bindings via pybind11";

    m.def("init_logger", &init_logger);
    m.def("read_sample", &read_sample);
    m.def("run_benchmark", &run_benchmark);
}