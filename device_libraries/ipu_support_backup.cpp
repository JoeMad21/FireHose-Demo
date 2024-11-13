#include "ipu_support.hpp"

void graphConfig::getDevice() {
    std::cout << "Getting Device..." << std::endl;

    auto manager = poplar::DeviceManager::createDeviceManager();
    std::vector<poplar::Device> temp_devices;
    poplar::Device temp_device;

    switch(this->hardware) {
        case HARDWARE::IPU:
            temp_devices = manager.getDevices(poplar::TargetType::IPU, this->device);
            break;

        case HARDWARE::MODEL:
            temp_devices = manager.getDevices(poplar::TargetType::IPU_MODEL, this->device);
            break;

        case HARDWARE::CPU:
            temp_devices = manager.getDevices(poplar::TargetType::CPU, this->device);
            break;
    }

    auto it = std::find_if(temp_devices.begin(), temp_devices.end(), [](poplar::Device &device) { return device.attach(); });
    poplar::Device device;

    if (it != temp_devices.end()) {
        temp_device = std::move(*it);
    }

    std::cout << "Got Device!" << std::endl;

    this->device = temp_device;

    return;
}

void graphConfig::graphConfig() {

    // Graph Details
    this->v1 = 0;
    this->v2 = 0;
    this->v3 = 0;
    this->v4 = 0;
    this->v5 = 0;
    this->v6 = 0;
    this->v7 = 0;
    this->v8 = 0;
    this->hardware = 2;
    this->compat = 0;
    this->map = 0;
    this->buf_depth = 0;
    this->packets = 0;
    this->consumption_cores = 0;
    this->devices = 0;
    this->seed = 0;
    this->offset = 0;
    this->num_layers = 0;
    this->num_inputs = 0;
    this->num_outputs = 0;

    // Get Device
    poplar::Device device = getDevice();
    this->device = device;

    const unsigned numTiles = device.getTarget().getNumTiles();

    // Graph Setup
    std::cout << "Creating Graph..." << std::endl;
    poplar::Graph graph(this->device.getTarget());
    this->graph = graph;
    this->IN_vgraph = this->graph.createVirtualGraph(0, 3);
    this->WORK_vgraph = this->graph.createVirtualGraph(4, 6);
    this->OUT_vgraph = this->graph.createVirtualGraph(7, 9);
    std::cout << "Created Graph!" << std::endl;

    // Create Memory Connections
    allocateComputeSets();
    allocateVertices();
    allocateStreams();

    return;
}

void graphConfig::graphConfig(boost::program_options::variables_map& vm) {

    // Graph Details
    this->v1 = vm["v1"].as<long unsigned int>();
    this->v2 = vm["v2"].as<long unsigned int>();
    this->v3 = vm["v3"].as<long unsigned int>();
    this->v4 = vm["v4"].as<long unsigned int>();
    this->v5 = vm["v5"].as<long unsigned int>();
    this->v6 = vm["v6"].as<long unsigned int>();
    this->v7 = vm["v7"].as<long unsigned int>();
    this->v8 = vm["v8"].as<long unsigned int>();
    this->hardware = vm["hardware"].as<long unsigned int>();
    this->compat = vm["compat"].as<long unsigned int>();
    this->map = vm["map"].as<long unsigned int>();
    this->buf_depth = vm["buf_depth"].as<long unsigned int>();
    this->packets = vm["packets"].as<long unsigned int>();
    this->consumption_cores = vm["consumption_cores"].as<long unsigned int>();
    this->devices = vm["devices"].as<long unsigned int>();
    this->seed = vm["seed"].as<long unsigned int>();
    this->offset = vm["offset"].as<long unsigned int>();
    this->num_layers = vm["num_layers"].as<long unsigned int>();
    this->num_inputs = vm["num_inputs"].as<long unsigned int>();
    this->num_outputs = vm["num_outputs"].as<long unsigned int>();
    this->remote_buffer = vm["remote_buffer"].as<long unsigned int>();
    
    // Get Device
    poplar::Device device = getDevice();

    const unsigned numTiles = device.getTarget().getNumTiles();

    // Graph Setup
    std::cout << "Creating Graph..." << std::endl;
    poplar::Graph graph(this->device.getTarget());
    this->graph = graph;
    this->IN_vgraph = this->graph.createVirtualGraph(0, 3);
    this->WORK_vgraph = this->graph.createVirtualGraph(4, 6);
    this->OUT_vgraph = this->graph.createVirtualGraph(7, 9);
    std::cout << "Created Graph!" << std::endl;

    // Create Memory Connections
    allocateComputeSets();
    allocateVertices();
    allocateStreams();
    
    return;
}

