#include "General.h"


#include <fstream>
#include <cmath>

#define POSINF (std::numeric_limits<double>::infinity())
#define NEGINF (-std::numeric_limits<double>::infinity())

Face::Face(const std::vector<unsigned int> &point_indexes)
    : point_indexes(point_indexes) {
}

std::vector<Face> Face::triangulate() const {
    std::vector<Face> out;

    if (point_indexes.size() < 3) {
        throw std::runtime_error("Cannot triangulate a face with fewer than 3 points.");
    }

    for (size_t i = 1; i < point_indexes.size() - 1; i++) {
        out.emplace_back(Face({point_indexes[0], point_indexes[i], point_indexes[i + 1]}));
    }

    return out;
}

void Figure::triangulate() {
    std::vector<Face> allTriangulatedFaces;
    for (const auto &face: faces) {
        std::vector<Face> newfaces = face.triangulate();
        allTriangulatedFaces.insert(allTriangulatedFaces.end(), newfaces.begin(), newfaces.end());
    }
    faces = std::move(allTriangulatedFaces);
}

void Figure::applyTransformation(const Matrix &m) {
    for (auto &point: points) {
        point *= m;
    }
}

bool fileExistsAndNotEmpty(const std::string &path) {
    std::ifstream fin(path);
    return fin.good() && fin.peek() != std::istream::traits_type::eof();
}

// Convertion functions
uint8_t convertTo255(const double &color) {
    // Convert a float between [0, 1] to an integer between [0, 255]
    if (color < 0 || color > 1) {
        std::cerr << "Tried to convert " << color << "to 255, but it isn't [0,1]" << std::endl;
        return 0;
    }
    return std::lround(color * 255);
}

img::Color vecToColor(const std::vector<double> &vec) {
    // Converts a vector of doubles as given by the .ini files into a color object
    if (vec.size() != 3) {
        std::cerr << "Vector of size " << vec.size() << " cannot be converted to Color object" << std::endl;
        return {};
    }
    return {convertTo255(vec[0]), convertTo255(vec[1]), convertTo255(vec[2])};
}

double degToRad(const double &deg) {
    // Takes in some degrees and converts them to radials
    constexpr double pi180 = M_PI / 180;
    return deg * pi180;
}
