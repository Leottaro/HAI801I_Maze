#include "Graph.hpp"

Graph Graph::gridGraph(float _n) {
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

Graph Graph::cubeGraph(float _n) {
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

Graph Graph::circleGraph(float _nbCercles, float _n) {
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