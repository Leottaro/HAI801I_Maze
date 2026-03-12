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
#include <functional>

class Graph {
    size_t m_n;
    std::vector<glm::vec3> m_vertices;
    std::vector<std::unordered_set<size_t>> m_neighbours;

public:
    // CREATION

    static Graph gridGraph(float _n);
    static Graph cubeGraph(float _n);
    static Graph circleGraph(float _nbCercles, float _n);

    // GENERAL
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
private:
    void _depthFirstRecursiveGeneration(size_t _current_cell, std::mt19937 &_rng, std::unordered_set<size_t> &_visited, Graph &res) const;
    Graph _cloneVertices() const;

public:
    Graph depthFirstRecursiveGeneration(size_t _initial_cell = 0) const;
    Graph depthFirstIterativeGeneration(size_t _initial_cell = 0) const;
    Graph kruskalGeneration() const;
    Graph primGeneration(size_t _initial_cell = 0) const;
};