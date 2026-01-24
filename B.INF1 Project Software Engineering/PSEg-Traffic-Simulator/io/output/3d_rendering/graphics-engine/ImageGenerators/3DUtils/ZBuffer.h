//
// Created by adria on 3-4-2025.
//

#ifndef ZBUFFER_H
#define ZBUFFER_H
#include <vector>


class ZBuffer {
public:
    ZBuffer(int width, int height);

    bool testAndSet(int x, int y, double z);
    void clear();

private:
    int width, height;
    std::vector<std::vector<double>> buffer;
};


#endif //ZBUFFER_H
