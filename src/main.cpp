// GLEW
#include <GL/glew.h>

// GLM
#include <glm/ext.hpp>
#include <glm/glm.hpp>

// GLFW
#include <GLFW/glfw3.h>

// IMGUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// USUAL INCLUDES
#include <execinfo.h>
#include <stdio.h>
#include <unistd.h>

#include <array>
#include <iostream>
#include <thread>
#include <unordered_set>
#include <vector>
#include <boost/algorithm/string/join.hpp>

#include "Camera.hpp"
#include "Graph.hpp"

using namespace std;

GLuint window_width = 1280, window_height = 720;
glm::vec2 cursor_pos = glm::vec2(0, 0);
glm::vec2 cursor_vel = glm::vec2(0, 0);
glm::vec2 scroll = glm::vec2(0, 0);
size_t user_pos = 0;
long user_next_pos = 0;
vector<size_t> user_neighbours;
unordered_set<size_t> user_nodes = {user_pos};
long user_path_pointer = 0;
GLFWwindow *window;
bool gaming = false;

std::vector<size_t> path;
Graph original, maze, path_graph;
bool display_original = false;
bool display_maze = true;
bool display_path = false;

#define ALL_GRAPH_TYPES "Grid\0Cube\0Circle\0Sphere\0SphereContour\0Loaded\0"
int original_type = 0, n = 10, nbCercles = 10, nbEtage = 10;
#define ALL_MESHES_STR "face\0killeroo\0man\0rhino2\0"
const std::vector<std::string> ALL_MESHES{"models/face.off", "models/killeroo.off", "models/man.off", "models/rhino2.off"}; 
int loaded_path = 0;
#define ALL_MAZE_ALGORITHM "depth-first (recursive)\0depth-first (iterative)\0Kruskal\0Prim's\0"
int maze_algo = 2;
bool animate_generation = false;
float animation_quasiseconds = 3.f;
#define ALL_PATHFINDING_ALGORITHM "Dijkstra\0A*\0"
int pathfinding_algo = 1, sdeb=0, sfin;

Camera camera;

void globalInit();

static void debugUserState(const char *tag = "state") {
    cout << "\n[USER DEBUG] " << tag << '\n';
    cout << "user_pos          = " << user_pos << '\n';
    cout << "user_next_pos     = " << user_next_pos << '\n';
    cout << "user_path_pointer = " << user_path_pointer << '\n';

    cout << "user_neighbours (" << user_neighbours.size() << "): ";
    for (const auto v : user_neighbours)
        cout << v << ", ";
    cout << '\n';

    cout << "user_nodes (" << user_nodes.size() << "): ";
    for (const auto v : user_nodes)
        cout << v << ", ";
    cout << "\n"
         << endl;
}

bool containedIn(unordered_set<size_t> set, size_t element) {
    return set.find(element) != set.end();
}

void setNextMove() {
    if (user_neighbours.size() == 0) {
        user_next_pos = -1;
        return;
    }
    // if (user_neighbours.size() == 1 && containedIn(user_nodes, user_neighbours[0])) {
    //     user_next_pos = -1;
    //     return;
    // }
    // do {
    //     std::cout << user_path_pointer << " " << user_neighbours.size() << std::endl;
    //     user_path_pointer = (user_path_pointer + 1) % user_neighbours.size();
    //     user_next_pos = user_neighbours[user_path_pointer];
    // } while (containedIn(user_nodes, user_next_pos));
    // std::cout << user_path_pointer << " " << user_neighbours.size() << std::endl;
    user_path_pointer = (user_path_pointer + 1) % user_neighbours.size();
    user_next_pos = user_neighbours[user_path_pointer];
}

void init_game() {
    user_pos = sdeb;
    user_nodes = {user_pos};
    unordered_set<size_t> neigh_set = maze.getNeighbours(user_pos);
    user_neighbours.clear();
    user_neighbours.insert(user_neighbours.end(), neigh_set.begin(), neigh_set.end());
    setNextMove();
}

