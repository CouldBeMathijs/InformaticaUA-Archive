//
// Created by adria on 3-4-2025.
//

#include "PrimitiveFigures.h"

Figure3D PrimitiveFigures::createCube() {
    Figure3D figure;

    figure.points = {
            Vector3D::point(+1, -1, -1),
            Vector3D::point(-1, +1, -1),
            Vector3D::point(+1, +1, +1),
            Vector3D::point(-1, -1, +1),
            Vector3D::point(+1, +1, -1),
            Vector3D::point(-1, -1, -1),
            Vector3D::point(+1, -1, +1),
            Vector3D::point(-1, +1, +1),
    };

    Utils::addEdge(figure.lines, 0, 4);
    Utils::addEdge(figure.lines, 4, 2);
    Utils::addEdge(figure.lines, 2, 6);
    Utils::addEdge(figure.lines, 6, 0);

    Utils::addEdge(figure.lines, 4, 1);
    Utils::addEdge(figure.lines, 1, 7);
    Utils::addEdge(figure.lines, 7, 2);
    Utils::addEdge(figure.lines, 2, 4);

    Utils::addEdge(figure.lines, 1, 5);
    Utils::addEdge(figure.lines, 5, 3);
    Utils::addEdge(figure.lines, 3, 7);
    Utils::addEdge(figure.lines, 7, 1);

    Utils::addEdge(figure.lines, 5, 0);
    Utils::addEdge(figure.lines, 0, 6);
    Utils::addEdge(figure.lines, 6, 3);
    Utils::addEdge(figure.lines, 3, 5);

    Utils::addEdge(figure.lines, 6, 2);
    Utils::addEdge(figure.lines, 2, 7);
    Utils::addEdge(figure.lines, 7, 3);
    Utils::addEdge(figure.lines, 3, 6);

    Utils::addEdge(figure.lines, 0, 5);
    Utils::addEdge(figure.lines, 5, 1);
    Utils::addEdge(figure.lines, 1, 4);
    Utils::addEdge(figure.lines, 4, 0);

    figure.faces = {
        {0, 4, 2}, {0, 2, 6},
        {4, 1, 7}, {4, 7, 2},
        {1, 5, 3}, {1, 3, 7},
        {5, 0, 6}, {5, 6, 3},
        {6, 2, 7}, {6, 7, 3},
        {0, 5, 1}, {0, 1, 4}
    };

    return figure;
}

Figure3D PrimitiveFigures::createTetrahedron() {
    Figure3D figure;

    figure.points = {
            Vector3D::point(+1, -1, -1),
            Vector3D::point(-1, 1, -1),
            Vector3D::point(+1, +1, +1),
            Vector3D::point(-1, -1, +1)
    };

    Utils::addEdge(figure.lines, 0, 1);
    Utils::addEdge(figure.lines, 1, 2);
    Utils::addEdge(figure.lines, 2, 0);

    Utils::addEdge(figure.lines, 1, 3);
    Utils::addEdge(figure.lines, 3, 2);
    Utils::addEdge(figure.lines, 2, 1);

    Utils::addEdge(figure.lines, 0, 3);
    Utils::addEdge(figure.lines, 3, 1);
    Utils::addEdge(figure.lines, 1, 0);

    Utils::addEdge(figure.lines, 0, 2);
    Utils::addEdge(figure.lines, 2, 3);
    Utils::addEdge(figure.lines, 3, 0);

    figure.faces = {
        {0, 1, 2},
        {0, 3, 1},
        {0, 2, 3},
        {1, 3, 2}
    };

    return figure;
}

Figure3D PrimitiveFigures::createOctahedron() {
    Figure3D figure;

    figure.points = {
        Vector3D::point(+1,  0,  0),
        Vector3D::point( 0, +1,  0),
        Vector3D::point(-1,  0,  0),
        Vector3D::point( 0, -1,  0),
        Vector3D::point( 0,  0, -1),
        Vector3D::point( 0,  0, +1)
    };

    Utils::addEdge(figure.lines, 0, 1);
    Utils::addEdge(figure.lines, 1, 5);
    Utils::addEdge(figure.lines, 5, 0);

    Utils::addEdge(figure.lines, 1, 2);
    Utils::addEdge(figure.lines, 2, 5);
    Utils::addEdge(figure.lines, 5, 1);

    Utils::addEdge(figure.lines, 2, 3);
    Utils::addEdge(figure.lines, 3, 5);
    Utils::addEdge(figure.lines, 5, 2);

    Utils::addEdge(figure.lines, 3, 0);
    Utils::addEdge(figure.lines, 0, 5);
    Utils::addEdge(figure.lines, 5, 3);

    Utils::addEdge(figure.lines, 1, 0);
    Utils::addEdge(figure.lines, 0, 4);
    Utils::addEdge(figure.lines, 4, 1);

    Utils::addEdge(figure.lines, 2, 1);
    Utils::addEdge(figure.lines, 1, 4);
    Utils::addEdge(figure.lines, 4, 2);

    Utils::addEdge(figure.lines, 3, 2);
    Utils::addEdge(figure.lines, 2, 4);
    Utils::addEdge(figure.lines, 4, 3);

    Utils::addEdge(figure.lines, 0, 3);
    Utils::addEdge(figure.lines, 3, 4);
    Utils::addEdge(figure.lines, 4, 0);

    figure.faces = {
        {0, 1, 5},
        {1, 2, 5},
        {2, 3, 5},
        {3, 0, 5},
        {1, 0, 4},
        {2, 1, 4},
        {3, 2, 4},
        {0, 3, 4}
    };

    return figure;
}

