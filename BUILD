package(default_visibility = ["//visibility:public"])

cc_library(
    name = "lakeml-lib",
    srcs = [
            "src/BoostedClassifier.cpp",
            "src/GaussianLearner.cpp",
            "src/GaussianMixtureModel.cpp",
            "src/Histogram3D.cpp",
            "src/Kmeans.cpp",
            "src/LossFunction.cpp",
            "src/math_utils.cpp",
            "src/NaiveBayesClassifier.cpp",
            "src/ThresholdLearner.cpp",
            ],
    hdrs = [
            "src/BoostedClassifier.h",
            "src/Classifier.h",
            "src/ClassifierFactory.h",
            "src/Dataset.h",
            "src/GaussianLearner.h",
            "src/GaussianMixtureModel.h",
            "src/Histogram3D.h",
            "src/Kmeans.h",
            "src/LossFunction.h",
            "src/math_utils.h",
            "src/NaiveBayesClassifier.h",
            "src/ThresholdLearner.h",
            ],
)

cc_binary(
    name = "lakeml-demo",
    srcs = ["demo/demo.cpp"],
    deps = [":lakeml-lib"],
)
