#include "Draw3DLines.h"

#include "General.h"
#include "external/l_parser.h"
#include "external/ini_configuration.h"
#include "external/vector3d.h"

#include <cmath>
#include <fstream>
#include <stack>
#include <unordered_map>

Matrix scaleFigure(const double scale) {
    Matrix m;
    m(1, 1) = scale;
    m(2, 2) = scale;
    m(3, 3) = scale;
    return m;
}

Matrix rotateX(const double angle) {
    Matrix m;
    double cosangle = std::cos(angle);
    double sinangle = std::sin(angle);
    m(2, 2) = cosangle;
    m(2, 3) = sinangle;
    m(3, 2) = -sinangle;
    m(3, 3) = cosangle;
    return m;
}

Matrix rotateY(const double angle) {
    Matrix m;
    double cosangle = std::cos(angle);
    double sinangle = std::sin(angle);
    m(1, 1) = cosangle;
    m(3, 1) = sinangle;
    m(1, 3) = -sinangle;
    m(3, 3) = cosangle;
    return m;
}

Matrix rotateZ(const double angle) {
    Matrix m;
    double cosangle = std::cos(angle);
    double sinangle = std::sin(angle);
    m(1, 1) = cosangle;
    m(2, 1) = -sinangle;
    m(1, 2) = sinangle;
    m(2, 2) = cosangle;
    return m;
}

Matrix translate(const Vector3D &vector) {
    Matrix m;
    m(4, 1) = vector.x;
    m(4, 2) = vector.y;
    m(4, 3) = vector.z;
    return m;
}

Matrix translate(const std::vector<double> &v) {
    if (v.size() != 3) {
        throw std::invalid_argument(
            "Cannot generate translation matrix from a vector of size " + std::to_string(v.size()));
    }
    return translate(Vector3D::point(v[0], v[1], v[2]));
}

void applyTransformation(std::vector<Figure> &figs, const Matrix &m) {
    for (auto &fig: figs) {
        for (auto &point: fig.points) {
            point *= m;
        }
    }
}

void toPolar(const Vector3D &point, double &theta, double &phi, double &r) {
    r = std::sqrt(std::pow(point.x, 2) + std::pow(point.y, 2) +
                  std::pow(point.z, 2));
    theta = std::atan2(point.y, point.x);
    phi = std::acos(point.z / r);
}

std::vector<img::Line2D> doProjectionNoZBuf(const std::vector<Figure> &figures) {
    std::vector<img::Line2D> out;
    for (const auto &figure: figures) {
        for (const auto &face: figure.faces) {
            // Get the number of points in the face
            size_t numPoints = face.point_indexes.size();
            if (numPoints < 2) {
                continue; // Skip faces with less than 2 points
            }

            // Iterate through the point indexes and create lines
            for (size_t i = 0; i < numPoints; ++i) {
                // Get the current point and the next point (wrapping around)
                const auto &startPoint = figure.points[face.point_indexes[i]];
                const auto &endPoint = figure.points[face.point_indexes[(i + 1) % numPoints]];

                // Create a line from startPoint to endPoint and add it to the output
                out.emplace_back(img::Point2D(startPoint, 1), img::Point2D(endPoint, 1), figure.ambientReflection);
            }
        }
    }
    return out;
}

std::vector<img::Line2D> doProjectionZBuf(const std::vector<Figure> &figures) {
    std::vector<img::Line2D> out;
    for (const auto &figure: figures) {
        for (const auto &face: figure.faces) {
            // Get the number of points in the face
            size_t numPoints = face.point_indexes.size();
            if (numPoints < 2) {
                throw std::logic_error("Face with 2 points or less");
            }

            // Iterate through the point indexes and create lines
            for (size_t i = 0; i < numPoints; ++i) {
                // Get the current point and the next point (wrapping around)
                const auto &startPoint = figure.points[face.point_indexes[i]];
                const auto &endPoint = figure.points[face.point_indexes[(i + 1) % numPoints]];

                // Project points onto 2D space
                img::Point2D projectedStart(startPoint, 1);
                img::Point2D projectedEnd(endPoint, 1);

                // Create a line with proper z-values
                img::Line2D line(projectedStart, projectedEnd, figure.ambientReflection);
                line.z1 = 1 / startPoint.z; // Store original z-depth
                line.z2 = 1 / endPoint.z; // Store original z-depth

                out.emplace_back(line);
            }
        }
    }
    return out;
}