Figure3D PrimitiveFigures::createIcosahedron() {
    Figure3D figure;

    figure.points = {
        Vector3D::point(0,  0,  std::sqrt(5)/2),

        Vector3D::point(1 , 0,  0.5),
        Vector3D::point(std::cos(2*M_PI/5), std::sin(2*M_PI/5), 0.5),
        Vector3D::point(std::cos(4*M_PI/5), std::sin(4*M_PI/5), 0.5),
        Vector3D::point(std::cos(6*M_PI/5), std::sin(6*M_PI/5), 0.5),
        Vector3D::point(std::cos(8*M_PI/5), std::sin(8*M_PI/5), 0.5),

        Vector3D::point(std::cos(M_PI/5), std::sin(M_PI/5), -0.5),
        Vector3D::point(std::cos(3*M_PI/5), std::sin(3*M_PI/5), -0.5),
        Vector3D::point(std::cos(5*M_PI/5), std::sin(5*M_PI/5), -0.5),
        Vector3D::point(std::cos(7*M_PI/5), std::sin(7*M_PI/5), -0.5),
        Vector3D::point(std::cos(9*M_PI/5), std::sin(9*M_PI/5), -0.5),

        Vector3D::point(0,  0,  -std::sqrt(5)/2)
    };

    int faces[20][3] = {
        {0, 1, 2}, {0, 2, 3}, {0, 3, 4}, {0, 4, 5}, {0, 5, 1},
        {1, 6, 2}, {2, 6, 7}, {2, 7, 3}, {3, 7, 8}, {3, 8, 4},
        {4, 8, 9}, {4, 9, 5}, {5, 9, 10}, {5, 10, 1}, {1, 10, 6},
        {11, 7, 6}, {11, 8, 7}, {11, 9, 8}, {11, 10, 9}, {11, 6, 10}
    };

    for (int i = 0; i < 20; i++) {
        Utils::addEdge(figure.lines, faces[i][0], faces[i][1]);
        Utils::addEdge(figure.lines, faces[i][1], faces[i][2]);
        Utils::addEdge(figure.lines, faces[i][2], faces[i][0]);
    }

    figure.faces = {
        {0, 1, 2}, {0, 2, 3}, {0, 3, 4}, {0, 4, 5}, {0, 5, 1},
        {1, 6, 2}, {2, 6, 7}, {2, 7, 3}, {3, 7, 8}, {3, 8, 4},
        {4, 8, 9}, {4, 9, 5}, {5, 9, 10}, {5, 10, 1}, {1, 10, 6},
        {11, 7, 6}, {11, 8, 7}, {11, 9, 8}, {11, 10, 9}, {11, 6, 10}
    };

    return figure;
}

