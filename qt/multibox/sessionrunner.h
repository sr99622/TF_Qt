#ifndef SESSIONRUNNER_H
#define SESSIONRUNNER_H

#include <iostream>

#include "tensorflow/core/framework/tensor.h"
#include "tensorflow/core/framework/graph.pb.h"
#include "tensorflow/core/public/session.h"
#include "tensorflow/cc/framework/scope.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/cc/client/client_session.h"

#include <QtCore/QObject>
#include <QtCore/QRunnable>
#include <QtCore/QMutex>

#include "label.h"

using namespace std;
using namespace tensorflow;

class SessionRunner : public QObject, public QRunnable
{
    Q_OBJECT

public: 
    SessionRunner();
    void run() override;

    Status ParseLocationsFile(const string &filename);
    Status LoadGraph(string &filename);
    Status BuildDecoder(GraphDef *graph_def);
    Status BuildJpegReader(GraphDef *graph_def);
    Status Initialize();

    unique_ptr<Session> session;
    vector<Tensor> outputs;

    vector<pair<string, Tensor>> inputs;
    vector<string> output_names;

    bool initialized = false;

    string graph_path = "tensorflow/examples/multibox_detector/data/multibox_model.pb";
    string box_priors = "tensorflow/examples/multibox_detector/data/multibox_location_priors.txt";
    Tensor box_priors_means;
    Tensor box_priors_std_devs;

signals:
    void done();

};


#endif // SESSIONRUNNER_H