Matrix eyePointTrans(const Vector3D &eyepoint) {
    double r, phi, theta;
    toPolar(eyepoint, theta, phi, r);
    Matrix v;
    double phicos = std::cos(phi);
    double phisin = std::sin(phi);
    double thetacos = std::cos(theta);
    double thetasin = std::sin(theta);
    v(1, 1) = -thetasin;
    v(2, 1) = thetacos;
    v(1, 2) = -thetacos * phicos;
    v(2, 2) = -thetasin * phicos;
    v(3, 2) = phisin;
    v(1, 3) = thetacos * phisin;
    v(2, 3) = thetasin * phisin;
    v(3, 3) = phicos;
    v(4, 3) = -r;
    return v;
}

// Functions made for the session on 12/03/2025
Figure createCube() {
    Figure out;
    out.points = {
        Vector3D::point(1, -1, -1),
        Vector3D::point(-1, 1, -1),
        Vector3D::point(1, 1, 1),
        Vector3D::point(-1, -1, 1),
        Vector3D::point(1, 1, -1),
        Vector3D::point(-1, -1, -1),
        Vector3D::point(1, -1, 1),
        Vector3D::point(-1, 1, 1)
    };
    out.faces = {
        Face({0, 4, 2, 6}),
        Face({4, 1, 7, 2}),
        Face({1, 5, 3, 7}),
        Face({5, 0, 6, 3}),
        Face({6, 2, 7, 3}),
        Face({0, 5, 1, 4})
    };
    return out;
}

Figure createTetrahedron() {
    Figure out;
    out.points = {
        Vector3D::point(1, -1, -1),
        Vector3D::point(-1, 1, -1),
        Vector3D::point(1, 1, 1),
        Vector3D::point(-1, -1, 1),
    };
    out.faces = {
        Face({0, 1, 2}),
        Face({1, 3, 2}),
        Face({0, 3, 1}),
        Face({0, 2, 3})
    };
    return out;
}

Figure createOctahedron() {
    Figure out;
    out.points = {
        Vector3D::point(1, 0, 0),
        Vector3D::point(0, 1, 0),
        Vector3D::point(-1, 0, 0),
        Vector3D::point(0, -1, 0),
        Vector3D::point(0, 0, -1),
        Vector3D::point(0, 0, 1)
    };
    out.faces = {
        Face({0, 1, 5}),
        Face({1, 2, 5}),
        Face({2, 3, 5}),
        Face({3, 0, 5}),
        Face({1, 0, 4}),
        Face({2, 1, 4}),
        Face({3, 2, 4}),
        Face({0, 3, 4})
    };
    return out;
}

Figure createIcosahedron() {
    Figure out;
    constexpr double phi = 2 * M_PI / 5;
    constexpr double sqrt52 = std::sqrt(5) / 2;
    out.points.push_back(Vector3D::point(0, 0, sqrt52));

    // Generate two pentagonal rings
    for (int i = 0; i < 10; i++) {
        double angle = (i % 5) * phi + (i >= 5 ? M_PI / 5 : 0);
        out.points.push_back(Vector3D::point(
            std::cos(angle),
            std::sin(angle),
            (i < 5) ? 0.5 : -0.5
        ));
    }

    out.points.push_back(Vector3D::point(0, 0, -sqrt52));
    out.faces = {
        Face({0, 1, 2}),
        Face({0, 2, 3}),
        Face({0, 3, 4}),
        Face({0, 4, 5}),
        Face({0, 5, 1}),
        Face({1, 6, 2}),
        Face({2, 6, 7}),
        Face({2, 7, 3}),
        Face({3, 7, 8}),
        Face({3, 8, 4}),
        Face({4, 8, 9}),
        Face({4, 9, 5}),
        Face({5, 9, 10}),
        Face({5, 10, 1}),
        Face({1, 10, 6}),
        Face({11, 7, 6}),
        Face({11, 8, 7}),
        Face({11, 9, 8}),
        Face({11, 10, 9}),
        Face({11, 6, 10})
    };
    return out;
}