Figure3D PrimitiveFigures::createDodecahedron() {
    Figure3D figure;

    std::vector<Vector3D> icosahedronPoints = {
        Vector3D::point(0,  0,  std::sqrt(5)/2),
        Vector3D::point(1 , 0,  0.5),
        Vector3D::point(std::cos(2*M_PI/5), std::sin(2*M_PI/5), 0.5),
        Vector3D::point(std::cos(4*M_PI/5), std::sin(4*M_PI/5), 0.5),
        Vector3D::point(std::cos(6*M_PI/5), std::sin(6*M_PI/5), 0.5),
        Vector3D::point(std::cos(8*M_PI/5), std::sin(8*M_PI/5), 0.5),
        Vector3D::point(std::cos(M_PI/5), std::sin(M_PI/5), -0.5),
        Vector3D::point(std::cos(3*M_PI/5), std::sin(3*M_PI/5), -0.5),
        Vector3D::point(std::cos(5*M_PI/5), std::sin(5*M_PI/5), -0.5),
        Vector3D::point(std::cos(7*M_PI/5), std::sin(7*M_PI/5), -0.5),
        Vector3D::point(std::cos(9*M_PI/5), std::sin(9*M_PI/5), -0.5),
        Vector3D::point(0,  0,  -std::sqrt(5)/2)
    };

    int faces[20][3] = {
        {0, 1, 2}, {0, 2, 3}, {0, 3, 4}, {0, 4, 5}, {0, 5, 1},
        {1, 6, 2}, {2, 6, 7}, {2, 7, 3}, {3, 7, 8}, {3, 8, 4},
        {4, 8, 9}, {4, 9, 5}, {5, 9, 10}, {5, 10, 1}, {1, 10, 6},
        {11, 7, 6}, {11, 8, 7}, {11, 9, 8}, {11, 10, 9}, {11, 6, 10}
    };

    for (int i = 0; i < 20; i++) {
        Vector3D A = icosahedronPoints[faces[i][0]];
        Vector3D B = icosahedronPoints[faces[i][1]];
        Vector3D C = icosahedronPoints[faces[i][2]];

        Vector3D mid = (A + B + C) / 3.0;
        figure.points.push_back(mid);
    }

    int pentagons[12][5] = {
        {0, 1, 2, 3, 4},
        {0, 5, 6, 7, 1},
        {1, 7, 8, 9, 2},
        {2, 9, 10, 11, 3},
        {3, 11, 12, 13, 4},
        {4, 13, 14, 5, 0},
        {19, 18, 17, 16, 15},
        {19, 14, 13, 12, 18},
        {18, 12, 11, 10, 17},
        {17, 10, 9, 8, 16},
        {16, 8, 7, 6, 15},
        {15, 6, 5, 14, 19}
    };

    for (int i = 0; i < 12; i++) {
        Utils::addEdge(figure.lines, pentagons[i][0], pentagons[i][1]);
        Utils::addEdge(figure.lines, pentagons[i][1], pentagons[i][2]);
        Utils::addEdge(figure.lines, pentagons[i][2], pentagons[i][3]);
        Utils::addEdge(figure.lines, pentagons[i][3], pentagons[i][4]);
        Utils::addEdge(figure.lines, pentagons[i][4], pentagons[i][0]);
    }

    for (int i = 0; i < 12; i++) {
        int a = pentagons[i][0];
        for (int j = 1; j < 4; j++) {
            figure.faces.push_back({a, pentagons[i][j], pentagons[i][j+1]});
        }
    }

    return figure;
}

Figure3D PrimitiveFigures::createCylinder(int n, double height) {
    Figure3D figure;

    figure.points.reserve(2*n);
    double dAlpha = 2.0 * M_PI / n;
    for (int i = 0; i < n; i++) {
        double alpha = i * dAlpha;
        double x = std::cos(alpha);
        double y = std::sin(alpha);
        figure.points.push_back(Vector3D::point(x, y, 0));
    }
    for (int i = 0; i < n; i++) {
        double alpha = i * dAlpha;
        double x = std::cos(alpha);
        double y = std::sin(alpha);
        figure.points.push_back(Vector3D::point(x, y, height));
    }

    for (int i = 0; i < n; i++){
        Utils::addEdge(figure.lines, i, (i+1) % n);
    }

    for (int i = 0; i < n; i++){
        Utils::addEdge(figure.lines, n + i, n + (i+1) % n);
    }

    for (int i = 0; i < n; i++){
        Utils::addEdge(figure.lines, i, n+i);
    }

    int bottomCenterIndex = static_cast<int>(figure.points.size());
    figure.points.push_back(Vector3D::point(0, 0, 0));

    int topCenterIndex = static_cast<int>(figure.points.size());
    figure.points.push_back(Vector3D::point(0, 0, height));

    // Onderkant en bovenkant trianguleren
    for (int i = 0; i < n; i++) {
        int next = (i + 1) % n;

        // Onderkant
        figure.faces.push_back({bottomCenterIndex, next, i});

        // Bovenkant
        figure.faces.push_back({topCenterIndex, n + i, n + next});
    }

    for (int i = 0; i < n; i++) {
        int next = (i + 1) % n;

        figure.faces.push_back({i, next, n + next});
        figure.faces.push_back({i, n + next, n + i});
    }

    return figure;
}

Figure3D PrimitiveFigures::createCone(int n, double height)
{
    Figure3D figure;

    figure.points.reserve(n+1);
    double dAlpha = 2.0*M_PI/n;
    for (int i = 0; i < n; i++){
        double alpha = i * dAlpha;
        figure.points.push_back( Vector3D::point(std::cos(alpha), std::sin(alpha), 0) );
    }

    figure.points.push_back( Vector3D::point(0, 0, height) );

    for (int i = 0; i < n; i++){
        Utils::addEdge(figure.lines, i, (i+1)%n);
    }

    int topIndex = n;
    for (int i = 0; i < n; i++){
        Utils::addEdge(figure.lines, i, topIndex);
    }

    for (int i = 0; i < n; i++) {
        int next = (i + 1) % n;

        figure.faces.push_back({i, next, topIndex});
    }

    return figure;
}

