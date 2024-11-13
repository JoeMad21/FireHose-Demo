#include "device_libraries/firehose_ipu.hpp"
#include <string>
#include <cstdio>
#include <ctime>
#include <chrono>

int main(int argc, char *argv[]) {

    namespace po = boost::program_options;

    po::options_description desc("Options");

    long unsigned int num_transfers = 0;
    long unsigned int con_task = 0;
    long unsigned int op = OP::NOOP;
    long unsigned int num_inputs = 0;
    long unsigned int num_outputs = 0;
    long unsigned int dtype = 0;
    long unsigned int hardware = HARDWARE::IPU;
    long unsigned int num_devices = 1;
    long unsigned int num_cores = 1;
    long unsigned int map_tensor = 0;
    long unsigned int map_vertex = 0;
    long unsigned int comPat_Internal = 0;
    long unsigned int comPat_External = 0;
    long unsigned int core_tiles = 0;
    long unsigned int num_tiles = 0;
    long unsigned int max_layers_per_core = 0;
    long unsigned int max_tensors_per_core = 0;
    long unsigned int max_tensors_per_layer = 0;
    long unsigned int max_progs_per_core = 0;
    long unsigned int buf_depth = 0;

    desc.add_options()
        ("help", "produce help message")
        ("num_transfers", po::value<long unsigned int>(&con_task)->default_value(0), "")
        ("con_task", po::value<long unsigned int>(&op)->default_value(0), "")
        ("op", po::value<long unsigned int>(&op)->default_value(0), "Preselected operation for the IPU")
        ("num_inputs", po::value<long unsigned int>(&num_inputs)->default_value(0), "Number of inputs into the system")
        ("num_outputs", po::value<long unsigned int>(&num_outputs)->default_value(0), "Number of outputs into the system")
        ("dtype", po::value<long unsigned int>(&dtype)->default_value(0), "Data type for the IPU")
        ("hardware", po::value<long unsigned int>(&hardware)->default_value(0), "Device mode selected")
        ("num_devices", po::value<long unsigned int>(&num_devices)->default_value(0), "Number of IPUs to use")
        ("num_cores", po::value<long unsigned int>(&num_cores)->default_value(0), "Number of cores")
        ("map_tensor", po::value<long unsigned int>(&map_tensor)->default_value(0), "Mode of mapping tensors")
        ("map_vertex", po::value<long unsigned int>(&map_vertex)->default_value(0), "Mode of mapping vertices")
        ("comPat_Internal", po::value<long unsigned int>(&comPat_Internal)->default_value(0), "Internal communication pattern for templates")
        ("comPat_External", po::value<long unsigned int>(&comPat_External)->default_value(0), "External communication pattern for templates")
        ("core_tiles", po::value<long unsigned int>(&core_tiles)->default_value(0), "Tiles to anchor cores to")
        ("num_tiles", po::value<long unsigned int>(&num_tiles)->default_value(0), "Number of tiles for each core")
        ("max_layers_per_core", po::value<long unsigned int>(&max_layers_per_core)->default_value(0), "Number of tensor layers per core")
        ("max_tensors_per_layer", po::value<long unsigned int>(&max_tensors_per_layer)->default_value(0), "Max number of tensors for any given layer")
        ("max_progs_per_core", po::value<long unsigned int>(&max_progs_per_core)->default_value(0), "Maximum amount of programs for any given core")
        ("buf_depth", po::value<long unsigned int>(&buf_depth)->default_value(0), "Optimization parameter, number of communication buffers");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if(vm.count("help")) {
            std::cout << desc << std::endl;
            return 1;
        }

    std::clock_t startcputime = std::clock();
    double cpu_duration = (std::clock() - startcputime) / (double)CLOCKS_PER_SEC;
    auto wcts = std::chrono::system_clock::now();
    std::chrono::duration<double> wctduration = (std::chrono::system_clock::now() - wcts);

    switch(vm["con_task"].as<long unsigned int>()) {
        case TASK::TENSOR_DECOMP:
            startcputime = std::clock();
            wcts = std::chrono::system_clock::now();
            tensorDecomp(vm);
            cpu_duration = (std::clock() - startcputime) / (double)CLOCKS_PER_SEC;
            wctduration = (std::chrono::system_clock::now() - wcts);
            break;

        case TASK::MAT_MUL:
            startcputime = std::clock();
            wcts = std::chrono::system_clock::now();
            matMul(vm);
            cpu_duration = (std::clock() - startcputime) / (double)CLOCKS_PER_SEC;
            wctduration = (std::chrono::system_clock::now() - wcts);
            break;

        case TASK::MAT_ADD:
            startcputime = std::clock();
            wcts = std::chrono::system_clock::now();
            matAdd(vm);
            cpu_duration = (std::clock() - startcputime) / (double)CLOCKS_PER_SEC;
            wctduration = (std::chrono::system_clock::now() - wcts);
            break;

        case TASK::TRANSPOSE:
            startcputime = std::clock();
            wcts = std::chrono::system_clock::now();
            transpose(vm);
            cpu_duration = (std::clock() - startcputime) / (double)CLOCKS_PER_SEC;
            wctduration = (std::chrono::system_clock::now() - wcts);
            break;

        case TASK::CONVOLUTION:
            startcputime = std::clock();
            wcts = std::chrono::system_clock::now();
            convolution(vm);
            cpu_duration = (std::clock() - startcputime) / (double)CLOCKS_PER_SEC;
            wctduration = (std::chrono::system_clock::now() - wcts);
            break;
    }

    std::ofstream fileStream("TimeReport.out", std::ios::app);
    fileStream << "Report: CT" << vm["con_task"].as<long unsigned int>() << " DIM" << vm["col"].as<long unsigned int>() << std::endl;

    fileStream << "Finished in " << cpu_duration << " seconds [CPU Clock] " << std::endl;
    fileStream << "Finished in " << wctduration.count() << " seconds [Wall Clock]" << std::endl;

    return 0;
}
