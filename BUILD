package(default_visibility = ["//visibility:public"])

cc_library(
    name = "lakeml-lib",
    srcs = [
            "src/boosted_classifier.cpp",
            "src/exponential_loss.cpp",
            "src/gaussian_learner.cpp",
            "src/gaussian_mixture_model.cpp",
            "src/histogram3d.cpp",
            "src/kmeans.cpp",
            "src/math_utils.cpp",
            "src/naive_bayes_classifier.cpp",
            "src/threshold_learner.cpp",
            ],
    hdrs = [
            "src/boosted_classifier.h",
            "src/classifier.h",
            "src/classifier_factory.h",
            "src/dataset.h",
            "src/exponential_loss.h",
            "src/gaussian_learner.h",
            "src/gaussian_mixture_model.h",
            "src/histogram3d.h",
            "src/kmeans.h",
            "src/loss.h",
            "src/math_utils.h",
            "src/naive_bayes_classifier.h",
            "src/threshold_learner.h",
            ],
)

cc_binary(
    name = "lakeml-demo",
    srcs = ["demo/demo.cpp"],
    deps = [":lakeml-lib"],
)
