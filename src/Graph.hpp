#pragma once

// GLEW
#include <GL/glew.h>

// GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// USUAL INCLUDES
#include <vector>

struct Graph {
    std::vector<glm::vec3> vertices;
    std::vector<glm::uvec2> links;
};

void drawGraph(Graph G) {
    glBegin(GL_LINES);
    glColor3f(1., 0., 0.);
    for (int i = 0; i < G.links.size(); i++) {
        glVertex3f(G.vertices[G.links[i][0]][0], G.vertices[G.links[i][0]][1], G.vertices[G.links[i][0]][2]);
        glVertex3f(G.vertices[G.links[i][1]][0], G.vertices[G.links[i][1]][1], G.vertices[G.links[i][1]][2]);
    }
    glEnd();
}

Graph gridGraph(float n) {
    Graph G;
    for (float i = 0; i < n; i++) {
        for (float j = 0; j < n; j++) {
            G.vertices.push_back(glm::vec3(i / (n - 1), 0., j / (n - 1)));
        }
    }
    for (float i = 0; i < n; i++) {
        for (float j = 0; j < n; j++) {
            if (i != 0) {
                G.links.push_back(glm::uvec2(j * n + i - 1, j * n + i));
            }
            if (j != 0) {
                G.links.push_back(glm::uvec2((j - 1) * n + i, j * n + i));
            }
        }
    }
    return G;
}