Figure createDodecahedron() {
    Figure icosa = createIcosahedron();
    Figure out;
    for (const auto &face: icosa.faces) {
        const auto &x1 = icosa.points[face.point_indexes[0]].x;
        const auto &y1 = icosa.points[face.point_indexes[0]].y;
        const auto &z1 = icosa.points[face.point_indexes[0]].z;

        const auto &x2 = icosa.points[face.point_indexes[1]].x;
        const auto &y2 = icosa.points[face.point_indexes[1]].y;
        const auto &z2 = icosa.points[face.point_indexes[1]].z;

        const auto &x3 = icosa.points[face.point_indexes[2]].x;
        const auto &y3 = icosa.points[face.point_indexes[2]].y;
        const auto &z3 = icosa.points[face.point_indexes[2]].z;

        Vector3D avgPoint = Vector3D::point((x1 + x2 + x3) / 3, (y1 + y2 + y3) / 3, (z1 + z2 + z3) / 3);
        avgPoint.normalise();
        out.points.push_back(avgPoint);
    }
    out.faces = {
        Face({0, 1, 2, 3, 4}),
        Face({0, 5, 6, 7, 1}),
        Face({1, 7, 8, 9, 2}),
        Face({2, 9, 10, 11, 3}),
        Face({3, 11, 12, 13, 4}),
        Face({4, 13, 14, 5, 0}),
        Face({19, 18, 17, 16, 15}),
        Face({19, 14, 13, 12, 18}),
        Face({18, 12, 11, 10, 17}),
        Face({17, 10, 9, 8, 16}),
        Face({16, 8, 7, 6, 15}),
        Face({15, 6, 5, 14, 19})
    };
    return out;
}

Figure createSphere(const unsigned int n) {
    Figure sphere = createIcosahedron();

    for (unsigned int i = 0; i < n; i++) {
        Figure newsphere;
        newsphere.points = sphere.points;

        std::map<std::pair<int, int>, int> midpointCache;

        auto getMidpoint = [&](int p1, int p2) -> int {
            if (p1 > p2) std::swap(p1, p2);
            auto key = std::make_pair(p1, p2);
            if (midpointCache.find(key) == midpointCache.end()) {
                auto midpoint = (newsphere.points[p1] + newsphere.points[p2]) / 2;
                newsphere.points.push_back(midpoint);
                midpointCache[key] = static_cast<int>(newsphere.points.size()) - 1;
            }
            return midpointCache[key];
        };

        for (const auto &face: sphere.faces) {
            unsigned int ai = face.point_indexes[0];
            unsigned int bi = face.point_indexes[1];
            unsigned int ci = face.point_indexes[2];

            unsigned int di = getMidpoint(ai, bi);
            unsigned int ei = getMidpoint(ai, ci);
            unsigned int fi = getMidpoint(bi, ci);

            newsphere.faces.push_back(Face({ai, di, ei}));
            newsphere.faces.push_back(Face({bi, fi, di}));
            newsphere.faces.push_back(Face({ci, ei, fi}));
            newsphere.faces.push_back(Face({di, fi, ei}));
        }

        sphere = std::move(newsphere);
    }
    for (auto &point: sphere.points) {
        point.normalise();
    }
    return sphere;
}

