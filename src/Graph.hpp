#pragma once

// GLEW
#include <GL/glew.h>

// GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// USUAL INCLUDES
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <random>

class Graph {
    size_t m_n;
    std::vector<glm::vec3> m_vertices;
    std::vector<std::unordered_set<size_t>> m_neighbours;

    // maze
    std::vector<glm::uvec2> m_links;

public:
    // CREATION

    static Graph gridGraph(float _n) {
        Graph G;

        G.m_n = _n * _n;
        for (float i = 0; i < _n; i++) {
            for (float j = 0; j < _n; j++) {
                G.m_vertices.push_back(glm::vec3(i / (_n - 1) - 0.5, 0., j / (_n - 1) - 0.5));
                G.m_neighbours.push_back({});
            }
        }
        for (float i = 0; i < _n; i++) {
            for (float j = 0; j < _n; j++) {
                size_t v0 = j * _n + i;
                if (i != 0) {
                    size_t v1 = j * _n + i - 1;
                    G.m_neighbours[v0].insert(v1);
                    G.m_neighbours[v1].insert(v0);
                }
                if (j != 0) {
                    size_t v2 = (j - 1) * _n + i;
                    G.m_neighbours[v0].insert(v2);
                    G.m_neighbours[v2].insert(v0);
                }
            }
        }
        return G;
    }

    static Graph cubeGraph(float _n) {
        Graph G;

        G.m_n = _n * _n * _n;
        for (float i = 0; i < _n; i++) {
            for (float j = 0; j < _n; j++) {
                for (float k = 0; k < _n; k++) {
                    G.m_vertices.push_back(glm::vec3(i / (_n - 1) - 0.5, k / (_n - 1) - 0.5, j / (_n - 1) - 0.5));
                    G.m_neighbours.push_back({});
                }
            }
        }
        for (float i = 0; i < _n; i++) {
            for (float j = 0; j < _n; j++) {
                for (float k = 0; k < _n; k++) {
                    size_t v0 = j * _n * _n + k * _n + i;
                    if (i != 0) {
                        size_t v1 = j * _n * _n + k * _n + i - 1;
                        G.m_neighbours[v0].insert(v1);
                        G.m_neighbours[v1].insert(v0);
                    }
                    if (j != 0) {
                        size_t v2 = (j - 1) * _n * _n + k * _n + i;
                        G.m_neighbours[v0].insert(v2);
                        G.m_neighbours[v2].insert(v0);
                    }
                    if (k != 0) {
                        size_t v3 = j * _n * _n + (k - 1) * _n + i;
                        G.m_neighbours[v0].insert(v3);
                        G.m_neighbours[v3].insert(v0);
                    }
                }
            }
        }
        return G;
    }

    static Graph circleGraph(float _nbCercles, float _n) {
        Graph G;

        G.m_n = _nbCercles * _n + 1;
        G.m_vertices.push_back(glm::vec3(0., 0., 0.));
        G.m_neighbours.push_back({});
        for (float i = 1; i < _nbCercles + 1; i++) {
            float r = i / _n;
            for (float j = 0; j < _n; j++) {
                float theta = (j / _n) * 2 * M_PI;
                G.m_vertices.push_back(glm::vec3(r * cos(theta), 0., r * sin(theta)));
                G.m_neighbours.push_back({});
            }
        }

        for (float j = 0; j < _n; j++) {
            size_t v0 = j + 1;
            G.m_neighbours[v0].insert(0);
            G.m_neighbours[0].insert(v0);

            if (j != 0) {
                size_t v2 = j;
                G.m_neighbours[v0].insert(v2);
                G.m_neighbours[v2].insert(v0);
            }
            if (j == _n - 1) {
                size_t v2 = 1;
                G.m_neighbours[v0].insert(v2);
                G.m_neighbours[v2].insert(v0);
            }
        }
        G.m_neighbours[_n + 1].insert(1);
        G.m_neighbours[1].insert(_n + 1);
        for (float i = 1; i < _nbCercles; i++) {
            for (float j = 0; j < _n; j++) {
                size_t v0 = i * _n + j + 1;
                if (j != 0) {
                    size_t v1 = i * _n + j;
                    G.m_neighbours[v0].insert(v1);
                    G.m_neighbours[v1].insert(v0);
                }
                size_t v2 = (i - 1) * _n + j + 1;
                G.m_neighbours[v0].insert(v2);
                G.m_neighbours[v2].insert(v0);
                if (j == _n - 1) {
                    size_t v1 = i * _n + 1;
                    G.m_neighbours[v0].insert(v1);
                    G.m_neighbours[v1].insert(v0);
                }
            }
        }
        return G;
    }

