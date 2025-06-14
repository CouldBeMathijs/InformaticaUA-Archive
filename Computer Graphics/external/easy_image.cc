/*
 * easy_image.cc
 * Copyright (C) 2011  Daniel van den Akker
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "easy_image.h"

#include "vector3d.h"

#ifndef le32toh
#define le32toh(x) (x)
#endif

namespace {
    //structs borrowed from wikipedia's article on the BMP file format
    struct bmpfile_magic {
        uint8_t magic[2];
    };

    struct bmpfile_header {
        uint32_t file_size;
        uint16_t reserved_1;
        uint16_t reserved_2;
        uint32_t bmp_offset;
    };

    struct bmp_header {
        uint32_t header_size;
        int32_t width;
        int32_t height;
        uint16_t nplanes;
        uint16_t bits_per_pixel;
        uint32_t compress_type;
        uint32_t pixel_size;
        int32_t hres;
        int32_t vres;
        uint32_t ncolors;
        uint32_t nimpcolors;
    };

    //copy-pasted from lparser.cc to allow these classes to be used independently from each other
    class enable_exceptions {
    private:
        std::ios &ios;
        std::ios::iostate state;

    public:
        enable_exceptions(std::ios &an_ios, std::ios::iostate exceptions) : ios(an_ios) {
            state = ios.exceptions();
            ios.exceptions(exceptions);
        }

        ~enable_exceptions() {
            ios.exceptions(state);
        }
    };

    //helper function to convert a number (char, int, ...) to little endian
    //regardless of the endiannes of the system
    //more efficient machine-dependent functions exist, but this one is more portable
    template<typename T>
    T to_little_endian(T value) {
        //yes, unions must be used with caution, but this is a case in which a union is needed
        union {
            T t;
            uint8_t bytes[sizeof(T)];
        } temp_storage;

        for (uint8_t i = 0; i < sizeof(T); i++) {
            temp_storage.bytes[i] = value & 0xFF;
            value >>= 8;
        }
        return temp_storage.t;
    }

    template<typename T>
    T from_little_endian(T value) {
        //yes, unions must be used with caution, but this is a case in which a union is needed
        union {
            T t;
            uint8_t bytes[sizeof(T)];
        } temp_storage;
        temp_storage.t = value;
        T retVal = 0;

        for (uint8_t i = 0; i < sizeof(T); i++) {
            retVal = (retVal << 8) | temp_storage.bytes[sizeof(T) - i - 1];
        }
        return retVal;
    }
}

img::Color::Color() : blue(0), green(0), red(0) {
}

img::Color::Color(uint8_t r, uint8_t g, uint8_t b) : blue(b), green(g), red(r) {
}

img::Color::~Color() {
}


img::UnsupportedFileTypeException::UnsupportedFileTypeException(std::string const &msg) : message(msg) {
}

img::UnsupportedFileTypeException::UnsupportedFileTypeException(const UnsupportedFileTypeException &original)
    : std::exception(original)
      , message(original.message) {
}

img::UnsupportedFileTypeException::~UnsupportedFileTypeException() throw () {
}

img::UnsupportedFileTypeException &img::UnsupportedFileTypeException::operator=(
    UnsupportedFileTypeException const &original) {
    this->message = original.message;
    return *this;
}

const char *img::UnsupportedFileTypeException::what() const throw() {
    return message.c_str();
}

void img::Point2D::doProjectionOnPoint(const Vector3D &point, double d) {
    if (point.z >= 0) {
        throw std::invalid_argument("Z point of coordinate should always be negative!");
    }
    x = (d * point.x) / -point.z;
    y = (d * point.y) / -point.z;
}

img::Point2D::Point2D(const Vector3D &point, double d) {
    if (point.z >= 0) {
        throw std::invalid_argument("Z point of coordinate should always be negative!");
    }
    x = (d * point.x) / -point.z;
    y = (d * point.y) / -point.z;
}

img::ZBuffer::ZBuffer(const int width, const int height): std::vector<std::vector<double> >(
    width, std::vector<double>(height, std::numeric_limits<double>::infinity())) {
}

bool img::ZBuffer::isValid(const int x, const int y, const double zvalue) {
    if (x >= (*this).size() || y >= (*this)[0].size() || x < 0 || y < 0) {
        return false;
    }

    double currentZ = (*this)[x][y];

    if (zvalue < currentZ) {
        (*this)[x][y] = zvalue;
        return true;
    }

    return false;
}

img::EasyImage::EasyImage() : width(0), height(0), bitmap() {
}

img::EasyImage::EasyImage(unsigned int _width, unsigned int _height, Color color) : width(_width), height(_height),
    bitmap(width * height, color) {
}

img::EasyImage::EasyImage(EasyImage const &img) : width(img.width), height(img.height), bitmap(img.bitmap) {
}

img::EasyImage::~EasyImage() {
    bitmap.clear();
}

img::EasyImage &img::EasyImage::operator=(img::EasyImage const &img) {
    width = img.width;
    height = img.height;
    bitmap.assign(img.bitmap.begin(), img.bitmap.end());
    return (*this);
}

unsigned int img::EasyImage::get_width() const {
    return width;
}

unsigned int img::EasyImage::get_height() const {
    return height;
}

void img::EasyImage::clear(Color color) {
    for (std::vector<Color>::iterator i = bitmap.begin(); i != bitmap.end(); i++) {
        *i = color;
    }
}

img::Color &img::EasyImage::operator()(unsigned int x, unsigned int y) {
    assert(x < this->width);
    assert(y < this->height);
    return bitmap.at(x * height + y);
}

img::Color const &img::EasyImage::operator()(unsigned int x, unsigned int y) const {
    assert(x < this->width);
    assert(y < this->height);
    return bitmap.at(x * height + y);
}

void img::EasyImage::draw_line(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, Color color) {
    if (x0 >= this->width || y0 >= this->height || x1 >= this->width || y1 > this->height) {
        std::stringstream ss;
        ss << "Drawing line from (" << x0 << "," << y0 << ") to (" << x1 << "," << y1 << ") in image of width "
                << this->width << " and height " << this->height;
        throw std::runtime_error(ss.str());
    }
    if (x0 == x1) {
        // Special case for vertical line
        for (int i = (int) (std::max(y0, y1) - std::min(y0, y1)); i >= 0; i--) {
            (*this)(x0, std::min(y0, y1) + i) = color;
        }
    } else if (y0 == y1) {
        // Special case for horizontal line
        for (int i = (int) (std::max(x0, x1) - std::min(x0, x1)); i >= 0; i--) {
            (*this)(std::min(x0, x1) + i, y0) = color;
        }
    } else {
        if (x0 > x1) {
            // Flip points if x1 > x0 to ensure x0 has the lowest value
            std::swap(x0, x1);
            std::swap(y0, y1);
        }
        double m = ((double) y1 - (double) y0) / ((double) x1 - (double) x0);
        if (-1.0 <= m && m <= 1.0) {
            for (int i = (int) (x1 - x0); i >= 0; i--) {
                (*this)(x0 + i, (unsigned int) round(y0 + m * i)) = color;
            }
        } else if (m > 1.0) {
            for (int i = (int) (y1 - y0); i >= 0; i--) {
                (*this)((unsigned int) round(x0 + (i / m)), y0 + i) = color;
            }
        } else if (m < -1.0) {
            for (int i = (int) (y0 - y1); i >= 0; i--) {
                (*this)((unsigned int) round(x0 - (i / m)), y0 - i) = color;
            }
        }
    }
}

void img::EasyImage::draw_line(const Line2D &line) {
    draw_line(std::lround(line.p1.x), std::lround(line.p1.y),
              std::lround(line.p2.x), std::lround(line.p2.y), line.color);
}

void img::EasyImage::draw_zbuf_line(ZBuffer &zBuffer,
                                    unsigned int x0,
                                    unsigned int y0, double z0,
                                    unsigned int x1,
                                    unsigned int y1, double z1,
                                    const img::Color &color) {
    // Bounds isValid
    if (x0 >= this->width || y0 >= this->height || x1 >= this->width || y1 >= this->height) {
        std::stringstream ss;
        ss << "Drawing line from (" << x0 << "," << y0 << ") to (" << x1 << "," << y1 << ") in image of width "
                << this->width << " and height " << this->height;
        throw std::runtime_error(ss.str());
    }

    // Ensure x0 <= x1 for simpler looping logic
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
        std::swap(z0, z1);
    }

    // Calculate the slope for line interpolation
    double m = (x1 != x0) ? ((double) y1 - (double) y0) / ((double) x1 - (double) x0) : 0.0; // Slope calculation
    bool isVertical = (x0 == x1); // Check if it's a vertical line
    bool isHorizontal = (y0 == y1); // Check if it's a horizontal line

    if (isVertical) {
        // Vertical line, iterate over y values
        if (y0 > y1) {
            std::swap(y0, y1);
            std::swap(z0, z1);
        }
        for (unsigned int y = y0; y <= y1; ++y) {
            double z = z0 + (z1 - z0) * (double(y - y0) / (y1 - y0));

            if (zBuffer[x0][y] > z) {
                (*this)(x0, y) = color;
                zBuffer[x0][y] = z;
            }
        }
    } else if (isHorizontal) {
        // Horizontal line, iterate over x values
        for (unsigned int x = x0; x <= x1; ++x) {
            double z = z0 + (z1 - z0) * (double(x - x0) / (x1 - x0));

            if (zBuffer[x][y0] > z) {
                (*this)(x, y0) = color;
                zBuffer[x][y0] = z;
            }
        }
    } else {
        // Non-vertical, non-horizontal line, interpolate along both axes
        if (abs(x1 - x0) >= abs(y1 - y0)) {
            // Shallow slope (|m| <= 1), iterate over x
            for (unsigned int x = x0; x <= x1; ++x) {
                unsigned int y = (unsigned int) round(y0 + m * (x - x0));
                double z = z0 + (z1 - z0) * (double(x - x0) / (x1 - x0));

                if (zBuffer[x][y] > z) {
                    (*this)(x, y) = color;
                    zBuffer[x][y] = z;
                }
            }
        } else {
            // Steep slope (|m| > 1), iterate over y
            if (y0 > y1) {
                std::swap(y0, y1);
                std::swap(x0, x1);
                std::swap(z0, z1);
            }
            m = ((double) y1 - (double) y0) / ((double) x1 - (double) x0); // Recalculate slope for steep lines
            for (unsigned int y = y0; y <= y1; ++y) {
                unsigned int x = (unsigned int) round(x0 + (y - y0) / m);
                double z = z0 + (z1 - z0) * (double(y - y0) / (y1 - y0));

                if (zBuffer[x][y] > z) {
                    (*this)(x, y) = color;
                    zBuffer[x][y] = z;
                }
            }
        }
    }
}

void img::EasyImage::draw_zbuf_line(const Line2D &line, ZBuffer &zbuf) {
    draw_zbuf_line(zbuf, std::lround(line.p1.x), std::lround(line.p1.y), line.z1, std::lround(line.p2.x),
                   std::lround(line.p2.y), line.z2, line.color);
}

void img::EasyImage::draw_zbuf_triangle(ZBuffer &zbuf, const Vector3D &A,
                                        const Vector3D &B, const Vector3D &C,
                                        double d, double dx, double dy,
                                        const img::Color &color) {
    // Project 3D points to 2D screen space
    img::Point2D A_accent(A, d);
    img::Point2D B_accent(B, d);
    img::Point2D C_accent(C, d);

    // Apply translation
    A_accent.x += dx;
    A_accent.y += dy;
    B_accent.x += dx;
    B_accent.y += dy;
    C_accent.x += dx;
    C_accent.y += dy;

    // Determine the bounding box of the triangle
    int ymin = static_cast<int>(std::floor(std::min({A_accent.y, B_accent.y, C_accent.y})));
    int ymax = static_cast<int>(std::ceil(std::max({A_accent.y, B_accent.y, C_accent.y})));

    // Loop over each pixel within the bounding box
    for (int y = ymin; y <= ymax; ++y) {
        // For each y, calculate the leftmost and rightmost x-values of the triangle
        double xL = std::numeric_limits<double>::infinity();
        double xR = -std::numeric_limits<double>::infinity();

        // Find the intersections of the triangle edges with the current horizontal line y
        if ((y - A_accent.y) * (y - B_accent.y) <= 0) {
            xL = std::min(xL, A_accent.x + (B_accent.x - A_accent.x) * (y - A_accent.y) / (B_accent.y - A_accent.y));
            xR = std::max(xR, A_accent.x + (B_accent.x - A_accent.x) * (y - A_accent.y) / (B_accent.y - A_accent.y));
        }
        if ((y - A_accent.y) * (y - C_accent.y) <= 0) {
            xL = std::min(xL, A_accent.x + (C_accent.x - A_accent.x) * (y - A_accent.y) / (C_accent.y - A_accent.y));
            xR = std::max(xR, A_accent.x + (C_accent.x - A_accent.x) * (y - A_accent.y) / (C_accent.y - A_accent.y));
        }
        if ((y - B_accent.y) * (y - C_accent.y) <= 0) {
            xL = std::min(xL, B_accent.x + (C_accent.x - B_accent.x) * (y - B_accent.y) / (C_accent.y - B_accent.y));
            xR = std::max(xR, B_accent.x + (C_accent.x - B_accent.x) * (y - B_accent.y) / (C_accent.y - B_accent.y));
        }

        // Check if the triangle is valid for the current y value
        if (xL == std::numeric_limits<double>::infinity() || xR == -std::numeric_limits<double>::infinity())
            continue; // No valid x-range for this y

        // Iterate over each x in the current row
        for (int x = static_cast<int>(std::ceil(xL)); x <= static_cast<int>(std::floor(xR)); ++x) {
            // Compute the z value for this (x, y) pixel using the barycentric coordinates
            double lambdaA = ((B_accent.y - C_accent.y) * (x - C_accent.x) + (C_accent.x - B_accent.x) * (y - C_accent.y)) /
                             ((B_accent.y - C_accent.y) * (A_accent.x - C_accent.x) + (C_accent.x - B_accent.x) * (A_accent.y - C_accent.y));
            double lambdaB = ((C_accent.y - A_accent.y) * (x - C_accent.x) + (A_accent.x - C_accent.x) * (y - C_accent.y)) /
                             ((B_accent.y - C_accent.y) * (A_accent.x - C_accent.x) + (C_accent.x - B_accent.x) * (A_accent.y - C_accent.y));
            double lambdaC = 1.0 - lambdaA - lambdaB;

            // Compute z value at the pixel (x, y)
            double z = (lambdaA / A.z + lambdaB / B.z + lambdaC / C.z);

            // Check the z-buffer to see if we should draw this pixel
            if (zbuf.isValid(x, y, z)) {
                // If we pass the z-buffer test, set the pixel color
                (*this)(x, y) = color;
            }
        }
    }
}


img::EasyImage img::EasyImage::draw2DLinesNoZBuf(std::vector<Line2D> &lines, const int size,
                                                 const img::Color &backgroundcolor) {
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
        for (const Point2D &point: {line.p1, line.p2}) {
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
    EasyImage image(std::lround(imagex), std::lround(imagey));
    image.clear(backgroundcolor);

    // Draw the lines
    for (auto &line: lines) {
        line.p1.x *= d;
        line.p1.y *= d;
        line.p2.x *= d;
        line.p2.y *= d;
        line.p1.x += dx;
        line.p1.y += dy;
        line.p2.x += dx;
        line.p2.y += dy;
        image.draw_line(line);
    }

    return image;
}

img::EasyImage img::EasyImage::draw2DLinesZBuf(std::vector<Line2D> &lines, const int size,
                                               const img::Color &backgroundcolor, double &d, double & dx, double & dy) {
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
        for (const Point2D &point: {line.p1, line.p2}) {
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
    d = 0.95 * (imagex / xrange);
    double DCx = d * ((xmin + xmax) / 2);
    double DCy = d * ((ymin + ymax) / 2);
    dx = (imagex / 2) - DCx;
    dy = (imagey / 2) - DCy;

    // Create the image
    EasyImage image(std::lround(imagex), std::lround(imagey));
    image.clear(backgroundcolor);
    ZBuffer zbuf(image.width, image.height);

    // Draw the lines
    for (auto &line: lines) {
        line.p1.x *= d;
        line.p1.y *= d;
        line.p2.x *= d;
        line.p2.y *= d;
        line.p1.x += dx;
        line.p1.y += dy;
        line.p2.x += dx;
        line.p2.y += dy;
        image.draw_zbuf_line(line, zbuf);
    }

    return image;
}

std::ostream &img::operator<<(std::ostream &out, EasyImage const &image) {
    //temporaryily enable exceptions on output stream
    enable_exceptions(out, std::ios::badbit | std::ios::failbit);
    //declare some struct-vars we're going to need:
    bmpfile_magic magic;
    bmpfile_header file_header;
    bmp_header header;
    uint8_t padding[] =
            {0, 0, 0, 0};
    //calculate the total size of the pixel data
    unsigned int line_width = image.get_width() * 3; //3 bytes per pixel
    unsigned int line_padding = 0;
    if (line_width % 4 != 0) {
        line_padding = 4 - (line_width % 4);
    }
    //lines must be aligned to a multiple of 4 bytes
    line_width += line_padding;
    unsigned int pixel_size = image.get_height() * line_width;

    //start filling the headers
    magic.magic[0] = 'B';
    magic.magic[1] = 'M';

    file_header.file_size = to_little_endian(pixel_size + sizeof(file_header) + sizeof(header) + sizeof(magic));
    file_header.bmp_offset = to_little_endian(sizeof(file_header) + sizeof(header) + sizeof(magic));
    file_header.reserved_1 = 0;
    file_header.reserved_2 = 0;
    header.header_size = to_little_endian(sizeof(header));
    header.width = to_little_endian(image.get_width());
    header.height = to_little_endian(image.get_height());
    header.nplanes = to_little_endian(1);
    header.bits_per_pixel = to_little_endian(24); //3bytes or 24 bits per pixel
    header.compress_type = 0; //no compression
    header.pixel_size = pixel_size;
    header.hres = to_little_endian(11811); //11811 pixels/meter or 300dpi
    header.vres = to_little_endian(11811); //11811 pixels/meter or 300dpi
    header.ncolors = 0; //no color palette
    header.nimpcolors = 0; //no important colors

    //okay that should be all the header stuff: let's write it to the stream
    out.write((char *) &magic, sizeof(magic));
    out.write((char *) &file_header, sizeof(file_header));
    out.write((char *) &header, sizeof(header));

    //okay let's write the pixels themselves:
    //they are arranged left->right, bottom->top, b,g,r
    for (unsigned int i = 0; i < image.get_height(); i++) {
        //loop over all lines
        for (unsigned int j = 0; j < image.get_width(); j++) {
            //loop over all pixels in a line
            //we cast &color to char*. since the color fields are ordered blue,green,red they should be written automatically
            //in the right order
            out.write((char *) &image(j, i), 3 * sizeof(uint8_t));
        }
        if (line_padding > 0)
            out.write((char *) padding, line_padding);
    }
    //okay we should be done
    return out;
}

std::istream &img::operator>>(std::istream &in, EasyImage &image) {
    enable_exceptions(in, std::ios::badbit | std::ios::failbit);
    // declare some struct-vars we're going to need
    bmpfile_magic magic;
    bmpfile_header file_header;
    bmp_header header;
    // a temp buffer for reading the padding at the end of each line
    uint8_t padding[] = {0, 0, 0, 0};

    // read the headers && do some sanity checks
    in.read((char *) &magic, sizeof(magic));
    if (magic.magic[0] != 'B' || magic.magic[1] != 'M')
        throw UnsupportedFileTypeException(
            "Could not parse BMP File: invalid magic header");
    in.read((char *) &file_header, sizeof(file_header));
    in.read((char *) &header, sizeof(header));
    if (le32toh(header.pixel_size) + le32toh(file_header.bmp_offset) !=
        le32toh(file_header.file_size))
        throw UnsupportedFileTypeException(
            "Could not parse BMP File: file size mismatch");
    if (le32toh(header.header_size) != sizeof(header))
        throw UnsupportedFileTypeException(
            "Could not parse BMP File: Unsupported BITMAPV5HEADER size");
    if (le32toh(header.compress_type) != 0)
        throw UnsupportedFileTypeException(
            "Could not parse BMP File: Only uncompressed BMP files can be parsed");
    if (le32toh(header.nplanes) != 1)
        throw UnsupportedFileTypeException("Could not parse BMP File: Only one "
            "plane should exist in the BMP file");
    if (le32toh(header.bits_per_pixel) != 24)
        throw UnsupportedFileTypeException(
            "Could not parse BMP File: Only 24bit/pixel BMP's are supported");
    // if height<0 -> read top to bottom instead of bottom to top
    bool invertedLines = from_little_endian(header.height) < 0;
    image.height = std::abs(from_little_endian(header.height));
    image.width = std::abs(from_little_endian(header.width));
    unsigned int line_padding =
            from_little_endian(header.pixel_size) / image.height - (3 * image.width);
    // re-initialize the image bitmap
    image.bitmap.clear();
    image.bitmap.assign(image.height * image.width, Color());
    // okay let's read the pixels themselves:
    // they are arranged left->right., bottom->top if height>0, top->bottom if
    // height<0, b,g,r
    for (unsigned int i = 0; i < image.get_height(); i++) {
        // loop over all lines
        for (unsigned int j = 0; j < image.get_width(); j++) {
            // loop over all pixels in a line
            // we cast &color to char*. since the color fields are ordered
            // blue,green,red, the data read should be written in the right variables
            if (invertedLines) {
                // store top-to-bottom
                in.read((char *) &image(j, image.height - 1 - i), 3 * sizeof(uint8_t));
            } else {
                // store bottom-to-top
                in.read((char *) &image(j, i), 3 * sizeof(uint8_t));
            }
        }
        if (line_padding > 0) {
            in.read((char *) padding, line_padding);
        }
    }
    // okay we're done
    return in;
}

ZBuffer::ZBuffer(const unsigned int width, const unsigned int height)
    : std::vector<std::vector<double> >(
        height, std::vector(
            width, std::numeric_limits<double>::infinity())) {
}
