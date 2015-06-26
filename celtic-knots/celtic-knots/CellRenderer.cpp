

#include "CellRenderer.h"

const static float NUM_POINTS = 128.0f;
const static std::string PLACEHOLDER = "!!!!!";

CellRenderer::CellRenderer() :
    cell_size_(0.0f),
    half_size_(0.0f),
    ribbon_size_(0.0f),
    border_size_(0.0f),
    svg()
{
    init();
}

CellRenderer::CellRenderer(int cellSize, float ribbonSize, float borderSize) :
    cell_size_((float)cellSize),
    half_size_(cellSize / 2.0f),
    ribbon_size_(ribbonSize),
    border_size_(borderSize),
    svg()
{
    set_ribbon_color(0.0f, 1.0f, 1.0f);
    set_border_color(1.0f, 0.0f, 1.0f);
    init();
}

std::string CellRenderer::toRGB(const lemon::Array<float,3>& color) const {
    return "rgb(" + std::to_string((int)(255.0f*color[0])) + "," + std::to_string((int)(255.0f*color[1])) + "," + std::to_string((int)(255.0f*color[2])) + ")";
}

float CellRenderer::cell_size() const {
    return cell_size_;
}

float CellRenderer::half_size() const {
    return half_size_;
}

float CellRenderer::ribbon_size() const {
    return ribbon_size_;
}

float CellRenderer::border_size() const {
    return border_size_;
}

void CellRenderer::set_cell_size(float cell_size) {
    cell_size_ = cell_size;
    half_size_ = cell_size / 2.0f;
}
    
void CellRenderer::set_ribbon_size(float ribbon_size) {
    ribbon_size_ = ribbon_size;
}

void CellRenderer::set_border_size(float border_size) {
    border_size_ = border_size;
}

void CellRenderer::set_ribbon_color(float red, float green, float blue) {
    ribbon_color_[0] = red;
    ribbon_color_[1] = green;
    ribbon_color_[2] = blue;
}

void CellRenderer::set_border_color(float red, float green, float blue) {
    border_color_[0] = red;
    border_color_[1] = green;
    border_color_[2] = blue;
}

CellRenderer& CellRenderer::operator=(const CellRenderer& rhs) {
    cell_size_ = rhs.cell_size();
    half_size_ = rhs.half_size();
    ribbon_size_ = rhs.ribbon_size();
    border_size_ = rhs.border_size();
    ribbon_color_ = rhs.ribbon_color_;
    border_color_ = rhs.border_color_;
    return *this;
}


void CellRenderer::render(const CelticCell& cell) const {
    render(cell, std::ofstream());  // stream will just error, nothing to worry about
}

void CellRenderer::startBorder() const {
    glColor3fv(border_color_.data());
    glLineWidth(2.0f * border_size() + ribbon_size());
    svg << "<path stroke=\"" << toRGB(border_color_) << "\" stroke-width=\"" << (2.0f * border_size() + ribbon_size()) << "\" ";
}

void CellRenderer::startRibbon() const {
    glColor3fv(ribbon_color_.data());
    glLineWidth(ribbon_size());
    svg << "<path stroke=\"" << toRGB(ribbon_color_) << "\" stroke-width=\"" << ribbon_size() << "\" ";
}

void CellRenderer::setUpScissor(float objX, float objY) const {
    GLdouble model[16], proj[16];
    GLint view[4];
    GLdouble startX, startY, endX, endY, z;
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    glGetIntegerv(GL_VIEWPORT, view);
    gluProject(objX-1, objY+cell_size()+1, 0.0, model, proj, view, &startX, &startY, &z);
    gluProject(objX+cell_size()+1, objY-1, 0.0, model, proj, view, &endX, &endY, &z);
    glScissor((int)startX, (int)startY, (int)(endX-startX), (int)(endY-startY));

    std::string clipPath = std::to_string((int)objX) + "-" + std::to_string((int)objY);
    svg << "<defs><clipPath id=\"" << clipPath << "\"><rect x=\"" << -half_size() << "\" y=\"" << -half_size() << "\""
        << " width=\"" << cell_size() << "\" height=\"" << cell_size() << "\"/></clipPath></defs>" << std::endl
        << "<g clip-path=\"url(#" << clipPath << ")\" fill=\"transparent\" transform=\"" << PLACEHOLDER << "\">" << std::endl;
}

void CellRenderer::renderMap() const {
    glBegin(GL_LINE_STRIP);
    for (float i = 0; i <= NUM_POINTS; i++) {
        glEvalCoord1f(i);
    }
    glEnd();
}

void CellRenderer::renderCover() const {
    // draw a line from neighbouring centers; scissor will clip the rest
    startBorder();
    glBegin(GL_LINES);
        glVertex2f(cell_size(), 0.0f);
        glVertex2f(0.0f, cell_size());
    glEnd();
    svg << "d=\"M" << cell_size() << " 0 L0 " << cell_size() << "\"/>" << std::endl;

    startRibbon();
    glBegin(GL_LINES);
        glVertex2f(cell_size(), 0.0f);
        glVertex2f(0.0f, cell_size());
    glEnd();
    svg << "d=\"M" << cell_size() << " 0 L0 " << cell_size() << "\"/>" << std::endl;
}