void graphConfig::graphConfig(std::map<std::string, long unsigned int>& vm) {

    // Graph Details
    this->v1 = vm["v1"];
    this->v2 = vm["v2"];
    this->v3 = vm["v3"];
    this->v4 = vm["v4"];
    this->v5 = vm["v5"];
    this->v6 = vm["v6"];
    this->v7 = vm["v7"];
    this->v8 = vm["v8"];
    this->hardware = vm["hardware"];
    this->compat = vm["compat"];
    this->map = vm["map"];
    this->buf_depth = vm["buf_depth"];
    this->packets = vm["packets"];
    this->consumption_cores = vm["consumption_cores"];
    this->devices = vm["devices"];
    this->seed = vm["seed"];
    this->offset = vm["offset"];
    this->num_layers = vm["num_layers"];
    this->num_inputs = vm["num_inputs"];
    this->num_outputs = vm["num_outputs"];
    this->preproc_stages = vm["preproc_stages"];
    this->work_stages = vm["work_stages"];
    this->postproc_stages = vm["postproc_stages"];

    // Get Device
    poplar::Device device = getDevice(vm["hardware"], vm["devices"]);
    this->device = device;

    const unsigned numTiles = device.getTarget().getNumTiles();

    // Graph Setup
    std::cout << "Creating Graph..." << std::endl;
    poplar::Graph graph(this->device.getTarget());
    this->graph = graph;
    this->IN_vgraph = this->graph.createVirtualGraph(0, 490);
    this->WORK_vgraph = this->graph.createVirtualGraph(491, 980);
    this->OUT_vgraph = this->graph.createVirtualGraph(981, 1471);
    std::cout << "Created Graph!" << std::endl;

    // Create Memory Connections
    allocateComputeSets();
    allocateVertices();
    allocateStreams();

    return;
}

void graphConfig::allocateComputeSets() {
    //long unsigned int dim1 = this->preproc_stages+this->work_stages+this->postproc_stages;

    long unsigned int dim1 = this->this->consumption_cores;

    std::vector< std::vector<poplar::Vertex>> cps(dim1, std::vector<poplar::Vertex>>(dim2));

    this->cps = cps;

    return;
}

void graphConfig::allocateVertices() {

    long unsigned int dim1 = this->preproc_stages+this->work_stages+this->postproc_stages-1;
    long unsigned int dim2 = this->consumption_cores;
    long unsigned int dim3 = std::max(this->num_inputs, this->num_outputs);

    std::vector< std::vector< std::vector<poplar::Vertex>>> vtx(dim1, std::vector<std::vector<poplar::Vertex>>(dim2, std::vector<poplar::Vertex>(dim3)));

    this->vtx = vtx;

    return;
}

void graphConfig::allocateStreams() {
    long unsigned int dim1 = 2; // You're either IN or OUT
    long unsigned int dim2 = this->consumption_cores;
    long unsigned int dim3 = std::max(this->num_inputs, this->num_outputs);


    std::vector< std::vector< std::vector<poplar::Vertex>>> strms(dim1, std::vector<std::vector<poplar::Vertex>>(dim2, std::vector<poplar::Vertex>(dim3)));

    this->strms = strms;

    return;
}

