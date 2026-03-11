#pragma once

// GLEW
#include <GL/glew.h>

// GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// USUAL INCLUDES
#include <vector>
#include <set>
#include <algorithm>
#include <random>

class Graph {
    size_t m_n;
    std::vector<glm::vec3> m_vertices;
    std::vector<std::vector<size_t>> m_neighbours;

    // maze
    std::vector<glm::uvec2> m_links;

public:
    // CREATION

    static Graph gridGraph(float _n) {
        Graph G;

        G.m_n = _n * _n;
        for (float i = 0; i < _n; i++) {
            for (float j = 0; j < _n; j++) {
                G.m_vertices.push_back(glm::vec3(i / (_n - 1), 0., j / (_n - 1)));
                G.m_neighbours.push_back({});
            }
        }
        for (float i = 0; i < _n; i++) {
            for (float j = 0; j < _n; j++) {
                size_t v0 = j * _n + i;
                if (i != 0) {
                    size_t v1 = j * _n + i - 1;
                    G.m_neighbours[v0].push_back(v1);
                    G.m_neighbours[v1].push_back(v0);
                }
                if (j != 0) {
                    size_t v2 = (j - 1) * _n + i;
                    G.m_neighbours[v0].push_back(v2);
                    G.m_neighbours[v2].push_back(v0);
                }
            }
        }
        return G;
    }

    // DRAW

    void draw() const {
        // glColor3f(1.f, 0.831373f, 0.211765f);
        // glLineWidth(2.f);
        // glBegin(GL_LINES);

        // for (size_t v0 = 0; v0 < m_n; v0++) {
        //     for (size_t v1 : m_neighbours[v0]) {
        //         glVertex3f(m_vertices[v0].x, m_vertices[v0].y, m_vertices[v0].z);
        //         glVertex3f(m_vertices[v1].x, m_vertices[v1].y, m_vertices[v1].z);
        //     }
        // }
        // glEnd();

        glColor3f(1.f, 0.f, 0.f);
        glLineWidth(3.f);
        glBegin(GL_LINES);
        for (size_t i = 0; i < m_links.size(); i++) {
            glVertex3f(m_vertices[m_links[i][0]][0], m_vertices[m_links[i][0]][1], m_vertices[m_links[i][0]][2]);
            glVertex3f(m_vertices[m_links[i][1]][0], m_vertices[m_links[i][1]][1], m_vertices[m_links[i][1]][2]);
        }
        glEnd();
    }

    // MAZE GENERATION

    void depthFirstGeneration(size_t _current_cell, std::set<size_t> &_visited) {
        _visited.insert(_current_cell);

        std::vector<size_t> cell_neighbours(m_neighbours[_current_cell]);

        std::mt19937 rng(std::random_device{}());
        std::shuffle(cell_neighbours.begin(), cell_neighbours.end(), rng);

        for (size_t i = 0; i < cell_neighbours.size(); i++) {
            if (_visited.find(cell_neighbours[i]) != _visited.end())
                continue;
            m_links.push_back(glm::uvec2(_current_cell, cell_neighbours[i]));

            depthFirstGeneration(cell_neighbours[i], _visited);
        }
    }
    void depthFirstGeneration(size_t _current_cell) {
        std::set<size_t> visited;
        depthFirstGeneration(_current_cell, visited);
    }
};