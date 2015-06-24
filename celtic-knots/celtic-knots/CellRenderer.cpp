

#include "CellRenderer.h"


CellRenderer::CellRenderer(int cellSize, float ribbonSize, float borderSize) :
    cell_size_((float)cellSize),
    half_size_(cellSize / 2.0f),
    ribbon_size_(ribbonSize),
    border_size_(borderSize)
{
}


void CellRenderer::render(const CelticCell& cell) const {
    /*
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex2f(cell.x()*cell_size() + half_size(), cell.y()*cell_size() + half_size());
    glEnd();
    */
    glPushMatrix();
    glTranslatef(cell.x() * cell_size() + half_size(), cell.y() * cell_size() + half_size(), 0.0f);
    glRotatef(270.0f, 0.0f, 0.0f, 1.0f);
    renderPipe();
    glPopMatrix();
}



void CellRenderer::renderPipe() const {
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(ribbon_size());
    glBegin(GL_LINES);  // draw the central ribbon
        glVertex2f(half_size(), 0);
        glVertex2f(half_size(), cell_size());
    glEnd();
}