void graphConfig::createPipelines() {

    /* Create Datastreams */

    // Inputs
    for(int port = 0; port < this->num_inputs; port++) {
           
        addInputStreams(port, IO::IN);
        
    }

    // Outputs
    for(int port = 0; port < this->num_outputs; port++) {
           
        addOutputStreams(port, IO::OUT);
        
    }

    /* Create ComputeSets */
           
    addMoveComputeSets();

    /* Create Vertices */

    //addMoveVertex();
    
}

void graphConfig::addCodelet(int cdlt) {

    switch(cdlt) {
        case CODELETS::POPLIN:
            poplin::addCodelets(this->graph);
            break;

        case CODELETS::POPOPS:
            popops::addCodelets(this->graph);
            break;

        case CODELETS::POPRAND:
            poprand::addCodelets(this->graph);
            break;

        default:
            break;
    }

    return;
}

void graphConfig::addCodelet(std::string filename) {

    std::string fileAddress;
    fileAddress = "./device_libraries/" + filename;

    this->graph.addCodelets(fileAddress);

    return;
}

void graphConfig::addPipeline(poplar::program::Sequence& seq, int io, int port) {

    switch(io) {
        case IO::IN:
            for(int i = 0; i < this->consumption_cores; i++) {

                // Stream Inputs Programs

                seq.add(poplar::program::Copy(this->strm[IO::IN][i][port], this->myModels[i].layers[0].tensors[port]));
                seq.add(poplar::program::Execute(cps[IO::IN][i]));

            }
            break;

        case IO::OUT:
            long unsigned int bottom_layer = this->num_layers;
            for(int i = 0; i < this->consumption_cores; i++) {

                // Stream Outputs Programs

                seq.add(poplar::program::Execute(cps[IO::OUT][i]));
                seq.add(poplar::program::Copy(myModels[i].layers[bottom_layer].tensors[port], strm[IO::OUT][i]));
            }
            break;

    }

    return;
}
/*
void graphConfig::appendLayer(int layer_id, long unsigned int dim) {
    
    std::string db_name;

    long unsigned int mydim = 0;

    db_name = "Layer " + std::to_string(layer_id) + " Tensor " + std::to_string(i);
    mydim = dim;
    this->originalModel.layers.tensors.push_back(this->graph.addvariable(poplar::FLOAT, {mydim, mydim}, db_name));

    if(this->hardware != HARDWARE::IPU) {
            graph.setTileMapping(myLayer.tensors[i], 0);
        }
        else {
            switch(this->map) {
                case MAPPING::LINEAR:
                    poputil::mapTensorLinearly(this->graph, myLayer.tensors[i]);
                    break;
                case MAPPING::SET:
                    graph.setTileMapping(myLayer.tensors[i], i);
                    break;
                default:
                    poputil::mapTensorLinearly(graph, myLayer.tensors[i]);
                    std::cout << "WARNING: DEFAULTED IN appendLayer()" << std::endl;
                    break;
            }
        }

    return;

}
*/
void graphConfig::buildLayer(int layer_id, int num_tensors) {

    this->num_layers++;
    this->layer_lens.push_back(num_tensors);

    std::string db_name;

    long unsigned int mydim = 0;

    for(int i = 0; i < num_tensors; i++) {
        db_name = "Layer " + std::to_string(layer_id) + " Tensor " + std::to_string(i);
        mydim = this->dim;
        this->originalModel.layers.tensors.push_back(this->graph.addVariable(poplar::FLOAT, {mydim, mydim} , db_name));

        if(this->hardware != HARDWARE::IPU) {
            graph.setTileMapping(myLayer.tensors[i], 0);
        }
        else {
            switch(this->map) {
                case MAPPING::LINEAR:
                    poputil::mapTensorLinearly(this->graph, myLayer.tensors[i]);
                    break;
                case MAPPING::SET:
                    graph.setTileMapping(myLayer.tensors[i], i);
                    break;
                default:
                    poputil::mapTensorLinearly(graph, myLayer.tensors[i]);
                    std::cout << "WARNING: DEFAULTED IN buildLayer()" << std::endl;
                    break;
            }
        }
    }

    return;
}

