
#include <iostream>

#include "lakeml/src/Dataset.h"
#include "lakeml/src/kmeans.h"


int main(int argc, char **argv) {

    DataInstance s1(2);
    s1[0] = 0;
    s1[1] = 1;
    DataInstance s2(2);
    s2[0] = 1;
    s2[1] = 2;
    DataInstance s3(2);
    s3[0] = 3;
    s3[1] = 3;

    Dataset data;
    data.add(s1,0);
    data.add(s2,1);
    data.add(s3,1);

    Kmeans k(data, 2);
    k.run(2, 0.1f);

    for (int i = 0; i < data.size(); ++i)
    {
        for (int j = 0; j < data[i].size(); ++j)
        {
            std::cout << " " << data[i][j];
        }
        std::cout << std::endl;
    }

    return 0;
}