void regenerateOriginal() {
    switch (original_type) {
    case 0:
        original = Graph::gridGraph(n);
        break;
    case 1:
        original = Graph::cubeGraph(n);
        break;
    case 2:
        original = Graph::circleGraph(n, nbCercles);
        break;
    case 3:
        original = Graph::sphereGraph(n, nbCercles, nbEtage);
        break;
    case 4:
        original = Graph::sphereContourGraph(n, nbEtage);
        break;
    case 5:
        original = Graph::loadOFF(ALL_MESHES[loaded_path]);
        break;
    default:
        throw std::runtime_error("Unimplemented graph_type in regenerateOriginal");
    }
}

void regenerateMaze() {
    uint counter = 0;
    auto callback = [&counter](Graph &_g) {
        if (!animate_generation || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            return;
        }

        if (++counter < 3 * _g.getN() / (250.f * animation_quasiseconds))
            return;

        counter = 0;

        _g.draw(glm::vec3(1.f, 0.831373f, 0.211765f), 3.f);
        if (display_original) {
            original.draw(glm::vec3(1.f, 0.f, 0.f), 0.5f);
        }

        glFlush();
        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    };

    switch (maze_algo) {
    case 0:
        maze = original.depthFirstRecursiveGeneration(callback);
        break;
    case 1:
        maze = original.depthFirstIterativeGeneration(callback);
        break;
    case 2:
        maze = original.kruskalGeneration(callback);
        break;
    case 3:
        maze = original.primGeneration(callback);
        break;
    default:
        throw std::runtime_error("Unimplemented maze_algo in regenerateMaze");
    }
}

void regeneratePath() {
    sdeb = glm::clamp(sdeb, 0, int(maze.getN() - 1));
    sfin = glm::clamp(sfin, 0, int(maze.getN() - 1));
    
    switch (pathfinding_algo) {
    case 0:
        path = maze.dijkstra(sdeb, sfin);
        break;
    case 1:
        path = maze.a_star(sdeb, sfin);
        break;
    default:
        throw std::runtime_error("Unimplemented pathfinding_algo in regeneratePath");
    }
    path_graph = maze.subPath(path);
}

bool updateInterface(float _deltaTime) {
    float disable_mouse_actions = false;
    if (ImGui::Begin("Maze Interface")) {
        disable_mouse_actions = ImGui::IsWindowHovered() || ImGui::IsAnyItemHovered() || ImGui::IsAnyItemActive() || ImGui::IsAnyItemFocused();

        ImGui::SeparatorText("Displays");
        ImGui::Spacing();

        ImGui::Checkbox("display original", &display_original);
        ImGui::Checkbox("display maze", &display_maze);
        ImGui::Checkbox("display path", &display_path);
        bool freecam = (camera.m_type == CameraFree);
        if (ImGui::Checkbox("freecam", &freecam)) {
            camera.m_type = freecam ? CameraFree : CameraOrbital;
        }

        ImGui::Spacing();
        ImGui::SeparatorText("Original graph");
        ImGui::Spacing();

        ImGui::Combo("Graph type", &original_type, ALL_GRAPH_TYPES);
        ImGui::Checkbox("animate generation", &animate_generation);
        ImGui::DragFloat("duration quasi-seconds", &animation_quasiseconds, 0.01f, 0.5f, 60.f);
        if (original_type != 5) {
            ImGui::DragInt("n", &n, 1.f, 2, 100);
        }
        if (original_type == 2 || original_type == 3) { // circle
            ImGui::DragInt("nb cercles", &nbCercles, 1.f, 2, 100);
        }
        if (original_type == 3 || original_type == 4) { // circle
            ImGui::DragInt("nb etages", &nbEtage, 1.f, 2, 100);
        }
        if (original_type == 5) {
            ImGui::Combo("Mesh path", &loaded_path, ALL_MESHES_STR);
        }
        if (ImGui::Button("Regenerate##original")) {
            regenerateOriginal();
            regenerateMaze();
            regeneratePath();
            init_game();
            setNextMove();
        }

        ImGui::Spacing();
        ImGui::SeparatorText("Maze");
        ImGui::Spacing();

        ImGui::Combo("algorithm##maze", &maze_algo, ALL_MAZE_ALGORITHM);
        if (ImGui::Button("Regenerate##maze")) {
            regenerateMaze();
            regeneratePath();
            init_game();
            setNextMove();
        }

        ImGui::Spacing();
        ImGui::SeparatorText("Pathfinding");
        ImGui::Spacing();
        ImGui::Combo("algorithm##path", &pathfinding_algo, ALL_PATHFINDING_ALGORITHM);
        if (ImGui::Button("randomize##sdeb")) {
            sdeb = rand()%int(maze.getN() - 1);
        }
        ImGui::SameLine();
        if (ImGui::DragInt("début", &sdeb, 1.f, 1, maze.getN() - 1)) {
            sdeb = glm::clamp(sdeb, 0, int(maze.getN() - 1));
        }

        if (ImGui::Button("randomize##sfin")) {
            sfin = rand()%int(maze.getN() - 1);
        }
        ImGui::SameLine();
        if (ImGui::DragInt("end", &sfin, 1.f, 1, maze.getN() - 1)) {
            sfin = glm::clamp(sfin, 0, int(maze.getN() - 1));
        }

        if (ImGui::Button("Recompute##path")) {
            regeneratePath();
            init_game();
            setNextMove();
        }
        ImGui::SameLine();
        if (ImGui::Button("last")) {
            sfin = maze.getN() - 1;
        }

        ImGui::Spacing();
        ImGui::SeparatorText("Play !!!");
        ImGui::Spacing();
        if (ImGui::Checkbox("Gaming", &gaming)) {
            camera.m_center = gaming ? &original.getVertex(user_pos) : &VEC_ZERO;
            init_game();
            setNextMove();
        }
    }

    ImGui::End();
    return disable_mouse_actions;
}