void CellRenderer::render(const CelticCell& cell, std::ostream& out) const {
    if (rendFunc.contains(idxof(cell))) { // if there is a rendering function for this cell
        svg.str("");    // clear the buffer
        glPushMatrix();
        setUpScissor(cell.x() * cell_size(), cell.y() * cell_size());
        glEnable(GL_SCISSOR_TEST);  // turn on clipping around this cell
        glTranslatef(cell.x() * cell_size() + half_size(), cell.y() * cell_size() + half_size(), 0.0f);
        transform = "translate(" + std::to_string(cell.x() * cell_size() + half_size()) + " " + std::to_string(cell.y() * cell_size() + half_size()) + ")";
        rendFunc.at(idxof(cell))(); // render this cell
        glDisable(GL_SCISSOR_TEST);
        glPopMatrix();
        svg << "</g>" << std::endl; // end the group started in `setUpScissor`
        int pos = svg.str().find(PLACEHOLDER);  // place the group transformation string in the buffer
        out << svg.str().erase(pos, PLACEHOLDER.length()).insert(pos, transform);
    }
}

void CellRenderer::renderPipe() const {
    startBorder();
    glBegin(GL_LINES);
        glVertex2f(-half_size(), 0);
        glVertex2f(half_size(), 0);
    glEnd();
    svg << "d=\"M" << -half_size() << " 0 L" << half_size() << " 0\"/>" << std::endl;

    startRibbon();
    glBegin(GL_LINES);
        glVertex2f(-half_size(), 0);
        glVertex2f(half_size(), 0);
    glEnd();
    svg << "d=\"M" << -half_size() << " 0 L" << half_size() << " 0\"/>" << std::endl;
}

void CellRenderer::renderDiagPipe() const {
    startBorder();
    glBegin(GL_LINES);
        glVertex2f(-half_size(), -half_size());
        glVertex2f(half_size(), half_size());
    glEnd();
    svg << "d=\"M" << -half_size() << " " << -half_size() << " L" << half_size() << " " << half_size() << "\"/>" << std::endl;

    startRibbon();
    glBegin(GL_LINES);
        glVertex2f(-half_size(),-half_size());
        glVertex2f(half_size(), half_size());
    glEnd();
    svg << "d=\"M" << -half_size() << " " << -half_size() << " L" << half_size() << " " << half_size() << "\"/>" << std::endl;

    renderCover();  // cover up the corner
}