    // DRAW

    void draw() const {
        glColor3f(1.f, 0.f, 0.f);
        glLineWidth(5.f);
        glBegin(GL_LINES);
        for (size_t i = 0; i < m_links.size(); i++) {
            glVertex3f(m_vertices[m_links[i][0]][0], m_vertices[m_links[i][0]][1], m_vertices[m_links[i][0]][2]);
            glVertex3f(m_vertices[m_links[i][1]][0], m_vertices[m_links[i][1]][1], m_vertices[m_links[i][1]][2]);
        }
        glEnd();

        // glColor3f(1.f, 0.831373f, 0.211765f);
        // glLineWidth(.5f);
        // glBegin(GL_LINES);
        // for (size_t v0 = 0; v0 < m_n; v0++) {
        //     for (size_t v1 : m_neighbours[v0]) {
        //         glVertex3f(m_vertices[v0].x, m_vertices[v0].y, m_vertices[v0].z);
        //         glVertex3f(m_vertices[v1].x, m_vertices[v1].y, m_vertices[v1].z);
        //     }
        // }
        glEnd();
    }

    // MAZE GENERATION

    void depthFirstGenerationRecursive(size_t _current_cell, std::unordered_set<size_t> &_visited) {
        _visited.insert(_current_cell);

        std::vector<size_t> cell_neighbours(m_neighbours[_current_cell].begin(), m_neighbours[_current_cell].end());

        std::mt19937 rng(std::random_device{}());
        std::shuffle(cell_neighbours.begin(), cell_neighbours.end(), rng);

        for (size_t i = 0; i < cell_neighbours.size(); i++) {
            if (_visited.find(cell_neighbours[i]) != _visited.end())
                continue;
            m_links.push_back(glm::uvec2(_current_cell, cell_neighbours[i]));

            depthFirstGenerationRecursive(cell_neighbours[i], _visited);
        }
    }
    void depthFirstGeneration(size_t _current_cell) {
        std::unordered_set<size_t> visited;
        depthFirstGenerationRecursive(_current_cell, visited);
    }

    void depthFirstGenerationIterative(size_t _initial_cell) {
        std::mt19937 rng(std::random_device{}());
        std::unordered_set<size_t> visited{_initial_cell};
        std::vector<size_t> stack{_initial_cell};
        stack.reserve(m_n);

        while (stack.size() != 0) {
            size_t current_cell = stack[stack.size() - 1];
            stack.pop_back();

            std::vector<size_t> cell_neighbours(m_neighbours[current_cell].begin(), m_neighbours[current_cell].end());
            std::shuffle(cell_neighbours.begin(), cell_neighbours.end(), rng);

            for (size_t i = 0; i < cell_neighbours.size(); i++) {
                if (visited.find(cell_neighbours[i]) != visited.end())
                    continue;

                stack.push_back(current_cell);
                m_links.push_back(glm::uvec2(current_cell, cell_neighbours[i]));

                visited.insert(cell_neighbours[i]);
                stack.push_back(cell_neighbours[i]);
                break;
            }
        }
    }

    void kruskalGeneration() {
        std::vector<std::unordered_set<size_t>> sets(m_n);
        for (size_t i = 0; i < m_n; i++) {
            sets[i] = {i};
        }

        std::vector<glm::uvec2> walls;
        for (size_t v0 = 0; v0 < m_n; v0++) {
            for (size_t v1 : m_neighbours[v0]) {
                if (v0 < v1) {
                    walls.push_back(glm::uvec2(v0, v1));
                }
            }
        }

        std::mt19937 rng(std::random_device{}());
        std::shuffle(walls.begin(), walls.end(), rng);

        for (const glm::uvec2 &wall : walls) {
            std::vector<size_t> diff;
            std::set_symmetric_difference(sets[wall.x].begin(), sets[wall.x].end(), sets[wall.y].begin(), sets[wall.y].end(), std::back_inserter(diff));

            if (diff.size() > 0) {
                sets[wall.x].insert(diff.begin(), diff.end());

                for (size_t v : sets[wall.x]) {
                    if (v != wall.x) {
                        sets[v] = sets[wall.x];
                    }
                }
                m_links.push_back(wall);
            }
        }
    }
};