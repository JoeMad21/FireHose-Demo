#pragma once

#include <boost/program_options.hpp>

#include <vector>
#include <algorithm>

#include <poplar/DeviceManager.hpp>
#include <poplar/Engine.hpp>
#include <poplar/Graph.hpp>
#include <poplar/IPUModel.hpp>
#include <poplar/Tensor.hpp>
#include <poplar/ReplicatedStreamMode.hpp>

#include <poputil/TileMapping.hpp>

enum HARDWARE {IPU, MODEL, CPU};
enum MAPPING {LINEAR, SET, SIM};
enum STAGE {INPUT, CONSUMPTION, OUTPUT, STAGES};
enum IO {IN, OUT};
enum COMPATSHAPE {TRIANGLEUP, TRIANGLEQR, TRIANGLEDOWN, SQUARE, LINE, CUSTOM};
enum OP::{NOOP, ADD, SUB, MULT, DECOMP}
enum CODELETS {POPLIN, POPOPS, POPRAND};


class graphConfig {
    private:

        // Graphcore IPU Types
        poplar::Graph graph;
        std::vector<poplar::Graph> vgraphs; // 1D array of virtual graphs, our cores essentially
        
        poplar::Device device;

        std::vector<std::vector<std::vector<poplar::Tensor>>> tensors; // 3D array of Tensors;

        std::vector<std::vector<poplar::DataStream>> strms; // 2D array of DataStreams;

        std::vector<std::vector<poplar::ComputeSet>> cps; // 2D array of ComputeSets

        std::vector<std::vector<std::vector<poplar::VertexRef>>> vtx; // 3D array of Vertices

        std::vector<std::vector<poplar::program::Program>> progs; // 2D array of Programs 


        poplar::Engine engine;

        // Basic Settings
        long unsigned int num_devices;
        long unsigned int hardware;
        long unsigned int map;
        long unsigned int num_cores;
        //long unsigned int comPat;
        //long unsigned int dtype;

        long unsigned int num_inputs;
        long unsigned int num_outputs;
        long unsigned int max_cps;
        long unsigned int max_tensor_layers;
        long unsigned int max_vtx;
        std::vector<long unsigned int> input_dims;

        std::vector<long unsigned int> start_tile_idx;
        std::vector<long unsigned int> num_tiles;
        std::vector<long unsigned int> num_tile_layers;
        std::vector<long unsigned int> num_tensors;
        std::vector<long unsigned int> num_tensor_layers;
        std::vector<std::vector<long unsigned int>> tile_layer_widths;
        std::vector<std::vector<long unsigned int>> tensor_layer_widths;

        // Performance Exploration
        long unsigned int buf_depth;


        //
        getDevice();
        allocateVirtualGraphs();
        allocateStreams();
        allocatePrograms();
    public:
        graphConfig();
        void buildLayers(long unsigned int& core, long unsigned int& layer_id, long unsigned int& num_tensors, long unsigned int& dim, std::vector<long unsigned int>& mappings = {});
        void buildLayers(long unsigned int& core, long unsigned int& layer_id, long unsigned int& num_tensors, std::vector<long unsigned int>& dims, std::vector<long unsigned int>& mappings = {});
        void plotVertex(long unsigned int& core, long unsigned int& layer_id, long unsigned int& idx, long unsigned int& cps_id, long unsigned int& input, long unsigned int& output, long unsigned int& tile_id, std::string& vertexName);

}

    // Input Settings
    // long unsigned int num_inputs;
    // long unsigned int num_input_tiles;
    // long unsigned int num_input_layers;
    // long unsigned int num_input_cores;
    // long unsigned int input_tile_idx;
    // std::vector<long unsigned int> input_dims;

    // Consumption Settings
    // long unsigned int num_consumption_tiles;
    // long unsigned int num_consumption_layers;
    // long unsigned int num_consumption_cores;
    // long unsigned int consumption_tile_idx;
    // std::vector<long unsigned int> consumption_dims;

    // Output Settings
    // long unsigned int num_outputs;
    // long unsigned int num_output_tiles;
    // long unsigned int num_output_layers;
    // long unsigned int num_output_cores;
    // long unsigned int output_tile_idx;
    // std::vector<long unsigned int> output_dims;

    // poplar::Graph graph;
    // std::vector<poplar::Graph> IN_vgraphs(consumption_cores);
    // std::vector<poplar::Graph> WORK_vgraphs(consumption_cores);
    // std::vector<poplar::Graph> OUT_vgraphs(consumption_cores);
    // poplar::Device device;
    // model originalModel;
    // std::vector<model> myModels;

    // long unsigned int dim = 0;
    // long unsigned int streams = 0;
    // long unsigned int compat = 0;
    // long unsigned int hw = 0;
    // long unsigned int map = 0;
    // long unsigned int buf_depth = 0;
    // long unsigned int offset = 0;
    // long unsigned int num_layers = 0;
        
    // std::vector< long unsigned int > layer_lens;
    // std::vector< poplar::program::Program > progs;

    // std::vector< std::vector<poplar::ComputeSet> > cps;
    // std::vector< std::vector< std::vector<poplar::VertexRef>  > > vtx;
    // std::vector< std::vector< std::vector<poplar::DataStream> > > strm;

    // this->v1 = 0;
    // this->v2 = 0;
    // this->v3 = 0;
    // this->v4 = 0;
    // this->v5 = 0;
    // this->v6 = 0;
    // this->v7 = 0;
    // this->v8 = 0;
    // this->hardware = 2;
    // this->compat = 0;
    // this->map = 0;
    // this->buf_depth = 0;
    // this->packets = 0;
    // this->consumption_cores = 0;
    // this->devices = 0;
    // this->seed = 0;
    // this->offset = 0;
    // this->num_layers = 0;
    // this->num_inputs = 0;
    // this->num_outputs = 0;