void CellRenderer::renderCorner() const {
    float points[3][3] = {
        { -half_size(), 0.0f, 0.0f },   // left middle
        { 0.0f, 0.0f, 0.0f },           // control point, the center
        { 0.0f, half_size(), 0.0f }};   // bottom middle
    glMap1f(GL_MAP1_VERTEX_3, 0.0f, NUM_POINTS, 3, 3, &points[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    
    startBorder();
    renderMap();
    svg << "d=\"M" << -half_size() << " 0 Q0 0, 0 " << half_size() << "\"/>" << std::endl;
    
    startRibbon();
    renderMap();
    svg << "d=\"M" << -half_size() << " 0 Q0 0, 0 " << half_size() << "\"/>" << std::endl;
}

void CellRenderer::renderOverBend() const {
    float points[3][3] = {
        { -half_size(), 0.0f, 0.0f },           // left middle
        { 0.0f, 0.0f, 0.0f },                   // control point, the center
        { half_size(), half_size(), 0.0f }};    // bottom-right corner
    glMap1f(GL_MAP1_VERTEX_3, 0.0f, NUM_POINTS, 3, 3, &points[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    
    startBorder();
    renderMap();
    svg << "d=\"M" << -half_size() << " 0 Q0 0, " << half_size() << " " << half_size() << "\"/>" << std::endl;
    
    startRibbon();
    renderMap();
    svg << "d=\"M" << -half_size() << " 0 Q0 0, " << half_size() << " " << half_size() << "\"/>" << std::endl;
}

void CellRenderer::renderUnderBend() const {
    renderOverBend();   // render the bend first, then we will just draw on top of it
    renderCover();
}

int CellRenderer::idxof(int x, int y, bool up, bool down, bool left, bool right) const {
    int result = 0;
    if (up) { result += 1; }
    if (down) { result += 2; }
    if (left) { result += 4; }
    if (right) { result += 8; }
    if ((x + y) % 2 == 0){ result += 16; }
    if (y % 2 == 0) { result += 32; }
    return result;
}

int CellRenderer::idxof(const CelticCell& cell) const {
    return idxof(cell.x(), cell.y(), cell.up(), cell.down(), cell.left(), cell.right());
}

void CellRenderer::init() {
    /** no break markers **/
    rendFunc[idxof(0, 0, false, false, false, false)] = [this](){
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        transform += " rotate(90)";
        renderDiagPipe();
    };
    rendFunc[idxof(1, 1, false, false, false, false)] = [this](){
        glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
        transform += " rotate(-90)";
        renderDiagPipe();
    };
    rendFunc[idxof(0, 1, false, false, false, false)] = [this](){
        renderDiagPipe();
    };
    rendFunc[idxof(1, 0, false, false, false, false)] = [this](){
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        transform += " rotate(180)";
        renderDiagPipe();
    };
    

    /** up only **/
    rendFunc[idxof(0, 0, true, false, false, false)] = [this](){
        glScalef(-1.0f, 1.0f, 1.0f);
        transform += " scale(-1, 1)";
        renderUnderBend();
    };
    rendFunc[idxof(1, 1, true, false, false, false)] = [this](){
        glScalef(-1.0f, 1.0f, 1.0f);
        transform += " scale(-1, 1)";
        renderOverBend();
    };
    rendFunc[idxof(0, 1, true, false, false, false)] = [this](){
        renderUnderBend();
    };
    rendFunc[idxof(1, 0, true, false, false, false)] = [this](){
        renderOverBend();
    };

    /** down only **/
    rendFunc[idxof(0, 0, false, true, false, false)] = [this](){
        glScalef(1.0f, -1.0f, 1.0f);
        transform += " scale(1, -1)";
        renderOverBend();
    };
    rendFunc[idxof(1, 1, false, true, false, false)] = [this](){
        glScalef(1.0f, -1.0f, 1.0f);
        transform += " scale(1, -1)";
        renderUnderBend();
    };
    rendFunc[idxof(0, 1, false, true, false, false)] = [this](){
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        transform += " rotate(180)";
        renderOverBend();
    };
    rendFunc[idxof(1, 0, false, true, false, false)] = [this](){
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        transform += " rotate(180)";
        renderUnderBend();
    };

    /** left only **/
    rendFunc[idxof(0, 0, false, false, true, false)] = [this](){
        glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
        transform += " rotate(-90)";
        renderOverBend();
    };
    rendFunc[idxof(1, 1, false, false, true, false)] = [this](){
        glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
        transform += " rotate(-90)";
        renderUnderBend();
    };
    rendFunc[idxof(0, 1, false, false, true, false)] = [this](){
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        glScalef(1.0f, -1.0f, 1.0f);
        transform += " rotate(90) scale(1, -1)";
        renderUnderBend();
    };
    rendFunc[idxof(1, 0, false, false, true, false)] = [this](){
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        glScalef(1.0f, -1.0f, 1.0f);
        transform += " rotate(90) scale(1, -1)";
        renderOverBend();
    };

    /** right only **/
    rendFunc[idxof(0, 0, false, false, false, true)] = [this](){
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        transform += " rotate(90)";
        renderUnderBend();
    };
    rendFunc[idxof(1, 1, false, false, false, true)] = [this](){
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        transform += " rotate(90)";
        renderOverBend();
    };
    rendFunc[idxof(0, 1, false, false, false, true)] = [this](){
        glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
        glScalef(1.0f, -1.0f, 1.0f);
        transform += " rotate(-90) scale(1, -1)";
        renderOverBend();
    };
    rendFunc[idxof(1, 0, false, false, false, true)] = [this](){
        glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
        glScalef(1.0f, -1.0f, 1.0f);
        transform += " rotate(-90) scale(1, -1)";
        renderUnderBend();
    };


    /** left-top **/
    rendFunc[idxof(0, 0, true, false, true, false)] =
    rendFunc[idxof(1, 1, true, false, true, false)] = [this](){
        glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
        transform += " rotate(-90)";
        renderCorner();
    };

    /** bottom-right **/
    rendFunc[idxof(0, 0, false, true, false, true)] =
    rendFunc[idxof(1, 1, false, true, false, true)] = [this](){
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        transform += " rotate(90)";
        renderCorner();
    };

    /** top-right **/
    rendFunc[idxof(0, 1, true, false, false, true)] =
    rendFunc[idxof(1, 0, true, false, false, true)] = [this](){
        renderCorner();
    };

    /** bottom-left **/
    rendFunc[idxof(0, 1, false, true, true, false)] =
    rendFunc[idxof(1, 0, false, true, true, false)] = [this](){
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        transform += " rotate(180)";
        renderCorner();
    };


    /** top-bottom */
    rendFunc[idxof(0, 0, true, true, false, false)] =
    rendFunc[idxof(1, 1, true, true, false, false)] =
    rendFunc[idxof(0, 1, true, true, false, false)] =
    rendFunc[idxof(1, 0, true, true, false, false)] = [this](){
        renderPipe();
    };

    /** left-right */
    rendFunc[idxof(0, 0, false, false, true, true)] =
    rendFunc[idxof(1, 1, false, false, true, true)] =
    rendFunc[idxof(0, 1, false, false, true, true)] =
    rendFunc[idxof(1, 0, false, false, true, true)] = [this](){
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        transform += " rotate(90)";
        renderPipe();
    };
}