void graphConfig::addMoveComputeSets() {

    std::string db_name;
    std::string title;
    
    //long unsigned int input_stage = this->preproc_stages - 1;
    //long unsigned int output_stage = this->preproc_stages+this->work_stages;

    
    
    title = "Input Compute Set for Model ";
    for (int i = 0; i < this->consumption_cores; i++) {
        db_name = title + std::to_string(i);
        this->cps[STAGE::IN][i] = this->IN_vgraph[i].addComputeSet(db_name);
    }
    
    title = "Output Compute Set for Model ";
    for (int i = 0; i < this->consumption_cores; i++) {
        db_name = title + std::to_string(i);
        this->cps[STAGE::OUT][i] = this->OUT_vgraph[i].addComputeSet(db_name);
    }

    return;
}

void graphConfig::addInputStreams(long unsigned int port_id) {

    std::string db_name;
    std::string title;
    std::string port_name = "port ";
    std::string model_name = " of model ";

    poplar::OptionFlags streamOpts {
      {"bufferingDepth", std::to_string(this->buf_depth)},
    };
    
    title = "Input Stream to ";

    for (int i = 0; i < this->consumption_cores; i++) {
        db_name = title + port_name + std::to_string(port_id) + model_name + std::to_string(i);
        this->strms[IO::IN][i][port_id] = this->IN_vgraph[i].addHostToDeviceFIFO(db_name, poplar::FLOAT, this->v1*this->v2, poplar::ReplicatedStreamMode::REPLICATE, streamOpts);
    }

    return;
}

void graphConfig::addOutputStreams(long unsigned int port_id) {

    std::string db_name;
    std::string title;
    std::string port_name = "port ";
    std::string model_name = " of model ";

    poplar::OptionFlags streamOpts {
      {"bufferingDepth", std::to_string(this->buf_depth)},
    };
    
    title = "Input Stream to ";

    for (int i = 0; i < this->consumption_cores; i++) {
        db_name = title + port_name + std::to_string(port_id) + model_name + std::to_string(i);
        this->strms[IO::OUT][i][port_id] = this->OUT_vgraph[i].addDeviceToHostFIFO(db_name, poplar::FLOAT, this->v1*this->v2, streamOpts);
    }
    
    return;
}

// void graphConfig::addMoveStream(long unsigned int port_id, long unsigned int io) {

//     std::string db_name;
//     std::string title;
//     std::string port_name = "port ";
//     std::string model_name = " of model ";

//     poplar::OptionFlags streamOpts {
//       {"bufferingDepth", std::to_string(this->buf_depth)},
//     };
    
//     switch(IO) {
//         case IO::IN:
//             title = "Input Stream to ";
//             break;
//         case IO::OUT:
//             title = "Output Stream to ";
//             break;
//         default:
//             title = "Input Stream ";
//             std::cout << "WARNING: DEFAULTED IN addStream()" << std::endl;
//             break;
//     }
    

//     switch(IO) {
//         case IO::IN:
            
//             for (int i = 0; i < this->consumption_cores; i++) {
//                 db_name = title + port_name + std::to_string(port_id) + model_name + std::to_string(i);
//                 this->strms[0][i][port_id] = this->IN_vgraph[i].addHostToDeviceFIFO(db_name, poplar::FLOAT, this->v1*this->v2, poplar::ReplicatedStreamMode::REPLICATE, streamOpts);
//             }
//             break;

//         case IO::OUT:

