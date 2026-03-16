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

Graph Graph::circleGraph(float _n, float _nbCercles) {
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

Graph Graph::sphereGraph(float _nbCercles, float _n, float nbEtage) {
    Graph G;

    
    G.m_n = (_nbCercles * _n + 1) * (nbEtage-2) +2;
    G.m_vertices.push_back(glm::vec3(0.,-0.5, 0.));
    G.m_neighbours.push_back({});

    for (float etage = 1; etage < nbEtage-1 ; etage++){
        int offset = (_nbCercles * _n + 1) * (etage -1) +1;
        
        G.m_vertices.push_back(glm::vec3(0., etage /(nbEtage-1) -0.5, 0.));
        G.m_neighbours.push_back({});
        for (float i = 1; i < _nbCercles + 1; i++) {
            float r = sin(acos((etage /(nbEtage-1) -0.5)* 2))*0.5f;
            r = r * (i/(_nbCercles));
            for (float j = 0; j < _n; j++) {
                float theta = (j / _n) * 2 * M_PI;
                G.m_vertices.push_back(glm::vec3(r * cos(theta), etage /(nbEtage-1) -0.5, r * sin(theta)));
                G.m_neighbours.push_back({});
            }
        }

        for (float j = 0; j < _n; j++) {
            size_t v0 = (j + 1) + offset;
            G.m_neighbours[v0].insert(0 + offset);
            G.m_neighbours[0 + offset].insert(v0);

            if (j != 0) {
                size_t v2 = j + offset;
                G.m_neighbours[v0].insert(v2);
                G.m_neighbours[v2].insert(v0);
            }
            if (j == _n - 1) {
                size_t v2 = 1 + offset;
                G.m_neighbours[v0].insert(v2);
                G.m_neighbours[v2].insert(v0);
            }
        }
        G.m_neighbours[_n + 1 + offset].insert(1 + offset);
        G.m_neighbours[1 + offset].insert(_n + 1 + offset);
        for (float i = 1; i < _nbCercles; i++) {
            for (float j = 0; j < _n; j++) {
                size_t v0 = i * _n + j + 1 + offset;
                if (j != 0) {
                    size_t v1 = i * _n + j + offset;
                    G.m_neighbours[v0].insert(v1);
                    G.m_neighbours[v1].insert(v0);
                }
                size_t v2 = (i - 1) * _n + j + 1 + offset;
                G.m_neighbours[v0].insert(v2);
                G.m_neighbours[v2].insert(v0);
                if (j == _n - 1) {
                    size_t v1 = i * _n + 1 + offset;
                    G.m_neighbours[v0].insert(v1);
                    G.m_neighbours[v1].insert(v0);
                }
            }
        }

        if (etage == 1.f){
            for (size_t i = 1; i < (_nbCercles * _n + 1) +1; i++){
                G.m_neighbours[0].insert(i);
                G.m_neighbours[i].insert(0);
            }
        }else{
            for (size_t i = offset; i < offset + (_nbCercles * _n + 1); i++){
                G.m_neighbours[i-(_nbCercles * _n + 1)].insert(i);
                G.m_neighbours[i].insert(i-(_nbCercles * _n + 1));
            }
        }
        
    }
    G.m_vertices.push_back(glm::vec3(0.,0.5, 0.));
    G.m_neighbours.push_back({});
    for (size_t i = G.m_n - (_nbCercles * _n + 1) -1 ; i < G.m_n - 1; i++){
                G.m_neighbours[G.m_n - 1].insert(i);
                G.m_neighbours[i].insert(G.m_n - 1);
    }

    return G;
}

Graph Graph::sphereContourGraph(float _n, float nbEtage) {
    Graph G;

    
    G.m_n = (_n) * (nbEtage-2) +2;
    G.m_vertices.push_back(glm::vec3(0.,-0.5, 0.));
    G.m_neighbours.push_back({});

    for (float etage = 1; etage < nbEtage-1 ; etage++){
        int offset = (_n) * (etage -1) +1;
        
        float r = sin(acos((etage /(nbEtage-1) -0.5)* 2))*0.5f;
        for (float j = 0; j < _n; j++) {
            float theta = (j / _n) * 2 * M_PI;
            G.m_vertices.push_back(glm::vec3(r * cos(theta), etage /(nbEtage-1) -0.5, r * sin(theta)));
            G.m_neighbours.push_back({});
        }

        for (float j = 0; j < _n; j++) {
            size_t v0 = j + offset;

            if (j != 0) {
                size_t v2 = j + offset - 1;
                G.m_neighbours[v0].insert(v2);
                G.m_neighbours[v2].insert(v0);
            }

            if (j == _n - 1) {
                size_t v2 = offset;
                G.m_neighbours[v0].insert(v2);
                G.m_neighbours[v2].insert(v0);
            }
        }
        G.m_neighbours[offset + _n - 1].insert(offset);
        G.m_neighbours[offset].insert(offset + _n - 1);
        if (etage == 1.f){
            for (size_t i = 1; i < (_n) +1; i++){
                G.m_neighbours[0].insert(i);
                G.m_neighbours[i].insert(0);
            }
        }else{
            for (size_t i = offset; i < offset + _n; i++){
                G.m_neighbours[i- _n].insert(i);
                G.m_neighbours[i].insert(i-_n);
            }
        }
    }
    G.m_vertices.push_back(glm::vec3(0.,0.5, 0.));
    G.m_neighbours.push_back({});
    for (size_t i = G.m_n - _n - 1 ; i < G.m_n - 1; i++){
                G.m_neighbours[G.m_n - 1].insert(i);
                G.m_neighbours[i].insert(G.m_n - 1);
    }

    return G;
}