Figure createCone(double h, unsigned int n) {
    Figure out;
    for (unsigned int i = 0; i < n; i++) {
        double angle = 2 * M_PI * i / n;
        out.points.push_back(Vector3D::point(
            std::cos(angle),
            std::sin(angle),
            0));

        out.faces.push_back(Face({
            i,
            (i + 1) % n,
            n
        }));
    }
    out.points.push_back(Vector3D::point(0, 0, h));
    Face fn;
    for (int i = static_cast<int>(n); i > 0; --i) {
        fn.point_indexes.push_back(i);
    }
    out.faces.push_back(fn);
    return out;
}

Figure createCylinder(double h, unsigned int n) {
    Figure out;

    for (unsigned int i = 0; i < n; i++) {
        double angle = 2 * M_PI * i / n;
        out.points.push_back(Vector3D::point(
            std::cos(angle),
            std::sin(angle),
            0
        ));
    }

    for (unsigned int i = 0; i < n; i++) {
        double angle = 2 * M_PI * i / n;
        out.points.push_back(Vector3D::point(
            std::cos(angle),
            std::sin(angle),
            h
        ));
    }

    for (unsigned int i = 0; i < n; i++) {
        out.faces.push_back(Face({
            i,
            (i + 1) % n,
            (i + 1) % n + n,
            i + n
        }));
    }

    Face topFace;
    for (unsigned int i = 0; i < n; i++) {
        topFace.point_indexes.push_back(i + n);
    }
    out.faces.push_back(topFace);

    Face bottomFace;
    for (unsigned int i = 0; i < n; i++) {
        bottomFace.point_indexes.push_back(i);
    }
    out.faces.push_back(bottomFace);

    return out;
}


Figure createTorus(const double r, const double R, const int n, const int m) {
    Figure out;

    for (int i = 0; i < n; i++) {
        double u = 2 * i * M_PI / n;
        for (int j = 0; j < m; j++) {
            double v = 2 * j * M_PI / m;

            double x = (R + r * std::cos(v)) * std::cos(u);
            double y = (R + r * std::cos(v)) * std::sin(u);
            double z = r * std::sin(v);

            out.points.push_back(Vector3D::point(x, y, z));
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            unsigned int i1 = i;
            unsigned int i2 = (i + 1) % n;
            unsigned int j1 = j;
            unsigned int j2 = (j + 1) % m;

            unsigned int p1 = i1 * m + j1;
            unsigned int p2 = i2 * m + j1;
            unsigned int p3 = i2 * m + j2;
            unsigned int p4 = i1 * m + j2;

            out.faces.push_back(Face({p1, p2, p3, p4}));
        }
    }

    return out;
}