//             for (int i = 0; i < this->consumption_cores; i++) {
//                 db_name = title + port_name + std::to_string(port_id) + model_name + std::to_string(i);
//                 this->strms[1][i][port_id] = this->OUT_vgraph[i].addDeviceToHostFIFO(db_name, poplar::FLOAT, this->v1*this->v2, streamOpts);
//             }
//             break;

//         default:
//             std::cout << "WARNING: DEFAULTED IN addStream()" << std::endl;
//             break;
//     }

//     return;
// }

// void graphConfig::addMoveVertices(long unsigned int io, long unsigned int port_id) {

//     std::string db_name;
//     std::string title;
//     std::string model_name = " of Model ";
    
//     long unsigned int input_cps = this->preproc_stages - 1;
//     long unsigned int output_cps = this->preproc_stages+this->work_stages;
//     long unsigned int chosen_cps;

//     long unsigned int input_stage = this->preproc_stages;
//     long unsigned int output_stage = this->preproc_stages+this->work_stage-1;
//     long unsigned int chosen_stage;

//     switch(io) {
//         case IO::IN:
//             title = "Input Vertex for Port ";
//             chosen_stage = input_stage;
//             chosen_cps = input_cps;
//             break;
//         case IO::OUT:
//             title = "Output Vertex for Port ";
//             chosen_stage = output_stage;
//             chosen_cps = output_cps;
//             break;
//     }
    
//     for (int model = 0; model < this->consumption_cores; model++) {
//         db_name = title + std::to_string(port_id) + model_name + std::to_string(model);
//         this->vtx[chosen_stage][model][port_id] = this->graph.addVertex(this->cps[chosen_cps][model], db_name);
//     }


//     return;

// }

void graphConfig::addVertex(long unsigned int& core, ) {

    for (int i = 0; i < this->consumption_cores; i++) {

        this->vtx[stage][num_port][i] = this->graph.addVertex(this->cps[io][i], vertex_name);

        switch(this->hardware) {
            case HARDWARE::IPU:
                graph.setTileMapping(vtx[io][num_port][i], i+(this->offset));
                break:
            default:
                graph.setTileMapping(this->vtx[io][num_port][i], 0);
        }

        graph.setPerfEstimate(this->vtx[io][num_port][i], 40, 40);
    }

    return;

}

void graphConfig::connectVertex(int io, int num_port, int top_layer, int bottom_layer, int top_tensor, int bottom_tensor) {
    for(int i = 0; i < this->streams; i++) {
        this->graph.connect(this->vtx[io][num_port]["strm_in"], this->myModels[i].layers[top_layer].tensors[top_tensor]);
        this->graph.connect(this->vtx[io][num_port]["strm_out"], this->myModels[i].layers[bottom_layer].tensors[bottom_tensor]);
    }

    return;
}

void graphConfig::connectEngineStream(std::vector<std::vector<std::vector<float>>>& cpu, int num_port, int IO) {

    std::string db_name;
    std::string title;
    std::string port;
    
    switch(IO) {
        case IO::IN:
            title = "Input Stream ";
            port = " for input ";
            break;
        case IO::OUT:
            title = "Output Stream ";
            port = " for output  ";
            break;
        default:
            title = "Input Stream ";
            port = " for input ";
            std::cout << "WARNING: DEFAULTED IN addStream()" << std::endl;
            break;
    }

    for (int i = 0; i < this->streams; i++) {
        db_name = title + std::to_string(i) + port + std::to_string(num_port);
        this->engine.connectStream(db_name, cpu[i].data(), cpu[i].data() + cpu[i].size());
    }

    return;
}

