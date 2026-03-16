#include "Graph.hpp"

std::vector<size_t> Graph::dijkstra(size_t _sdeb, size_t _sfin) const {
    std::vector<size_t> distances(m_n, UINT64_MAX);
    std::vector<size_t> previous(m_n, UINT64_MAX);
    std::vector<bool> visited(m_n, false);

    previous[_sdeb] == UINT64_MAX;
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