int main(void) {
    globalInit();

    // Objects initialisation
    regenerateOriginal();
    sfin = original.getN() - 1;
    regenerateMaze();
    regeneratePath();

    unordered_set<size_t> neigh_set = maze.getNeighbours(user_pos);
    user_neighbours.insert(user_neighbours.end(), neigh_set.begin(), neigh_set.end());
    setNextMove();

    // timings
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    size_t frame_count = 0;
    glfwSwapInterval(1); // VSync - avoid having 3000000 fps
    do {
        glFlush();
        glfwSwapBuffers(window);
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        frame_count++;
        unsigned int fps = static_cast<unsigned int>(1 / deltaTime);
        char titleString[64];
        snprintf(titleString, sizeof(titleString), "Projet Labyrinthe - FPS : %u", fps);
        glfwSetWindowTitle(window, titleString);

        // Imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // OBJECTS UPDATE
        float disable_mouse_actions = updateInterface(deltaTime);
        disable_mouse_actions = camera.updateInterface() || disable_mouse_actions;
        camera.update(window, deltaTime, cursor_vel, scroll, disable_mouse_actions);

        // Legacy OpenGL: feed camera matrices to the fixed-function pipeline.
        glm::mat4 projection = camera.getProjectionMatrix();
        glm::mat4 view = camera.getViewMatrix();

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(glm::value_ptr(projection));

        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(view));

        // Objects rendering
        if (display_path) {
            path_graph.draw(glm::vec3(0.32f, 0.64f, 0.85f), 5.f);
        }
        if (display_maze) {
            maze.draw(glm::vec3(1.f, 0.831373f, 0.211765f), 3.f, user_nodes, user_next_pos, user_pos, sfin, gaming);
        }
        if (display_original) {
            original.draw(glm::vec3(1.f, 0.f, 0.f), 0.5f);
        }

        // ImGui Render
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Reset some controls
        scroll = glm::vec2(0.);
        cursor_vel = glm::vec2(0.);
    } while (glfwWindowShouldClose(window) == GLFW_FALSE);

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // cout << "framebuffer size: " << width << ", " << height << endl;
    window_width = width;
    window_height = height;
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    const bool pressed = (action == GLFW_PRESS);
    // const bool repeated = (action == GLFW_REPEAT);
    // const bool press_or_repeat = pressed || repeated;

    // cout << "key:" << key << " scancode:" << scancode << " action:" << action << " mods:" << mods << endl;
    if (key == GLFW_KEY_TAB && pressed) {
        // if (user_nodes.find(user_next_pos) != user_nodes.end()) {
        //     cout << "on est kéblos !!!" << endl;
        //     return;
        // }
        camera.m_center = gaming ? &original.getVertex(user_next_pos) : &VEC_ZERO;
        user_pos = user_next_pos;
        user_nodes.insert(user_pos);
        unordered_set<size_t> neigh_set = maze.getNeighbours(user_pos);
        user_neighbours.clear();
        user_neighbours.insert(user_neighbours.end(), neigh_set.begin(), neigh_set.end());
        user_path_pointer = 0;
        user_next_pos = user_neighbours[user_path_pointer];
        if (containedIn(user_nodes, user_next_pos)) {
            user_path_pointer = (user_path_pointer + 1) % user_neighbours.size();
        }
        user_next_pos = user_neighbours[user_path_pointer];
    } else if (key == GLFW_KEY_LEFT_SHIFT && pressed) {
        if (user_neighbours.size() > 0) {
            // cout << user_path_pointer << " -> ";
            setNextMove();
            // cout << user_path_pointer << endl;
        }
    } else if (key == GLFW_KEY_U && pressed) {
        debugUserState("debug");
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    // cout << "mouse button:" << button << " action:" << action << " mods:" << mods << endl;
    // if (button == GLFW_MOUSE_BUTTON_LEFT) {
    //     glfwSetInputMode(window, GLFW_CURSOR, action == GLFW_PRESS ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    // }
}

