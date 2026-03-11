#include "Graph.hpp"

void Graph::_depthFirstGenerationRecursive(size_t _current_cell, std::mt19937 &_rng, std::unordered_set<size_t> &_visited, Graph &res) const {
    _visited.insert(_current_cell);
    std::vector<size_t> cell_neighbours(m_neighbours[_current_cell].begin(), m_neighbours[_current_cell].end());
    std::shuffle(cell_neighbours.begin(), cell_neighbours.end(), _rng);

    for (size_t i = 0; i < cell_neighbours.size(); i++) {
        if (_visited.find(cell_neighbours[i]) != _visited.end())
            continue;

        res.m_neighbours[_current_cell].insert(cell_neighbours[i]);
        res.m_neighbours[cell_neighbours[i]].insert(_current_cell);

        _depthFirstGenerationRecursive(cell_neighbours[i], _rng, _visited, res);
    }
}
Graph Graph::depthFirstGenerationRecursive(size_t _initial_cell) const {
    std::unordered_set<size_t> visited;
    std::mt19937 rng(std::random_device{}());
    Graph res;
    res.m_n = m_n;
    res.m_vertices = std::vector<glm::vec3>(m_vertices);
    res.m_neighbours.resize(m_n);
    for (size_t i = 0; i < m_n; i++) {
        res.m_neighbours[i] = {};
    }

    _depthFirstGenerationRecursive(_initial_cell, rng, visited, res);

    return res;
}

Graph Graph::depthFirstGenerationIterative(size_t _initial_cell) const {
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

Graph Graph::kruskalGeneration() const {
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