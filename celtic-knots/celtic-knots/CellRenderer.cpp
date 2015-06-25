

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

void CellRenderer::render(const CelticCell& cell) const {
    render(cell, std::ofstream());  // stream will just error, nothing to worry about
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
    gluProject(objX-1, objY+cell_size()+1, 0.0, model, proj, view, &startX, &startY, &z);
    gluProject(objX+cell_size()+1, objY-1, 0.0, model, proj, view, &endX, &endY, &z);
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
        glVertex2f(0.0f, cell_size());
    glEnd();

    startRibbon();
    glBegin(GL_LINES);
        glVertex2f(cell_size(), 0.0f);
        glVertex2f(0.0f, cell_size());
    glEnd();
}


void CellRenderer::render(const CelticCell& cell, std::ostream& svg) const {
    if (cell.ord() < 3) {
        glPushMatrix();
        setUpScissor(cell.x() * cell_size(), cell.y() * cell_size());
        glEnable(GL_SCISSOR_TEST);
        glTranslatef(cell.x() * cell_size() + half_size(), cell.y() * cell_size() + half_size(), 0.0f);
        bool odd = (cell.x() + cell.y()) % 2 == 1;
        if (odd) {
            renderOddCell(cell);
        }
        else {
            renderEvenCell(cell);
        }
        glDisable(GL_SCISSOR_TEST);
        glPopMatrix();
        svg << "hello" << std::endl;
    }
    /* TODO
    svg: clip-path can be used as glScissor, transform="..." can be used for gl* transformations
    */
}

void CellRenderer::renderOddCell(const CelticCell& cell) const {
    bool evenCol = cell.y() % 2 == 0;
    if (cell.ord() == 0) {
        if (evenCol) {
            glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        }
        renderDiagPipe();
    }
    else if (cell.ord() == 1) { // always render a bend
        if (cell.left()) {
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            glScalef(1.0f, -1.0f, 1.0f);
            evenCol ? renderOverBend() : renderUnderBend();
        }
        else if (cell.up()) {
            evenCol ? renderOverBend() : renderUnderBend();
        }
        else if (cell.right()) {
            glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
            glScalef(1.0f, -1.0f, 1.0f);
            evenCol ? renderUnderBend() : renderOverBend();
        }
        else if (cell.down()) {
            glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
            evenCol ? renderUnderBend() : renderOverBend();
        }
    }
    else if (cell.ord() == 2) {
        if (cell.up() && cell.right()) {
            renderCorner();
        }
        else if (cell.left() && cell.down()) {
            glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
            renderCorner();
        }
        else if (cell.left() && cell.right()) {
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            renderPipe();
        }
        else if (cell.up() && cell.down()) {
            renderPipe();
        }
    }
}

void CellRenderer::renderEvenCell(const CelticCell& cell) const {
    bool evenCol = cell.y() % 2 == 0;
    if (cell.ord() == 0) {
        if (!evenCol) {
            glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        }
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        renderDiagPipe();
    }
    else if (cell.ord() == 1) { // always render a bend
        if (cell.left()) {
            glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
            evenCol ? renderOverBend() : renderUnderBend();
        }
        else if (cell.up()) {
            glScalef(-1.0f, 1.0f, 1.0f);
            evenCol ? renderUnderBend() : renderOverBend();
        }
        else if (cell.right()) {
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            evenCol ? renderUnderBend() : renderOverBend();
        }
        else if (cell.down()) {
            glScalef(1.0f, -1.0f, 1.0f);
            evenCol ? renderOverBend() : renderUnderBend();
        }
    }
    else if (cell.ord() == 2) {
        if (cell.left() && cell.up()) {
            glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
            renderCorner();
        }
        else if (cell.right() && cell.down()) {
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            renderCorner();
        }
        else if (cell.left() && cell.right()) {
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            renderPipe();
        }
        else if (cell.up() && cell.down()) {
            renderPipe();
        }
    }
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
        glVertex2f(-half_size(), -half_size());
        glVertex2f(half_size(), half_size());
    glEnd();

    startRibbon();
    glBegin(GL_LINES);
        glVertex2f(-half_size(),-half_size());
        glVertex2f(half_size(), half_size());
    glEnd();

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
    
    startRibbon();
    renderMap();
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
    
    startRibbon();
    renderMap();
}

void CellRenderer::renderUnderBend() const {
    renderOverBend();   // render the bend first, then we will just draw on top of it
    renderCover();
}