Figure create3DLsystem(const std::string &inputFile) {
    LParser::LSystem3D lsystem;
    std::ifstream file(inputFile);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + inputFile);
    }
    file >> lsystem;
    file.close();

    std::string currentString = lsystem.get_initiator();
    const std::set<char> &alphabet = lsystem.get_alphabet();
    for (unsigned int i = 0; i < lsystem.get_nr_iterations(); ++i) {
        std::string newString;
        for (char c: currentString) {
            if (alphabet.find(c) == alphabet.end()) {
                newString += c;
            } else {
                newString += lsystem.get_replacement(c);
            }
        }
        currentString = std::move(newString);
    }

    double d = degToRad(lsystem.get_angle());

    auto H = Vector3D::vector(1, 0, 0); // Current direction
    auto L = Vector3D::vector(0, 1, 0); // Left direction
    auto U = Vector3D::vector(0, 0, 1); // Up direction

    struct fullPositions {
        Vector3D pos, H, U, L;

        fullPositions(const Vector3D &pos, const Vector3D &h,
                      const Vector3D &u, const Vector3D &l)
            : pos(pos), H(h), U(u), L(l) {
        }
    };
    std::stack<fullPositions> stack;

    auto currentPosition = Vector3D::point(0, 0, 0);
    auto currentDirection = H; // Start with H as the current direction
    Figure out;
    for (char c: currentString) {
        if (c == '+') {
            // Rotate left around U
            double cosD = cos(d);
            double sinD = sin(d);
            auto Hnew = Vector3D::vector(
                H.x * cosD + L.x * sinD,
                H.y * cosD + L.y * sinD,
                H.z * cosD + L.z * sinD
            );
            auto Lnew = Vector3D::vector(
                -H.x * sinD + L.x * cosD,
                -H.y * sinD + L.y * cosD,
                -H.z * sinD + L.z * cosD
            );
            H = Hnew;
            L = Lnew;
        } else if (c == '-') {
            // Rotate right around U
            double cosD = cos(-d);
            double sinD = sin(-d);
            auto Hnew = Vector3D::vector(
                H.x * cosD + L.x * sinD,
                H.y * cosD + L.y * sinD,
                H.z * cosD + L.z * sinD
            );
            auto Lnew = Vector3D::vector(
                -H.x * sinD + L.x * cosD,
                -H.y * sinD + L.y * cosD,
                -H.z * sinD + L.z * cosD
            );
            H = Hnew;
            L = Lnew;
        } else if (c == '^') {
            // Pitch up around L
            double cosD = cos(d);
            double sinD = sin(d);
            auto Hnew = Vector3D::vector(
                H.x * cosD + U.x * sinD,
                H.y * cosD + U.y * sinD,
                H.z * cosD + U.z * sinD
            );
            auto Unew = Vector3D::vector(
                -H.x * sinD + U.x * cosD,
                -H.y * sinD + U.y * cosD,
                -H.z * sinD + U.z * cosD
            );
            H = Hnew;
            U = Unew;
        } else if (c == '&') {
            // Pitch down around L
            double cosD = cos(-d);
            double sinD = sin(-d);
            auto Hnew = Vector3D::vector(
                H.x * cosD + U.x * sinD,
                H.y * cosD + U.y * sinD,
                H.z * cosD + U.z * sinD
            );
            auto Unew = Vector3D::vector(
                -H.x * sinD + U.x * cosD,
                -H.y * sinD + U.y * cosD,
                -H.z * sinD + U.z * cosD
            );
            H = Hnew;
            U = Unew;
        } else if (c == '\\') {
            // Roll left around H
            double cosD = cos(d);
            double sinD = sin(d);
            auto Lnew = Vector3D::vector(
                L.x * cosD - U.x * sinD,
                L.y * cosD - U.y * sinD,
                L.z * cosD - U.z * sinD
            );
            auto Unew = Vector3D::vector(
                L.x * sinD + U.x * cosD,
                L.y * sinD + U.y * cosD,
                L.z * sinD + U.z * cosD
            );
            L = Lnew;
            U = Unew;
        } else if (c == '/') {
            // Roll right around H
            double cosD = cos(-d);
            double sinD = sin(-d);
            auto Lnew = Vector3D::vector(
                L.x * cosD - U.x * sinD,
                L.y * cosD - U.y * sinD,
                L.z * cosD - U.z * sinD
            );
            auto Unew = Vector3D::vector(
                L.x * sinD + U.x * cosD,
                L.y * sinD + U.y * cosD,
                L.z * sinD + U.z * cosD
            );
            L = Lnew;
            U = Unew;
        } else if (c == '|') {
            // Reverse direction
            H = Vector3D::vector(-H.x, -H.y, -H.z);
            L = Vector3D::vector(-L.x, -L.y, -L.z);
        } else if (c == '(') {
            // Save current state
            stack.emplace(currentPosition, H, U, L);
        } else if (c == ')') {
            // Restore previous state
            if (!stack.empty()) {
                auto &top = stack.top();
                currentPosition = top.pos;
                H = top.H;
                L = top.L;
                U = top.U;
                stack.pop();
            }
        } else if (lsystem.draw(c)) {
            out.points.push_back(currentPosition);
            currentPosition += H;
            out.points.push_back(currentPosition);
            out.faces.push_back(Face({
                static_cast<unsigned>(out.points.size() - 1), static_cast<unsigned>(out.points.size() - 2)
            }));
        } else {
            currentPosition += H;
        }
    }
    return out;
}

