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

#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <string>

#include <gtest/gtest.h>

#include "src/csv_loader.h"
#include "src/dataset.h"

// Write a temporary CSV file and return its path.
static std::string WriteTempCsv(const std::string &content) {
    char path[] = "/tmp/lakeml_csv_test_XXXXXX";
    int fd = mkstemp(path);
    EXPECT_GE(fd, 0);
    close(fd);
    std::ofstream f(path);
    f << content;
    return std::string(path);
}

// Basic loading: numeric labels, two features.
TEST(CsvLoaderTest, LoadsNumericLabels) {
    std::string path = WriteTempCsv(
        "f1,f2,label\n"
        "1.0,2.0,0\n"
        "3.0,4.0,1\n"
        "5.0,6.0,0\n");

    Dataset ds = LoadCsvDataset(path);
    std::remove(path.c_str());

    ASSERT_EQ(ds.size(), static_cast<size_t>(3));
    EXPECT_DOUBLE_EQ(ds[0][0], 1.0);
    EXPECT_DOUBLE_EQ(ds[0][1], 2.0);
    EXPECT_EQ(ds.getLabelAt(0), 0);
    EXPECT_EQ(ds.getLabelAt(1), 1);
    EXPECT_EQ(ds.getLabelAt(2), 0);
}

// String labels should be mapped to sequential integers starting from 0.
TEST(CsvLoaderTest, MapsStringLabels) {
    std::string path = WriteTempCsv(
        "f1,f2,class\n"
        "1.0,2.0,cat\n"
        "3.0,4.0,dog\n"
        "5.0,6.0,cat\n");

    Dataset ds = LoadCsvDataset(path);
    std::remove(path.c_str());

    ASSERT_EQ(ds.size(), static_cast<size_t>(3));
    // "cat" comes first, so it maps to 0; "dog" maps to 1
    EXPECT_EQ(ds.getLabelAt(0), 0);
    EXPECT_EQ(ds.getLabelAt(1), 1);
    EXPECT_EQ(ds.getLabelAt(2), 0);
}

// Header row must be skipped (dataset should have exactly the data rows).
TEST(CsvLoaderTest, SkipsHeader) {
    std::string path = WriteTempCsv(
        "sepal_length,sepal_width,class\n"
        "5.1,3.5,Iris-setosa\n"
        "7.0,3.2,Iris-versicolor\n");

    Dataset ds = LoadCsvDataset(path);
    std::remove(path.c_str());

    EXPECT_EQ(ds.size(), static_cast<size_t>(2));
}

// Feature count: three numeric features plus one string label.
TEST(CsvLoaderTest, CorrectFeatureCount) {
    std::string path = WriteTempCsv(
        "a,b,c,label\n"
        "1.0,2.0,3.0,A\n");

    Dataset ds = LoadCsvDataset(path);
    std::remove(path.c_str());

    ASSERT_EQ(ds.size(), static_cast<size_t>(1));
    EXPECT_EQ(ds[0].size(), static_cast<size_t>(3));
}

// Missing file should throw a std::runtime_error.
TEST(CsvLoaderTest, ThrowsOnMissingFile) {
    EXPECT_THROW(LoadCsvDataset("/tmp/this_file_does_not_exist_lakeml.csv"),
                 std::runtime_error);
}

// Empty lines inside the file should be silently ignored.
TEST(CsvLoaderTest, IgnoresEmptyLines) {
    std::string path = WriteTempCsv(
        "f1,label\n"
        "1.0,0\n"
        "\n"
        "2.0,1\n");

    Dataset ds = LoadCsvDataset(path);
    std::remove(path.c_str());

    EXPECT_EQ(ds.size(), static_cast<size_t>(2));
}
