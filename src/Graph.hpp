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

    void draw(const glm::vec3 &_color = glm::vec3(1.f, 1.f, 1.f), float _line_width = 1.f) const {
        glColor3fv(glm::value_ptr(_color));
        glLineWidth(_line_width);
        glBegin(GL_LINES);
        for (size_t v0 = 0; v0 < m_n; v0++) {
            for (size_t v1 : m_neighbours[v0]) {
                glVertex3fv(glm::value_ptr(m_vertices[v0]));
                glVertex3fv(glm::value_ptr(m_vertices[v1]));
            }
        }
        glEnd();
    }

    // MAZE GENERATION

    void depthFirstGenerationRecursive(size_t _current_cell, std::mt19937 &_rng, std::unordered_set<size_t> &_visited, Graph &res) {
        _visited.insert(_current_cell);
        std::vector<size_t> cell_neighbours(m_neighbours[_current_cell].begin(), m_neighbours[_current_cell].end());
        std::shuffle(cell_neighbours.begin(), cell_neighbours.end(), _rng);

        for (size_t i = 0; i < cell_neighbours.size(); i++) {
            if (_visited.find(cell_neighbours[i]) != _visited.end())
                continue;

            res.m_neighbours[_current_cell].insert(cell_neighbours[i]);
            res.m_neighbours[cell_neighbours[i]].insert(_current_cell);

            depthFirstGenerationRecursive(cell_neighbours[i], _rng, _visited, res);
        }
    }
    Graph depthFirstGeneration(size_t _current_cell) {
        std::unordered_set<size_t> visited;
        std::mt19937 rng(std::random_device{}());
        Graph res;
        res.m_n = m_n;
        res.m_vertices = std::vector<glm::vec3>(m_vertices);
        res.m_neighbours.resize(m_n);
        for (size_t i = 0; i < m_n; i++) {
            res.m_neighbours[i] = {};
        }

        depthFirstGenerationRecursive(_current_cell, rng, visited, res);

        return res;
    }

    Graph depthFirstGenerationIterative(size_t _initial_cell) {
        std::mt19937 rng(std::random_device{}());
        std::unordered_set<size_t> already_stacked{_initial_cell};
        std::vector<size_t> stack{_initial_cell};
        stack.reserve(m_n);

        Graph res;
        res.m_n = m_n;
        res.m_vertices = std::vector<glm::vec3>(m_vertices);
        res.m_neighbours.resize(m_n);
        for (size_t i = 0; i < m_n; i++) {
            res.m_neighbours[i] = {};
        }

        while (stack.size() != 0) {
            size_t current_cell = stack[stack.size() - 1];
            stack.pop_back();

            std::vector<size_t> cell_neighbours(m_neighbours[current_cell].begin(), m_neighbours[current_cell].end());
            std::shuffle(cell_neighbours.begin(), cell_neighbours.end(), rng);

            for (size_t i = 0; i < cell_neighbours.size(); i++) {
                if (already_stacked.insert(cell_neighbours[i]).second) {
                    res.m_neighbours[current_cell].insert(cell_neighbours[i]);
                    res.m_neighbours[cell_neighbours[i]].insert(current_cell);

                    stack.push_back(current_cell);
                    stack.push_back(cell_neighbours[i]);
                    break;
                }
            }
        }

        return res;
    }

    Graph kruskalGeneration() {
        Graph res;
        res.m_n = m_n;
        res.m_vertices = std::vector<glm::vec3>(m_vertices);
        res.m_neighbours.resize(m_n);
        for (size_t i = 0; i < m_n; i++) {
            res.m_neighbours[i] = {};
        }

        std::vector<size_t> ids(m_n);
        std::vector<glm::uvec2> walls;
        walls.reserve(m_n);
        for (size_t v0 = 0; v0 < m_n; v0++) {
            ids[v0] = v0;
            for (size_t v1 : m_neighbours[v0]) {
                if (v0 < v1) {
                    walls.push_back(glm::uvec2(v0, v1));
                }
            }
        }

        std::mt19937 rng(std::random_device{}());
        std::shuffle(walls.begin(), walls.end(), rng);

        std::unordered_set<size_t> already_stacked;
        std::vector<size_t> stack;
        already_stacked.reserve(m_n);
        stack.reserve(m_n);
        for (const glm::uvec2 &wall : walls) {
            if (ids[wall.x] == ids[wall.y]) {
                continue;
            }
            res.m_neighbours[wall.x].insert(wall.y);
            res.m_neighbours[wall.y].insert(wall.x);

            size_t new_id = std::min(ids[wall.x], ids[wall.y]);
            size_t old_id = std::max(ids[wall.x], ids[wall.y]);
            already_stacked.clear();
            already_stacked.insert(wall.x);
            already_stacked.insert(wall.y);
            stack.clear();
            stack.push_back(wall.x);
            stack.push_back(wall.y);
            while (stack.size() > 0) {
                size_t v0 = stack[stack.size() - 1];
                stack.pop_back();

                if (ids[v0] != old_id) {
                    continue;
                }

                ids[v0] = new_id;
                for (size_t v1 : m_neighbours[v0]) {
                    if (already_stacked.insert(v1).second) {
                        stack.push_back(v1);
                    }
                }
            }
        }

        return res;
    }
};
