
load("//tensorflow:tensorflow.bzl", "tf_cc_binary")

package(
    default_visibility = ["//tensorflow:internal"],
    licenses = ["notice"],  # Apache 2.0
)

exports_files(["LICENSE"])

tf_cc_binary(
    name = "main.exe",
    srcs = ["main.cc",
            "mainwindow.cc",
            "mainwindow.h",
            "panel.cc",
            "panel.h",
            "label.cc",
            "label.h",
            "sessionrunner.cc",
            "sessionrunner.h",
            "moc_mainwindow.cc",
            "moc_panel.cc",
            "moc_label.cc",
            "moc_sessionrunner.cc",
    ],
#    linkopts = ["-lm"],
    deps = [
        "//tensorflow/cc:cc_ops",
        "//tensorflow/core:core_cpu",
        "//tensorflow/core:framework",
        "//tensorflow/core:framework_internal",
        "//tensorflow/core:lib",
        "//tensorflow/core:protos_all_cc",
        "//tensorflow/core:tensorflow",
        "//tensorflow/cc:gradients",
        "//tensorflow/cc:queue_runner",
        "//tensorflow/cc/saved_model:loader_lite_impl",
        "//tensorflow/cc:client_session",
        "@qt//:qt_core",
        "@qt//:qt_gui",
        "@qt//:qt_widgets",
    ],
)