Figure3D PrimitiveFigures::createSphere(int subdivisions) {
    Figure3D base = createIcosahedron();

    std::vector<std::array<int,3>> triangles = {
        {0, 1, 2}, {0, 2, 3}, {0, 3, 4}, {0, 4, 5}, {0, 5, 1},
        {1, 6, 2}, {2, 6, 7}, {2, 7, 3}, {3, 7, 8}, {3, 8, 4},
        {4, 8, 9}, {4, 9, 5}, {5, 9, 10}, {5, 10, 1}, {1, 10, 6},
        {11, 7, 6}, {11, 8, 7}, {11, 9, 8}, {11, 10, 9}, {11, 6, 10}
    };

    std::vector<Vector3D> points = base.points;

    for (int i = 0; i < subdivisions; i++){
        subdivideIco(points, triangles);
    }

    for (auto &p : points){
        p.normalise();
    }

    Figure3D figure;
    figure.points = points;
    for (auto &tri : triangles){
        Utils::addEdge(figure.lines, tri[0], tri[1]);
        Utils::addEdge(figure.lines, tri[1], tri[2]);
        Utils::addEdge(figure.lines, tri[2], tri[0]);
    }

    figure.faces = std::vector<std::array<int,3>>(triangles.begin(), triangles.end());

    return figure;
}

Figure3D PrimitiveFigures::createTorus(double R, double r, int n, int m) {
    Figure3D figure;

    figure.points.reserve((n+1)*(m+1));
    double du = 2.0*M_PI/n;
    double dv = 2.0*M_PI/m;

    for (int i = 0; i <= n; i++){
        double u = i*du;
        for (int j = 0; j <= m; j++){
            double v = j*dv;
            double x = (R + r*std::cos(v)) * std::cos(u);
            double y = (R + r*std::cos(v)) * std::sin(u);
            double z = r*std::sin(v);
            figure.points.push_back(Vector3D::point(x,y,z));
        }
    }

    auto idx = [&](int i, int j){
        return i*(m+1) + j;
    };

    for (int i = 0; i <= n; i++){
        for (int j = 0; j <= m; j++){

            int jnext = (j+1) % (m+1); // wrap-around
            Utils::addEdge(figure.lines, idx(i,j), idx(i,jnext));


            int inext = (i+1) % (n+1); // wrap-around
            Utils::addEdge(figure.lines, idx(i,j), idx(inext,j));
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int a = idx(i, j);
            int b = idx((i + 1) % (n + 1), j);
            int c = idx((i + 1) % (n + 1), (j + 1) % (m + 1));
            int d = idx(i, (j + 1) % (m + 1));

            // Twee driehoeken per vierhoek
            figure.faces.push_back({a, b, c});
            figure.faces.push_back({a, c, d});
        }
    }

    return figure;
}

void PrimitiveFigures::subdivideIco(std::vector<Vector3D> &points, std::vector<std::array<int,3>> &triangles) {
    std::map<long long,int> middlePointCache;
    std::vector<std::array<int,3>> newTriangles;
    newTriangles.reserve(triangles.size()*4);

    for (auto &tri : triangles) {
        int i0 = tri[0];
        int i1 = tri[1];
        int i2 = tri[2];

        int a = getMiddlePoint(i0, i1, points, middlePointCache);
        int b = getMiddlePoint(i1, i2, points, middlePointCache);
        int c = getMiddlePoint(i2, i0, points, middlePointCache);

        newTriangles.push_back({i0,a,c});
        newTriangles.push_back({i1,b,a});
        newTriangles.push_back({i2,c,b});
        newTriangles.push_back({a,b,c});
    }
    triangles = std::move(newTriangles);
}

int PrimitiveFigures::getMiddlePoint(int i1, int i2, std::vector<Vector3D> &points, std::map<long long,int> &middlePointCache) {
    if (i1>i2) std::swap(i1,i2);
    long long key = ((long long)i1 << 32) + i2;

    auto it = middlePointCache.find(key);
    if (it != middlePointCache.end()){
        return it->second;
    }

    Vector3D p1 = points[i1];
    Vector3D p2 = points[i2];
    Vector3D mid = Vector3D::point(
            0.5*(p1.x + p2.x),
            0.5*(p1.y + p2.y),
            0.5*(p1.z + p2.z)
    );
    int index = (int)points.size();
    points.push_back(mid);
    middlePointCache[key] = index;
    return index;
}