void generateFractal(const Figure &in, std::vector<Figure> &out, int nr_iterations, double scale) {
    if (nr_iterations == 0) {
        out.push_back(in);
        return;
    }

    std::vector<Figure> current_iteration_figures = {in};
    Matrix ms = scaleFigure(1.0 / scale);
    for (int i = 0; i < nr_iterations; ++i) {
        std::vector<Figure> next_iteration_figures;
        for (const auto &current_fig: current_iteration_figures) {
            for (size_t j = 0; j < current_fig.points.size(); ++j) {
                const auto &point = current_fig.points[j];
                Figure new_fig = current_fig;
                new_fig.applyTransformation(ms);
                const Vector3D &original_corresponding_point = current_fig.points[j];
                Vector3D scaled_corresponding_point = original_corresponding_point * ms;
                Vector3D translation = point - scaled_corresponding_point;
                Matrix mt = translate(translation);
                new_fig.applyTransformation(mt);
                next_iteration_figures.push_back(new_fig);
            }
        }
        current_iteration_figures = std::move(next_iteration_figures);
    }
    out.insert(out.end(), current_iteration_figures.begin(), current_iteration_figures.end());
}

std::vector<Figure> createFigures(const ini::Configuration &conf) {
    std::vector<double> eyexyz = conf["General"]["eye"].as_double_tuple_or_die();
    Vector3D eyepoint = Vector3D::vector(eyexyz[0], eyexyz[1], eyexyz[2]);

    int nrFigures = conf["General"]["nrFigures"].as_int_or_die();
    std::vector<Figure> figures;
    for (int i = 0; i < nrFigures; i++) {
        std::string name = "Figure" + std::to_string(i);
        std::string type = conf[name]["type"].as_string_or_die();

        Matrix transformatrix = scaleFigure(conf[name]["scale"].as_double_or_die());
        transformatrix *=
                rotateX(degToRad(conf[name]["rotateX"].as_double_or_die()));
        transformatrix *=
                rotateY(degToRad(conf[name]["rotateY"].as_double_or_die()));
        transformatrix *=
                rotateZ(degToRad(conf[name]["rotateZ"].as_double_or_die()));
        transformatrix *= translate(conf[name]["center"].as_double_tuple_or_die());
        transformatrix *= eyePointTrans(eyepoint);

        Figure figure;

        if (type == "LineDrawing") {
            int nrPoints = conf[name]["nrPoints"].as_int_or_die();

            for (int j = 0; j < nrPoints; j++) {
                std::vector<double> xyz =
                        conf[name]["point" + std::to_string(j)].as_double_tuple_or_die();
                figure.points.push_back(Vector3D::point(xyz[0], xyz[1], xyz[2]));
            }

            int nrLines = conf[name]["nrLines"].as_int_or_die();
            for (int j = 0; j < nrLines; j++) {
                Face face;
                auto intvec =
                        conf[name]["line" + std::to_string(j)].as_int_tuple_or_die();
                for (auto elem: intvec) {
                    face.point_indexes.push_back(static_cast<int>(elem));
                }
                figure.faces.emplace_back(face);
            }
        } else if (type == "Cube") {
            figure = createCube();
        } else if (type == "Tetrahedron") {
            figure = createTetrahedron();
        } else if (type == "Octahedron") {
            figure = createOctahedron();
        } else if (type == "Icosahedron") {
            figure = createIcosahedron();
        } else if (type == "Dodecahedron") {
            figure = createDodecahedron();
        } else if (type == "Sphere") {
            figure = createSphere(conf[name]["n"].as_int_or_die());
        } else if (type == "Cone") {
            figure = createCone(conf[name]["height"].as_double_or_die(),
                                conf[name]["n"].as_int_or_die());
        } else if (type == "Cylinder") {
            figure = createCylinder(conf[name]["height"].as_double_or_die(),
                                    conf[name]["n"].as_int_or_die());
        } else if (type == "Torus") {
            figure = createTorus(conf[name]["r"].as_double_or_die(),
                                 conf[name]["R"].as_double_or_die(),
                                 conf[name]["n"].as_int_or_die(),
                                 conf[name]["m"].as_int_or_die());
        } else if (type == "3DLSystem") {
            figure = create3DLsystem(conf[name]["inputfile"].as_string_or_die());
        } else if (type == "FractalCube") {
            figure = createCube();
            figure.applyTransformation(transformatrix);
            figure.ambientReflection = vecToColor(conf[name]["color"].as_double_tuple_or_die());
            generateFractal(figure, figures, conf[name]["nrIterations"].as_int_or_die(),
                            conf[name]["fractalScale"].as_double_or_die());
            continue;
        } else if (type == "FractalDodecahedron") {
            figure = createDodecahedron();
            figure.applyTransformation(transformatrix);
            figure.ambientReflection = vecToColor(conf[name]["color"].as_double_tuple_or_die());
            generateFractal(figure, figures, conf[name]["nrIterations"].as_int_or_die(),
                            conf[name]["fractalScale"].as_double_or_die());
            continue;
        } else if (type == "FractalIcosahedron") {
            figure = createIcosahedron();
            figure.applyTransformation(transformatrix);
            figure.ambientReflection = vecToColor(conf[name]["color"].as_double_tuple_or_die());
            generateFractal(figure, figures, conf[name]["nrIterations"].as_int_or_die(),
                            conf[name]["fractalScale"].as_double_or_die());
            continue;
        } else if (type == "FractalOctahedron") {
            figure = createOctahedron();
            figure.applyTransformation(transformatrix);
            figure.ambientReflection = vecToColor(conf[name]["color"].as_double_tuple_or_die());
            generateFractal(figure, figures, conf[name]["nrIterations"].as_int_or_die(),
                            conf[name]["fractalScale"].as_double_or_die());
            continue;
        } else if (type == "FractalTetrahedron") {
            figure = createTetrahedron();
            figure.applyTransformation(transformatrix);
            figure.ambientReflection = vecToColor(conf[name]["color"].as_double_tuple_or_die());
            generateFractal(figure, figures, conf[name]["nrIterations"].as_int_or_die(),
                            conf[name]["fractalScale"].as_double_or_die());
            continue;
        } else if (type == "MengerSponge") {
            std::cout << "[Warning]: MengerSponge is not implemented, putting in cube in stead" << std::endl;
            figure = createCube();
        } else if (type == "BuckyBall") {
            std::cout << "[Warning]: BuckyBall is not implemented, putting in dodecahedron in stead" << std::endl;
            figure = createDodecahedron();
        } else if (type == "FractalBuckyBall") {
            std::cout << "[Warning]: FractalBuckyBall is not implemented, putting in FractalDodecahedron in stead" <<
                    std::endl;
            figure = createDodecahedron();
            figure.applyTransformation(transformatrix);
            figure.ambientReflection = vecToColor(conf[name]["color"].as_double_tuple_or_die());
            generateFractal(figure, figures, conf[name]["nrIterations"].as_int_or_die(),
                            conf[name]["fractalScale"].as_double_or_die());
            continue;
        } else {
            // This code should never be reached
            throw std::logic_error("Creating figure of type '" + type +
                                   "' is not implemented");
        }
        figure.applyTransformation(transformatrix);
        figure.ambientReflection = vecToColor(conf[name]["color"].as_double_tuple_or_die());
        figures.push_back(figure);
    }
    return figures;
}

