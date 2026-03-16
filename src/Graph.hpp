#pragma once

// GLEW
#include <GL/glew.h>

// GLM
#include <glm/ext.hpp>
#include <glm/glm.hpp>

// USUAL INCLUDES
#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

class Graph {
    size_t m_n;
    std::vector<glm::vec3> m_vertices;
    std::vector<std::unordered_set<size_t>> m_neighbours;

public:
    typedef std::function<void(Graph &_g)> graph_gen_callback;

    // CREATION

    static Graph gridGraph(float _n);
    static Graph cubeGraph(float _n);
    static Graph circleGraph(float _n, float _nbCercles);
    static Graph sphereGraph(float _n, float _nbCercles, float nbEtage);
    static Graph sphereContourGraph(float _n, float nbEtage);

    // GENERAL
    inline size_t getN() const { return m_n; }
    inline std::unordered_set<size_t> getNeighbours(size_t i) const { return m_neighbours[i]; }

    void draw(const glm::vec3 &_color = glm::vec3(1.f, 1.f, 1.f), float _line_width = 1.f, std::unordered_set<size_t> _user_nodes = {0}) const {
        glColor3fv(glm::value_ptr(_color));
        glLineWidth(_line_width);
        glBegin(GL_LINES);
        for (size_t v0 = 0; v0 < m_n; v0++) {
            for (size_t v1 : m_neighbours[v0]) {
                if (_user_nodes.find(v0) != _user_nodes.end() && _user_nodes.find(v1) != _user_nodes.end())
                    glColor3f(1.f, 1.f, 1.f);
                else
                    glColor3fv(glm::value_ptr(_color));
                glVertex3fv(glm::value_ptr(m_vertices[v0]));
                glVertex3fv(glm::value_ptr(m_vertices[v1]));
            }
        }
        glEnd();
    }

    // MAZE GENERATION
private:
    void _depthFirstRecursiveGeneration(size_t _current_cell, std::mt19937 &_rng, std::unordered_set<size_t> &_visited, Graph &res, graph_gen_callback _callback) const;
    Graph _cloneVertices() const;

public:
    Graph depthFirstRecursiveGeneration(graph_gen_callback _callback) const;
    Graph depthFirstIterativeGeneration(graph_gen_callback _callback) const;
    Graph kruskalGeneration(graph_gen_callback _callback) const;
    Graph primGeneration(graph_gen_callback _callback) const;

    // PATHFINDING
    Graph subPath(const std::vector<size_t> _path);

    std::vector<size_t> dijkstra(size_t _sdeb, size_t _sfin) const;
    std::vector<size_t> dijkstra() const { return dijkstra(0, m_n - 1); }

    std::vector<size_t> a_star(size_t _sdeb, size_t _sfin) const;
    std::vector<size_t> a_star() const { return a_star(0, m_n - 1); }
};