void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {
    cursor_vel.x = xpos - cursor_pos.x;
    cursor_vel.y = ypos - cursor_pos.y;
    cursor_pos.x = xpos;
    cursor_pos.y = ypos;
    // cout << "cursor_pos: (" << cursor_pos.x << ", " << cursor_pos.y << ")\tcursor_vel: (" << cursor_vel.x << ", " << cursor_vel.y << ")" << endl;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    // cout << "scroll: (" << xoffset << ", " << yoffset << ")" << endl;
    scroll.x = xoffset;
    scroll.y = yoffset;
}

void initWindow() {
    glfwWindowHint(GLFW_SAMPLES, 4);
    // Request a legacy-compatible context so fixed-function calls
    // like glBegin/glEnd, glColor3f and glVertex3f are available.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#if defined(__APPLE__)
    // macOS does not provide OpenGL 2.1 contexts with modern GLFW; keep the previous 3.3 core fallback there.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
#endif
    glfwWindowHint(GLFW_SCALE_FRAMEBUFFER, GL_FALSE); // https://discourse.glfw.org/t/resizing-window-results-in-wrong-aspect-ratio/1268s

    window = glfwCreateWindow(window_width, window_height, "ImGui OpenGL3 example", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void initOpenGL() {
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE); // Ensure we can capture the escape key being pressed below

    // Ensure viewport matches framebuffer from first frame.
    int fb_w = 0, fb_h = 0;
    glfwGetFramebufferSize(window, &fb_w, &fb_h);
    glViewport(0, 0, fb_w, fb_h);

    glClearColor(0.1f, 0.1f, 0.3f, 0.0f); // Dark blue background
    glEnable(GL_DEPTH_TEST);              // Enable depth test
    glDepthFunc(GL_LESS);                 // Accept fragment if it closer to the camera than the former one
    glDisable(GL_CULL_FACE);              // Keep single-face geometry visible while orbiting the camera
    glEnable(GL_POINT_SMOOTH);
}

void globalInit() {
#if defined(__linux__)
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
    // INITIALIZE GLFW
    if (!glfwInit()) {
        cerr << "Failed to initialize GLFW!" << endl;
        exit(EXIT_FAILURE);
    }
    initWindow();

    // INITIALIZE GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK && err != 4) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }

    // INITIALIZE IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGuiIO &io = ImGui::GetIO();
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // IF using Docking Branch
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init("#version 120");

    initOpenGL();
}
