
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "src/boosted_classifier.h"
#include "src/classifier_factory.h"
#include "src/csv_loader.h"
#include "src/dataset.h"
#include "src/kmeans.h"
#include "src/naive_bayes_classifier.h"
#include "src/threshold_learner.h"

// Factory that creates ThresholdLearner instances cycling through feature indices.
class RoundRobinThresholdFactory : public ClassifierFactory {
public:
    explicit RoundRobinThresholdFactory(int num_features)
        : num_features_(num_features), next_feature_(0) {}

    Classifier *createRandomInstance() const override {
        int feature = next_feature_;
        next_feature_ = (next_feature_ + 1) % num_features_;
        return new ThresholdLearner(feature);
    }

private:
    int num_features_;
    mutable int next_feature_;
};

int main(int argc, char **argv) {
    std::string data_path = (argc > 1) ? argv[1] : "data/iris.csv";

    std::cout << "Loading dataset from: " << data_path << std::endl;

    Dataset dataset;
    try {
        dataset = LoadCsvDataset(data_path);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Usage: " << argv[0] << " [path/to/iris.csv]" << std::endl;
        std::cerr << "Run from the workspace root or provide the full path." << std::endl;
        return 1;
    }

    std::cout << "Loaded " << dataset.size() << " samples with "
              << dataset[0].size() << " features." << std::endl;

    // --- K-means Clustering Demo ---
    std::cout << "\n=== K-means Clustering (k=3) ===" << std::endl;

    const int k = 3;
    Kmeans kmeans(dataset, k);
    int iters = kmeans.run(100, 0.001f);
    std::cout << "Converged in " << iters << " iterations." << std::endl;

    // Tally cluster assignments against true class labels (0=setosa, 1=versicolor, 2=virginica)
    int confusion[3][3] = {};
    for (size_t i = 0; i < dataset.size(); ++i) {
        int true_class = dataset.getLabelAt(i);
        int cluster = kmeans.getClosestClusterLabel(dataset[i]);
        if (true_class >= 0 && true_class < k && cluster >= 0 && cluster < k)
            confusion[true_class][cluster]++;
    }

    std::cout << "\nSamples per class assigned to each cluster:\n";
    std::cout << std::left << std::setw(20) << "Class"
              << std::setw(10) << "Cluster0"
              << std::setw(10) << "Cluster1"
              << std::setw(10) << "Cluster2" << std::endl;
    const char *class_names[] = {"Iris-setosa", "Iris-versicolor", "Iris-virginica"};
    for (int c = 0; c < k; ++c) {
        std::cout << std::left << std::setw(20) << class_names[c];
        for (int cl = 0; cl < k; ++cl)
            std::cout << std::setw(10) << confusion[c][cl];
        std::cout << std::endl;
    }

    // --- Naive Bayes Binary Classification Demo ---
    std::cout << "\n=== Naive Bayes Classification (Setosa vs. Others) ===" << std::endl;

    // Build binary dataset: setosa = -1, versicolor/virginica = 1
    Dataset binary_dataset;
    for (size_t i = 0; i < dataset.size(); ++i) {
        DataInstance sample = dataset[i];
        int label = (dataset.getLabelAt(i) == 0) ? -1 : 1;
        binary_dataset.add(sample, label);
    }

    const int num_features = static_cast<int>(dataset[0].size());
    RoundRobinThresholdFactory factory(num_features);
    NaiveBayesClassifier nb(&factory, num_features);
    std::vector<double> weights(binary_dataset.size(), 1.0);
    nb.train(binary_dataset, weights);

    int correct = 0;
    for (size_t i = 0; i < binary_dataset.size(); ++i) {
        if (nb.classify(binary_dataset[i]) == binary_dataset.getLabelAt(i))
            ++correct;
    }
    double accuracy = 100.0 * correct / static_cast<double>(binary_dataset.size());
    std::cout << "Training accuracy: " << std::fixed << std::setprecision(1)
              << accuracy << "% (" << correct << "/" << binary_dataset.size() << ")"
              << std::endl;

    // --- AdaBoost Binary Classification Demo ---
    std::cout << "\n=== AdaBoost Classification (Setosa vs. Others) ===" << std::endl;

    BoostedClassifier boosted(&factory, 10, 4);
    boosted.train(binary_dataset, weights);

    correct = 0;
    for (size_t i = 0; i < binary_dataset.size(); ++i) {
        if (boosted.classify(binary_dataset[i]) == binary_dataset.getLabelAt(i))
            ++correct;
    }
    accuracy = 100.0 * correct / static_cast<double>(binary_dataset.size());

    std::cout << "Weak learners: " << boosted.getNumWeakLearners() << std::endl;
    std::cout << "Training accuracy: " << std::fixed << std::setprecision(1)
              << accuracy << "% (" << correct << "/" << binary_dataset.size() << ")"
              << std::endl;

    return 0;
}