bool drawWireframe(const ini::Configuration &conf, img::EasyImage &image) {
    if (conf["General"]["type"].as_string_or_die() != "Wireframe") {
        return false;
    }
    auto figures = createFigures(conf);
    std::vector<img::Line2D> lines = doProjectionNoZBuf(figures);
    if (lines.empty()) {
        throw std::logic_error("The lines vector is empty");
    }
    image = img::EasyImage::draw2DLinesNoZBuf(
        lines, conf["General"]["size"].as_int_or_die(),
        vecToColor(
            conf["General"]["backgroundcolor"].as_double_tuple_or_die()));
    return true;
}

bool drawZBufferedWireframe(const ini::Configuration &conf, img::EasyImage &image) {
    if (conf["General"]["type"].as_string_or_die() != "ZBufferedWireframe") {
        return false;
    }
    auto figures = createFigures(conf);
    // Else do projection with ZBuffering
    std::vector<img::Line2D> lines = doProjectionZBuf(figures);
    if (lines.empty()) {
        throw std::logic_error("The lines vector is empty");
    }
    double d, dx, dy;
    image = img::EasyImage::draw2DLinesZBuf(
        lines, conf["General"]["size"].as_int_or_die(),
        vecToColor(conf["General"]["backgroundcolor"].as_double_tuple_or_die()), d, dx, dy);
    return true;
}