void graphConfig::buildTemplate() {

    switch(this->compat) {

        case COMPATSHAPE::TRIANGLEUP:
            this->buildLayer(0, 1);
            this->buildLayer(1, 1);
            this->buildLayer(2, 2);

            this->addVertex("IOVertex");
            this->addVertex("IOVertex");
            this->addVertex("IOVertex");

            this->connectVertex(LAYERS::INPUT, LAYERS::CONSUMPTION, 0, 0);
            this->connectVertex(LAYERS::CONSUMPTION, LAYERS::OUTPUT, 0, 0);
            this->connectVertex(LAYERS::CONSUMPTION, LAYERS::OUTPUT, 0, 1);

            this->addStream(0, IO::IN);
            this->addStream(0, IO::OUT);
            this->addStream(1, IO::OUT);

            break;

        case COMPATSHAPE::TRIANGLEDOWN:
            this->buildLayer(0, 2);
            this->buildLayer(1, 2);
            this->buildLayer(2, 1);

            this->addVertex("IOVertex");
            this->addVertex("IOVertex");
            this->addVertex("IOVertex");

            this->connectVertex(LAYERS::INPUT, LAYERS::CONSUMPTION, 0, 0);
            this->connectVertex(LAYERS::INPUT, LAYERS::CONSUMPTION, 1, 1);
            this->connectVertex(LAYERS::CONSUMPTION, LAYERS::OUTPUT, 0, 0);

            this->addStream(0, IO::IN);
            this->addStream(1, IO::IN);
            this->addStream(0, IO::OUT);

            break;

        case COMPATSHAPE::SQUARE:
            this->buildLayer(0, 2);
            this->buildLayer(1, 2);
            this->buildLayer(2, 2);

            this->addVertex("IOVertex");
            this->addVertex("IOVertex");
            this->addVertex("IOVertex");
            this->addVertex("IOVertex");

            this->connectVertex(LAYERS::INPUT, LAYERS::CONSUMPTION, 0, 0);
            this->connectVertex(LAYERS::INPUT, LAYERS::CONSUMPTION, 1, 1);
            this->connectVertex(LAYERS::CONSUMPTION, LAYERS::OUTPUT, 0, 0);
            this->connectVertex(LAYERS::CONSUMPTION, LAYERS::OUTPUT, 1, 1);

            this->addStream(0, IO::IN);
            this->addStream(1, IO::IN);
            this->addStream(0, IO::OUT);
            this->addStream(1, IO::OUT);

            break;

        case COMPATSHAPE::LINE:
            buildLayer(0, 1);
            buildLayer(1, 1);
            buildLayer(2, 1);

            this->addVertex("IOVertex");
            this->addVertex("IOVertex");

            this->connectVertex(LAYERS::INPUT, LAYERS::CONSUMPTION, 0, 0);
            this->connectVertex(LAYERS::CONSUMPTION, LAYERS::OUTPUT, 0, 0);

            this->addStream(0, IO::IN);
            this->addStream(0, IO::OUT);
            
            break;

        default:
            std::cout << "WARNING: DEFAULTED IN addStream()" << std::endl;
            break;
    }
}

// void graphConfig::buildTensorTemplate() {
//     std::cout << "Building Model..." << std::endl;

//     switch(this->compat) {
//         case COMPATSHAPE::TRIANGLEUP:
//             buildLayer(0, 1);
//             buildLayer(1, 1);
//             buildLayer(2, 2);
//             break;

//         case COMPATSHAPE::TRIANGLEQR:
//             buildLayer(0, 1);
//             buildLayer(1, 2);
//             buildLayer(2, 2);
//             break;

//         case COMPATSHAPE::TRIANGLEDOWN:
//             buildLayer(0, 2);
//             buildLayer(1, 2);
//             buildLayer(2, 2);
//             break;

//         case COMPATSHAPE::SQUARE:
//             buildLayer(0, 2);
//             buildLayer(1, 2);
//             buildLayer(2, 2);
//             break;

//         case COMPATSHAPE::LINE:
//             buildLayer(0, 1);
//             buildLayer(1, 1);
//             buildLayer(2, 1);
//             break;

//         default:
//             std::cout << "WARNING: DEFAULTED IN addStream()" << std::endl;
//             break;
//     }

