/*
 *   Copyright 2008-2012 Hugo Penedones
 *
 *   This file is part of lakeml.
 *
 *   lakeml is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   lakeml is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with lakeml.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CSV_LOADER_H_
#define CSV_LOADER_H_

#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "dataset.h"

// Load a Dataset from a CSV file. The first row is treated as a header and
// skipped. All columns except the last are parsed as numeric features using
// std::stod; malformed values will cause std::stod to throw std::invalid_argument
// or std::out_of_range, which propagate to the caller. The last column is treated
// as the class label: numeric strings are parsed as integers, and non-numeric
// strings are mapped to sequential integers starting from 0.
inline Dataset LoadCsvDataset(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    Dataset dataset;
    std::map<std::string, int> label_map;
    int next_label = 0;
    std::string line;

    // Skip header row
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 2) continue;

        DataInstance sample;
        for (size_t i = 0; i + 1 < tokens.size(); ++i) {
            sample.push_back(std::stod(tokens[i]));
        }

        const std::string &label_str = tokens.back();
        int label;
        try {
            label = std::stoi(label_str);
        } catch (const std::invalid_argument &) {
            if (label_map.find(label_str) == label_map.end()) {
                label_map[label_str] = next_label++;
            }
            label = label_map[label_str];
        } catch (const std::out_of_range &) {
            if (label_map.find(label_str) == label_map.end()) {
                label_map[label_str] = next_label++;
            }
            label = label_map[label_str];
        }

        dataset.add(sample, label);
    }

    return dataset;
}

#endif  // CSV_LOADER_H_
