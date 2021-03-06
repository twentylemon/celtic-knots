

#include "CelticGrid.h"

const static int DEFAULT_CELL_SIZE = 20;
const static int DEFAULT_STROKE = 2;
const static float DEFAULT_POINT_SIZE = 10.0f;
const static float DEFAULT_MARKER_SIZE = 3.0f;

CelticGrid::CelticGrid(int width, int height) :
    width_(width),
    height_(height),
    cell_size_(DEFAULT_CELL_SIZE),
    stroke_(DEFAULT_STROKE),
    point_size_(DEFAULT_POINT_SIZE),
    marker_size_(DEFAULT_MARKER_SIZE),
    display_grid_(true),
    display_markers_(true)
{
    init();
}


CelticGrid::CelticGrid(std::string inFile) :
    cell_size_(DEFAULT_CELL_SIZE),
    stroke_(DEFAULT_STROKE),
    point_size_(DEFAULT_POINT_SIZE),
    marker_size_(DEFAULT_MARKER_SIZE),
    display_grid_(true),
    display_markers_(true)
{
    std::ifstream in(inFile);
    int x, y;
    char dir;
    in >> width_ >> height_ >> x;   // last digit is useless here
    init();
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

void CelticGrid::init() {
    for (int x = 0; x < width(); x++) {
        cells_.emplace_back();
        for (int y = 0; y < height(); y++) {
            cells_[x].emplace_back(x, y);
        }
    }
    cell_renderer_ = CellRenderer(cell_size(), cell_size()/4.0f, cell_size()/6.0f);
}


int CelticGrid::width() const {
    return width_;
}

int CelticGrid::height() const {
    return height_;
}

int CelticGrid::cell_size() const {
    return cell_size_;
}

int CelticGrid::stroke() const {
    return stroke_;
}

float CelticGrid::point_size() const {
    return point_size_;
}

float CelticGrid::marker_size() const {
    return marker_size_;
}
    
bool CelticGrid::display_grid() const {
    return display_grid_;
}

bool CelticGrid::display_markers() const {
    return display_markers_;
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

CellRenderer& CelticGrid::cell_renderer() {
    return cell_renderer_;
}

const CellRenderer& CelticGrid::cell_renderer() const {
    return cell_renderer_;
}

void CelticGrid::set_cell_size(int cell_size) {
    cell_size_ = cell_size;
    cell_renderer_.set_cell_size((float)cell_size);
}

void CelticGrid::set_stroke(int stroke) {
    stroke_ = stroke;
}

void CelticGrid::set_point_size(float point_size) {
    point_size_ = point_size;
}
    
void CelticGrid::set_marker_size(float marker_size) {
    marker_size_ = marker_size;
}

void CelticGrid::set_display_grid(bool display_grid) {
    display_grid_ = display_grid;
}

void CelticGrid::set_display_markers(bool display_markers) {
    display_markers_ = display_markers;
}




bool CelticGrid::addMarker(int x, int y, BreakMarker::Direction direction) {
    if (!markers_.contains(BreakMarker(x, y, direction))) {
        markers_.emplace_back(x, y, direction); // add the marker
        handleCell(x, y, direction, true);
        return true;
    }
    return false;
}

bool CelticGrid::removeMarker(int x, int y, BreakMarker::Direction direction) {
    int rem = markers_.remove(BreakMarker(x, y, direction));
    handleCell(x, y, direction, false);
    return rem != 0;
}

void CelticGrid::handleCell(int x, int y, BreakMarker::Direction direction, bool marker) {
    if (direction == BreakMarker::East) {
        if (y > 0) {
            cells_[x][y-1].set_down(marker);
            cells_[x+1][y-1].set_down(marker);
        }
        if (y < height()) {
            cells_[x][y].set_up(marker);
            cells_[x+1][y].set_up(marker);
        }
    }
    else {
        if (x > 0) {
            cells_[x-1][y].set_right(marker);
            cells_[x-1][y+1].set_right(marker);
        }
        if (x < width()) {
            cells_[x][y].set_left(marker);
            cells_[x][y+1].set_left(marker);
        }
    }
}

void CelticGrid::renderSelect(int x, int y) const {
    glPointSize(point_size());
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_POINTS);
        glVertex2i(x * cell_size(), y * cell_size());
    glEnd();
}

void CelticGrid::renderGrid() const {
    glPointSize(point_size());
    glBegin(GL_POINTS);
    glColor3f(1.0f, 0.0f, 0.0f);
    for (int x = 0; x <= width(); x+=2) {
        for (int y = 0; y <= height(); y+=2) {
            glVertex2i(x * cell_size(), y * cell_size());
        }
    }
    glColor3f(0.0f, 1.0f, 0.0f);
    for (int x = 1; x < width(); x+=2) {
        for (int y = 1; y < height(); y+=2) {
            glVertex2i(x * cell_size(), y * cell_size());
        }
    }
    glEnd();
}

void CelticGrid::renderMarkers() const {
    glLineWidth(marker_size());
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    markers_.each([this](const BreakMarker& marker){
        glVertex2i(marker.x() * cell_size(), marker.y() * cell_size());
        if (marker.direction() == BreakMarker::East) {
            glVertex2i((marker.x()+2) * cell_size(), marker.y() * cell_size());
        }
        else {
            glVertex2i(marker.x() * cell_size(), (marker.y()+2) * cell_size());
        }
    });
    glEnd();
}

void CelticGrid::glRender(int windowWidth, int windowHeight) const {
    std::ofstream svg("../../knot.svg");
    svg << "<?xml version=\"1.0\"?>" << std::endl
        << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << std::endl
        << "<svg width=\"" << (cell_size()*width()) << "\" height=\"" << (cell_size()*height()) << "\" "
        << "version=\"1.1\" xmlns=\"http://www.w3.org/2000/svg\">" << std::endl;
    
    if (display_grid()) {
        renderGrid();
    }
    if (display_markers()) {
        renderMarkers();
    }
    cells_.each([this,&svg](const lemon::Vector<CelticCell>& col){
        col.each([this,&svg](const CelticCell& cell){
            cell_renderer_.render(cell, svg);
        });
    });
    svg << "</svg>" << std::endl;
    svg.close();
}