//     // Duplicate Model (Still copied to vector even if there is no copy)
//     // TO DO: Overload assignment operator
//     for(int i = 0; i < num_streams; i++) {
//         myModels.push_back(myModel);
//     }
//     std::cout << "Built Model!" << std::endl;

//     return;
// }

// void graphConfig::buildIOTemplate() {

//     std::cout << "Adding Vertices..." << std::endl;

//     addComputeSet(IO::IN);
//     addComputeSet(IO::OUT);

//     switch(mode) {
//         case COMPATSHAPE::TRIANGLEUP:
//             addVertex();
//             addVertex();
//             addVertex();
//             break;
//         case COMPATSHAPE::TRIANGLEQR:
//             addVertex();
//             addVertex();
//             addVertex();
//             break;
//         case COMPATSHAPE::TRIANGLEDOWN:
//             addVertex();
//             addVertex();
//             addVertex();
//             break;
//         case COMPATSHAPE::SQUARE:
//             addVertex();
//             addVertex();
//             addVertex();
//             addVertex();
//             break;
//         case COMPATSHAPE::LINE:
//             addVertex();
//             addVertex();
//             break;

//         default:
//             std::cout << "WARNING: DEFAULTED IN addStream()" << std::endl;
//             break;
//     }

//     std::string in = "strm_in";
//     std::string out = "strm_out";

//     switch(mode) {
//         case COMPATSHAPE::TRIANGLEUP:
//             connectVertex(graph, comPat.vtx.in0, myModels, num_streams, LAYERS::INPUT, LAYERS::CONSUMPTION, 0, 0, in, out);
//             connectVertex(graph, comPat.vtx.out0, myModels, num_streams, LAYERS::CONSUMPTION, LAYERS::OUTPUT, 0, 0, in, out);
//             connectVertex(graph, comPat.vtx.out1, myModels, num_streams, LAYERS::CONSUMPTION, LAYERS::OUTPUT, 0, 1, in, out);
//             break;

//         case COMPATSHAPE::TRIANGLEQR:
//             connectVertex(graph, comPat.vtx.in0, myModels, num_streams, LAYERS::INPUT, LAYERS::CONSUMPTION, 0, 0, in, out);
//             connectVertex(graph, comPat.vtx.out0, myModels, num_streams, LAYERS::CONSUMPTION, LAYERS::OUTPUT, 0, 0, in, out);
//             connectVertex(graph, comPat.vtx.out1, myModels, num_streams, LAYERS::CONSUMPTION, LAYERS::OUTPUT, 1, 1, in, out);
//             break;

//         case COMPATSHAPE::TRIANGLEDOWN:
//             connectVertex(graph, comPat.vtx.in0, myModels, num_streams, LAYERS::INPUT, LAYERS::CONSUMPTION, 0, 0, in, out);
//             connectVertex(graph, comPat.vtx.in1, myModels, num_streams, LAYERS::INPUT, LAYERS::CONSUMPTION, 1, 1, in, out);
//             connectVertex(graph, comPat.vtx.out0, myModels, num_streams, LAYERS::CONSUMPTION, LAYERS::OUTPUT, 0, 0, in, out);
//             break;

//         case COMPATSHAPE::SQUARE:
//             connectVertex(graph, comPat.vtx.in0, myModels, num_streams, LAYERS::INPUT, LAYERS::CONSUMPTION, 0, 0, in, out);
//             connectVertex(graph, comPat.vtx.in1, myModels, num_streams, LAYERS::INPUT, LAYERS::CONSUMPTION, 1, 1, in, out);
//             connectVertex(graph, comPat.vtx.out0, myModels, num_streams, LAYERS::CONSUMPTION, LAYERS::OUTPUT, 0, 0, in, out);
//             connectVertex(graph, comPat.vtx.out1, myModels, num_streams, LAYERS::CONSUMPTION, LAYERS::OUTPUT, 1, 1, in, out);
//             break;

