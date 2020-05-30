/***********************************************************************
Copyright 2020 Stephen Rhodes
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
************************************************************************/

#include "sessionrunner.h"

using namespace std;
using namespace tensorflow;
using namespace tensorflow::ops;

SessionRunner::SessionRunner()
{
    setAutoDelete(false);
}

Status SessionRunner::Initialize()
{
    TF_RETURN_IF_ERROR(ParseLocationsFile(box_priors));
    TF_RETURN_IF_ERROR(LoadGraph(graph_path));
    initialized = true;

    return Status::OK();
}

void SessionRunner::run() 
{
    Status status;
    if (!initialized) {
        status = Initialize();
        if (!status.ok()) {
            cout << "SessionRunner::Initialize error " << status.error_message() << endl;
        }
    }

    status = session->Run(inputs, output_names, {}, &outputs);
    if (!status.ok()) {
        cout << "SessionRunner::run error " << status.error_message() << endl;
        return;
    }

    emit done();
}

Status SessionRunner::LoadGraph(string &filename)
{
    GraphDef graph_def;
    TF_RETURN_IF_ERROR(BuildJpegReader(&graph_def));

    GraphDef model_graph_def;
    TF_RETURN_IF_ERROR(ReadBinaryProto(Env::Default(), filename, &model_graph_def));

    for (int i = 0; i < model_graph_def.node_size(); i++) {
        NodeDef node_def = model_graph_def.node(i);
        for (int j = 0; j < node_def.input_size(); j++) {
            if (node_def.input(j) == "ResizeBilinear") {
                node_def.set_input(j, "j_normalized");
            }
        }
        *graph_def.add_node() = node_def;
    }

    GraphDef decoder_graph_def;
    TF_RETURN_IF_ERROR(BuildDecoder(&decoder_graph_def));
    for (int i = 0; i < decoder_graph_def.node_size(); i++) {
        NodeDef node_def = decoder_graph_def.node(i);
        if (node_def.name() == "x_top_k") {
            node_def.set_input(0, "output_scores/Reshape");
        }
        if (node_def.name() == "x_reshape") {
            node_def.set_input(0, "output_locations/Reshape");
        }
        if (node_def.name() == "x_draw_boxes") {
            node_def.set_input(0, "j_dims_expander");
        }
        *graph_def.add_node() = node_def;
    }

    session.reset(NewSession(SessionOptions()));
    TF_RETURN_IF_ERROR(session->Create(graph_def));

    return Status::OK();
}
    
Status SessionRunner::BuildJpegReader(GraphDef *graph_def)
{
    int32 input_width = 224;
    int32 input_height = 224;
    float input_mean = 128.0;
    float input_std = 128.0;

    Scope root = Scope::NewRootScope();
    Placeholder filename(root.WithOpName("j_filename"), DT_STRING);
    tensorflow::ops::ReadFile file_reader(root.WithOpName("j_file_reader"), filename);
    DecodeJpeg jpeg_reader(root.WithOpName("j_jpeg_reader"), file_reader, DecodeJpeg::Channels(3));
    Cast float_caster(root.WithOpName("j_float_caster"), jpeg_reader, DT_FLOAT);
    auto zero = Const(root.WithOpName("j_zero"), 0);
    ExpandDims dims_expander(root.WithOpName("j_dims_expander"), float_caster, zero);
    auto input_size = Const(root.WithOpName("j_input_size"), {input_height, input_width});
    ResizeBilinear resized(root.WithOpName("j_resized"), dims_expander, input_size);
    auto mean = Const(root.WithOpName("j_input_mean"), {input_mean});
    auto std_dev = Const(root.WithOpName("j_input_std"), {input_std});
    Div normalized(root.WithOpName("j_normalized"), Sub(root.WithOpName("j_sub_normal"), resized, mean), std_dev);

    return root.ToGraphDef(graph_def);
}

Status SessionRunner::BuildDecoder(GraphDef *graph_def)
{
    Scope root = Scope::NewRootScope();
    Placeholder input_scores(root.WithOpName("x_input_scores"), DT_FLOAT);
    Placeholder how_many_labels(root.WithOpName("x_how_many_labels"), DT_INT32);
    Placeholder input_locations(root.WithOpName("x_input_locations"), DT_FLOAT);
    Placeholder original_image(root.WithOpName("x_original_image"), DT_UINT8);

    TopK top_k(root.WithOpName("x_top_k"), input_scores, how_many_labels);

    Reshape reshape(root.WithOpName("x_reshape"), input_locations, {784, 4});
    Transpose transpose(root.WithOpName("x_transpose"), reshape, {1, 0});
    Gather gather(root.WithOpName("x_gather"), transpose, {1, 0, 3, 2});
    Transpose reordered(root.WithOpName("x_reordered"), gather, {1, 0});

    Gather detections(root.WithOpName("x_gather_detections"), reordered, top_k.indices);
    Gather means(root.WithOpName("x_gather_means"), box_priors_means, top_k.indices);
    Gather std_devs(root.WithOpName("s_gather_std_devs"), box_priors_std_devs, top_k.indices);

    Add add(root.WithOpName("x_add"), Multiply(root, detections, std_devs), means);
    Maximum maximum(root.WithOpName("x_maximum"), add, {0.0f});
    Minimum minimum(root.WithOpName("x_minimum"), maximum, {1.0f});
    Sigmoid sigmoid(root.WithOpName("x_sigmoid"), top_k.values);
    
    Cast float_caster(root.WithOpName("x_float_caster"), original_image, DT_FLOAT);
    ExpandDims image_expander(root.WithOpName("x_image_expander"), float_caster, 0);
    DrawBoundingBoxes draw_boxes(root.WithOpName("x_draw_boxes"), image_expander, minimum);
    Cast uint_image(root.WithOpName("x_uint_image"), draw_boxes, DT_UINT8);

    return root.ToGraphDef(graph_def);
}

Status SessionRunner::ParseLocationsFile(const string &filename)
{
    Scope root = Scope::NewRootScope();
    tensorflow::ops::ReadFile file_reader(root.WithOpName("file_reader"), filename);
    StringStrip strip(root.WithOpName("strip"), file_reader);
    ExpandDims expander(root.WithOpName("substr"), strip, 0);
    RegexReplace new_line_double(root.WithOpName("new_line_double"), expander, "\n\n", ":");
    RegexReplace new_line(root.WithOpName("new_line"), new_line_double, "\n", ",");
    StringSplit splitter(root.WithOpName("splitter"), new_line, ":");
    DecodeCSV csv(root.WithOpName("csv"), splitter.values, {{0.0f}, {0.0f}, {0.0f}, {0.0f}, {0.0f}, {0.0f}, {0.0f}, {0.0f}});
    Stack stack(root.WithOpName("stack"), csv.output);
    Gather means(root.WithOpName("means"), stack, {2, 0, 6, 4});
    Gather std_devs(root.WithOpName("std_devs"), stack, {3, 1, 7, 5});
    Transpose transpose_means(root.WithOpName("transpose_means"), means, {1, 0});
    Transpose transpose_std_devs(root.WithOpName("transpose_std_devs"), std_devs, {1, 0});

    vector<Tensor> outputs;
    ClientSession session(root);
    TF_RETURN_IF_ERROR(session.Run({transpose_means, transpose_std_devs}, &outputs));

    box_priors_means = outputs[0];
    box_priors_std_devs = outputs[1];

    return Status::OK();
}



