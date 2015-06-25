

#include "CellRenderer.h"

const static float NUM_POINTS = 128.0f;

CellRenderer::CellRenderer(int cellSize, float ribbonSize, float borderSize) :
    cell_size_((float)cellSize),
    half_size_(cellSize / 2.0f),
    ribbon_size_(ribbonSize),
    border_size_(borderSize)
{
    set_ribbon_color(0.0f, 1.0f, 1.0f);
    set_border_color(1.0f, 0.0f, 1.0f);
}

void CellRenderer::startBorder() const {
    glColor3fv(border_color_.data());
    glPointSize(2.0f * border_size() + ribbon_size());
    glLineWidth(2.0f * border_size() + ribbon_size());
}

void CellRenderer::startRibbon() const {
    glColor3fv(ribbon_color_.data());
    glPointSize(ribbon_size());
    glLineWidth(ribbon_size());
}

void CellRenderer::setUpScissor(float objX, float objY) const {
    GLdouble model[16], proj[16];
    GLint view[4];
    GLdouble startX, startY, endX, endY, z;
    glGetDoublev(GL_MODELVIEW_MATRIX, model);
    glGetDoublev(GL_PROJECTION_MATRIX, proj);
    glGetIntegerv(GL_VIEWPORT, view);
    gluProject(objX-1, objY-1, 0.0, model, proj, view, &startX, &startY, &z);
    gluProject(objX+cell_size()+1, objY+cell_size()+1, 0.0, model, proj, view, &endX, &endY, &z);
    glScissor((int)startX, (int)startY, (int)(endX-startX), (int)(endY-startY));
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
        glVertex2f(0.0f, -cell_size());
    glEnd();

    startRibbon();
    glBegin(GL_LINES);
        glVertex2f(cell_size(), 0.0f);
        glVertex2f(0.0f, -cell_size());
    glEnd();
}


void CellRenderer::render(const CelticCell& cell) const {
    // this draws a pipe in each cell
    glPushMatrix();
    setUpScissor(cell.x() * cell_size(), cell.y() * cell_size());
    glTranslatef(cell.x() * cell_size() + half_size(), cell.y() * cell_size() + half_size(), 0.0f);
    glEnable(GL_SCISSOR_TEST);
    //glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    int r = std::rand() % 5;
    if (r == 0)
        renderPipe();
    else if (r == 1)
        renderCorner();
    else if (r == 2)
        renderOverBend();
    else if (r == 3)
        renderUnderBend();
    else
        renderDiagPipe();
    glDisable(GL_SCISSOR_TEST);
    glPopMatrix();
    
}

void CellRenderer::renderPipe() const {
    startBorder();
    glBegin(GL_LINES);
        glVertex2f(-half_size(), 0);
        glVertex2f(half_size(), 0);
    glEnd();

    startRibbon();
    glBegin(GL_LINES);
        glVertex2f(-half_size(), 0);
        glVertex2f(half_size(), 0);
    glEnd();
}

void CellRenderer::renderDiagPipe() const {
    startBorder();
    glBegin(GL_LINES);
        glVertex2f(-half_size(), half_size());
        glVertex2f(half_size(), -half_size());
    glEnd();

    startRibbon();
    glBegin(GL_LINES);
        glVertex2f(-half_size(), half_size());
        glVertex2f(half_size(), -half_size());
    glEnd();

    renderCover();  // cover up the corner
}

void CellRenderer::renderCorner() const {
    float points[3][3] = {
        { -half_size(), 0.0f, 0.0f },   // left middle
        { 0.0f, 0.0f, 0.0f },           // control point, the center
        { 0.0f, -half_size(), 0.0f }};  // bottom middle
    glMap1f(GL_MAP1_VERTEX_3, 0.0f, NUM_POINTS, 3, 3, &points[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    
    startBorder();
    renderMap();
    
    startRibbon();
    renderMap();
}

void CellRenderer::renderOverBend() const {
    float points[3][3] = {
        { -half_size(), 0.0f, 0.0f },           // left middle
        { 0.0f, 0.0f, 0.0f },                   // control point, the center
        { half_size(), -half_size(), 0.0f }};   // right-bottom corner
    glMap1f(GL_MAP1_VERTEX_3, 0.0f, NUM_POINTS, 3, 3, &points[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    
    startBorder();
    renderMap();
    
    startRibbon();
    renderMap();
}

void CellRenderer::renderUnderBend() const {
    renderOverBend();   // render the bend first, then we will just draw on top of it
    renderCover();
}