//         case COMPATSHAPE::LINE:
//             connectVertex(graph, comPat.vtx.in0, myModels, num_streams, LAYERS::INPUT, LAYERS::CONSUMPTION, 0, 0, in, out);
//             connectVertex(graph, comPat.vtx.out0, myModels, num_streams, LAYERS::CONSUMPTION, LAYERS::OUTPUT, 0, 0, in, out);
//             break;

//         default:
//             std::cout << "WARNING: DEFAULTED IN addStream()" << std::endl;
//             break;
//     }

//     std::cout << "Added Vertices!" << std::endl;

//     // Streams
//     std::cout << "Adding Streams..." << std::endl;

//     switch(mode) {
//         //Changed buffer depth
//         case COMPATSHAPE::TRIANGLEUP:
//             addStream(graph, comPat.strm.in0, params, 8, 0, num_streams, IO::IN);
//             addStream(graph, comPat.strm.out0, params, 8, 0, num_streams, IO::OUT);
//             addStream(graph, comPat.strm.out1, params, 8, 1, num_streams, IO::OUT);
//             break;
        
//         case COMPATSHAPE::TRIANGLEQR:
//             addStream(graph, comPat.strm.in0, params, 8, 0, num_streams, IO::IN);
//             addStream(graph, comPat.strm.out0, params, 8, 0, num_streams, IO::OUT);
//             addStream(graph, comPat.strm.out1, params, 8, 1, num_streams, IO::OUT);
//             break;

//         case COMPATSHAPE::TRIANGLEDOWN:
//             addStream(graph, comPat.strm.in0, params, 8, 0, num_streams, IO::IN);
//             addStream(graph, comPat.strm.in1, params, 8, 1, num_streams, IO::IN);
//             addStream(graph, comPat.strm.out0, params, 8, 0, num_streams, IO::OUT);
//             break;

//         case COMPATSHAPE::SQUARE:
//             addStream(graph, comPat.strm.in0, params, 8, 0, num_streams, IO::IN);
//             addStream(graph, comPat.strm.in1, params, 8, 1, num_streams, IO::IN);
//             addStream(graph, comPat.strm.out0, params, 8, 0, num_streams, IO::OUT);
//             addStream(graph, comPat.strm.out1, params, 8, 1, num_streams, IO::OUT);
//             break;

//         case COMPATSHAPE::LINE:
//             addStream(graph, comPat.strm.in0, params, 8, 0, num_streams, IO::IN);
//             addStream(graph, comPat.strm.out0, params, 8, 0, num_streams, IO::OUT);
//             break;

//         default:
//             std::cout << "WARNING: DEFAULTED IN addStream()" << std::endl;
//             break;
//     }

//     std::cout << "Added Streams!" << std::endl;

//     return;

// }

void graphConfig::buildGraphTemplate(poplar::Graph& graph, graphConfig& myGraphConfig) {

    

}

mygraphConfig.buildLayer(0, 2);
mygraphConfig.buildLayer(1, 1);

mygraphConfig.connectVertex(vtx, 0, 0, 0);
mygraphConfig.connectVertex(vtx, 0, 1, 0);



 for (unsigned i = 0; i < numRows; ++i) {
    auto v = graph.addVertex(mulCS,              // Put the vertex in the
                                                 // 'mulCS' compute set.
                             "MatMulVertex", // Create a vertex of this
                                                 // type.
                             {{"a", matrixA[i]},  // Connect input 'a' of the
                                                 // vertex to a row of the
                                                 // matrix.
                              {"b", matrixB[i]},         // Connect input 'b' of the
                                                 // vertex to whole
                                                 // input vector.
                              {"out", out[i]}}); // Connect the output 'out'
                                                 // of the vertex to a single
                                                 // element of the output
                                                 // vector.
    graph.setTileMapping(v, i);
  }