bool drawZBuffering(const ini::Configuration &conf, img::EasyImage &image) {
    if (conf["General"]["type"].as_string_or_die() != "ZBuffering") {
        return false;
    }
    auto size = conf["General"]["size"].as_int_or_die();
    auto figures = createFigures(conf);
    for (auto &fig: figures) {
        fig.triangulate();
    }

    std::vector<img::Line2D> lines = doProjectionZBuf(figures);

    // Check if lines vector is empty
    if (lines.empty()) {
        throw std::invalid_argument("The lines vector is empty.");
    }

    // Check if size is valid
    if (size <= 0) {
        throw std::invalid_argument("Size parameter must be positive.");
    }

    // Initialize min/max bounds
    double xmin = lines[0].p1.x;
    double xmax = xmin;
    double ymin = lines[0].p1.y;
    double ymax = ymin;

    // Iterate over all lines and their points to find the min/max bounds
    for (const auto &line: lines) {
        for (const img::Point2D &point: {line.p1, line.p2}) {
            // Check for invalid coordinates
            if (std::isnan(point.x) || std::isnan(point.y) || std::isinf(point.x) || std::isinf(point.y)) {
                throw std::invalid_argument("Invalid coordinates in lines vector.");
            }
            xmin = std::min(xmin, point.x);
            xmax = std::max(xmax, point.x);
            ymin = std::min(ymin, point.y);
            ymax = std::max(ymax, point.y);
        }
    }

    // Calculate ranges
    double xrange = xmax - xmin;
    double yrange = ymax - ymin;

    // Check for zero range
    if (xrange == 0 || yrange == 0) {
        throw std::invalid_argument("All points have the same x or y coordinate.");
    }

    // Calculate image dimensions
    double imagex = size * (xrange / std::max(xrange, yrange));
    double imagey = size * (yrange / std::max(xrange, yrange));

    // Check for invalid image dimensions
    if (imagex <= 0 || imagey <= 0) {
        throw std::invalid_argument("Invalid image dimensions (width or height is zero or negative).");
    }

    // Calculate scaling and translation factors
    double d = 0.95 * (imagex / xrange);
    double DCx = d * ((xmin + xmax) / 2);
    double DCy = d * ((ymin + ymax) / 2);
    double dx = (imagex / 2) - DCx;
    double dy = (imagey / 2) - DCy;

    // Create the image
    image = img::EasyImage(std::floor(imagex), std::floor(imagey));
    image.clear(vecToColor(conf["General"]["backgroundcolor"].as_double_tuple_or_die()));
    if (lines.empty()) {
        throw std::logic_error("The lines vector is empty");
    }

    img::ZBuffer zbuf(std::floor(imagex), std::floor(imagey));

    for (const auto &fig: figures) {
        for (const auto &face: fig.faces) {
            const auto &poi = face.point_indexes;
            image.draw_zbuf_triangle(zbuf, fig.points[poi[0]], fig.points[poi[1]], fig.points[poi[2]], d, dx, dy,
                                     fig.ambientReflection);
        }
    }
    return true;
}
