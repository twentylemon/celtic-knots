

#include "CelticGrid.h"

const static int DEFAULT_CELL_SIZE = 20;
const static int DEFAULT_STROKE = 2;

CelticGrid::CelticGrid(int width, int height) :
    width_(width),
    height_(height),
    cellSize_(DEFAULT_CELL_SIZE),
    stroke_(DEFAULT_STROKE)
{
}


CelticGrid::CelticGrid(std::string inFile) :
    cellSize_(DEFAULT_CELL_SIZE),
    stroke_(DEFAULT_STROKE)
{
    std::ifstream in(inFile);
    int x, y;
    char dir;
    in >> width_ >> height_ >> x;   // last digit is useless here
    cells_ = lemon::Vector<lemon::Vector<CelticCell>>(width(), lemon::Vector<CelticCell>(height(), CelticCell()));
    while (in >> x >> y >> dir) {
        addMarker(x, y, dir=='e'||dir=='E' ? BreakMarker::East : BreakMarker::South);
    }
    in.close();
}

void CelticGrid::save(std::string outFile) const {
    std::ofstream out(outFile);
    out << width() << "\t" << height() << "\t" << markers().size() << std::endl;
    markers_.dump(out);
    out.close();
}


int CelticGrid::width() const {
    return width_;
}

int CelticGrid::height() const {
    return height_;
}

int CelticGrid::cellSize() const {
    return cellSize_;
}

int CelticGrid::stroke() const {
    return stroke_;
}

lemon::Vector<BreakMarker>& CelticGrid::markers() {
    return markers_;
}

const lemon::Vector<BreakMarker>& CelticGrid::markers() const {
    return markers_;
}

lemon::Vector<lemon::Vector<CelticCell>>& CelticGrid::cells() {
    return cells_;
}

const lemon::Vector<lemon::Vector<CelticCell>>& CelticGrid::cells() const {
    return cells_;
}

void CelticGrid::setCellSize(int cellSize) {
    cellSize_ = cellSize;
}

void CelticGrid::setStroke(int stroke) {
    stroke_ = stroke;
}


void CelticGrid::addMarker(const BreakMarker& marker) {
    addMarker(marker.x(), marker.y(), marker.direction());
}

void CelticGrid::addMarker(int x, int y, BreakMarker::Direction direction) {
    markers_.emplace_back(x, y, direction); // add the marker
    if (direction == BreakMarker::East) {
        if (y > 0) {
            cells_[x][y-1].down = cells_[x+1][y-1].down = true; // marker in that direction
        }
        if (y < height()) {
            cells_[x][y].up = cells_[x+1][y].up = true;
        }
    }
    else {
        if (x > 0) {
            cells_[x-1][y].right = cells_[x-1][y+1].right = true;
        }
        if (x < width()) {
            cells_[x][y].left = cells_[x][y+1].left = true;
        }
    }
}


void CelticGrid::glRender(int windowWidth, int windowHeight) const {
    glMatrixMode(GL_PROJECTION | GL_MATRIX_MODE);
    glLoadIdentity();
    glOrtho(-10, windowWidth+10, windowHeight+10, -10, 0, 1);

    // points on the grid
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    for (int x = 0; x <= width(); x+=2) {
        for (int y = 0; y <= height(); y+=2) {
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex2i(x * cellSize(), y * cellSize());
            glColor3f(0.0f, 1.0f, 0.0f);    // will draw off the window, but whatever
            glVertex2i((x+1) * cellSize(), (y+1)*cellSize());
        }
    }
    glEnd();

    // break markers
    glLineWidth(3.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    markers_.each([this](const BreakMarker& marker){
        glVertex2i(marker.x() * cellSize(), marker.y() * cellSize());
        if (marker.direction() == BreakMarker::East) {
            glVertex2i((marker.x()+2) * cellSize(), marker.y() * cellSize());
        }
        else {
            glVertex2i(marker.x() * cellSize(), (marker.y()+2) * cellSize());
        }
    });
    glEnd();
}


std::ostream& operator<<(std::ostream& out, const CelticGrid& knot) {
    return out;
}
