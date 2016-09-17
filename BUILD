package(default_visibility = ["//visibility:public"])

cc_library(
    name = "lakeml-lib",
    srcs = [
    		"src/BoostedClassifier.cpp",
    		"src/NaiveBayesClassifier.cpp",
    		"src/GaussianLearner.cpp",
    		"src/ThresholdLearner.cpp",
    		"src/GaussianMixtureModel.cpp",
    		"src/kmeans.cpp",
			"src/Histogram3D.cpp",
			"src/math_utils.cpp",
			"src/LossFunction.cpp",
			],
    hdrs = [
	    	"src/BoostedClassifier.h",
	    	"src/Histogram3D.h",
			"src/Classifier.h",
			"src/LossFunction.h",
			"src/ClassifierFactory.h",
			"src/NaiveBayesClassifier.h",
			"src/Dataset.h",
			"src/ThresholdLearner.h",
			"src/GaussianLearner.h",
			"src/kmeans.h",
			"src/GaussianMixtureModel.h",
			"src/math_utils.h",
			],
)

cc_binary(
    name = "lakeml-demo",
    srcs = ["demo/demo.cpp"],
    deps = [":lakeml-lib"],
)
