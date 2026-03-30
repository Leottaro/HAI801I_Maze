#include "Graph.hpp"

Graph Graph::subPath(const std::vector<size_t> _path) {
    Graph g;

    g.m_n = _path.size();
    g.m_vertices.resize(g.m_n);
    g.m_neighbours = std::vector<std::unordered_set<size_t>>(g.m_n);
    for (size_t i = 0; i < g.m_n; i++) {
        g.m_vertices[i] = m_vertices[_path[i]];
        if (i > 0) {
            g.m_neighbours[i - 1].insert(i);
            g.m_neighbours[i].insert(i - 1);
        }
    }

    return g;
}

std::vector<size_t> Graph::dijkstra(size_t _sdeb, size_t _sfin) const {
    std::vector<size_t> distances(m_n, UINT64_MAX);
    std::vector<size_t> previous(m_n, UINT64_MAX);
    std::vector<bool> visited(m_n, false);

    previous[_sdeb] = UINT64_MAX;
    distances[_sdeb] = 0;

    for (size_t i = 0; i < m_n; i++) {
        size_t u = UINT64_MAX;
        size_t minDist = UINT64_MAX;

        for (size_t j = 0; j < m_n; j++) {
            if (!visited[j] && distances[j] < minDist) {
                u = j;
                minDist = distances[j];
            }
        }

        if (u == UINT64_MAX)
            break;

        visited[u] = true;

        for (size_t v : m_neighbours[u]) {
            if (v <= m_n && !visited[v]) {
                size_t alt = distances[u] + 1;
                if (alt < distances[v]) {
                    distances[v] = alt;
                    previous[v] = u;
                }
            }
        }
    }

    std::vector<size_t> path;
    size_t curr = _sfin;
    while (curr != UINT64_MAX) {
        path.push_back(curr);
        curr = previous[curr];
    }
    std::reverse(path.begin(), path.end());

    return path;
}

std::vector<size_t> Graph::a_star(size_t _sdeb, size_t _sfin) const {
    std::vector<size_t> gScore(m_n, UINT64_MAX);
    std::vector<size_t> fScore(m_n, UINT64_MAX);
    std::vector<size_t> previous(m_n, UINT64_MAX);
    std::vector<bool> visited(m_n, false);

    gScore[_sdeb] = 0;
    fScore[_sdeb] = glm::distance(m_vertices[_sdeb], m_vertices[_sfin]);

    for (size_t i = 0; i < m_n; i++) {
        size_t u = UINT64_MAX;
        float minFScore = UINT64_MAX;

        for (size_t j = 0; j < m_n; j++) {
            if (!visited[j] && fScore[j] < minFScore) {
                u = j;
                minFScore = fScore[j];
            }
        }

        if (u == UINT64_MAX || u == _sfin)
            break;

        visited[u] = true;

        for (size_t v : m_neighbours[u]) {
            if (v <= m_n && !visited[v]) {
                size_t tentativeGScore = gScore[u] + 1;
                if (tentativeGScore < gScore[v]) {
                    previous[v] = u;
                    gScore[v] = tentativeGScore;
                    fScore[v] = gScore[v] + glm::distance(m_vertices[v], m_vertices[_sfin]);
                }
            }
        }
    }

    std::vector<size_t> path;
    size_t curr = _sfin;
    while (curr != UINT64_MAX) {
        path.push_back(curr);
        curr = previous[curr];
    }
    std::reverse(path.begin(), path.end());

    return path;
}