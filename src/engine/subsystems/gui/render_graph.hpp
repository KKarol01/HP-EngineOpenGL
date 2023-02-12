#pragma once

#include <vector>
#include <string>
#include <memory>
#include <any>
#include <unordered_map>
#include <functional>

#include <glm/glm.hpp>

struct ImDrawList;

enum class NodeType { DepthTest, VAO, Stage };

struct Node {
  public:
    auto start() const { return position; }
    auto end() const { return position + size; }

    NodeType type{0u};
    glm::vec2 position{0.f};
    glm::vec2 min_size, max_size;
    glm::vec2 size{150.f, 50.f};

    glm::vec2 drag_start{0.f}, drag_end{0.f};
    bool clicked{false}, hovered{false}, down{false};
    bool resizing{false}, moving{false};
    bool dragging_new_link{false};
    bool opened{true};
    std::string hovered_dot;
    uint32_t id = gid++;
    std::unordered_map<std::string, std::any> storage;

    inline static float corner_rounding = 5.f;
    inline static float border_size     = .7f;
    inline static float padding         = 2.f;
    inline static uint32_t gid          = 0u;
};

struct RenderGraphGUI;

struct NodeConnection {

    NodeConnection(RenderGraphGUI *parent,
                   Node *from,
                   Node *to,
                   glm::vec2 from_off,
                   glm::vec2 to_off,
                   std::string from_name,
                   std::string to_name)
        : parent{parent}, from_id{from->id}, to_id{to->id}, fromoff{from_off}, tooff{to_off}, from_name{from_name},
          to_name{to_name} {}

    glm::vec2 get_from_position() const;
    glm::vec2 get_to_position() const;

    std::string get_from_name() const;
    std::string get_to_name() const;

  private:
    RenderGraphGUI *parent{nullptr};
    uint32_t from_id, to_id;
    glm::vec2 fromoff, tooff;
    std::string from_name, to_name;
};

struct NodeBuilder {
    Node build(NodeType type);
};

class RenderGraphGUI {
    struct Colors {
        inline static glm::u8vec4 grid{200, 200, 200, 40};
        inline static glm::u8vec4 node_clicked{255, 255, 255, 255};
        inline static glm::u8vec4 node_hovered{150, 150, 150, 255};
        inline static glm::u8vec4 node{50, 50, 50, 255};
        inline static glm::u8vec4 node_outline{255};
        inline static glm::u8vec4 window{30, 30, 30, 255};

        inline static std::unordered_map<NodeType, glm::u8vec4> menubar{
            {{NodeType::VAO, {227, 177, 39, 255}}, {NodeType::Stage, {86, 56, 150, 255}}}};
    };

  public:
    RenderGraphGUI();

    void draw();
    bool is_open() const { return open; }
    void open_widget() { open = true; }

    int find_node_id(std::function<bool(Node *)> pred);
    Node *find_node_id(int);

  private:
    void draw_nodes();
    void draw_node_contents(Node *);
    void draw_resource_list();
    void add_node(NodeType type);
    void mouse_node_interactions();

    int inode = -1;
    std::vector<Node> nodes{};
    std::unordered_map<uint32_t, std::string> buffers_names;
    int editing_buffer_name;
    char *new_buffer_name{nullptr};

    glm::vec2 window_start{0.f}, window_size{0.f};
    glm::vec2 canvas_size{0.f};
    glm::vec2 canvas_start{0.f}, canvas_end{0.f};
    glm::vec2 pan_offset{0.f};
    float grid_step  = 64.f;
    float zoom_speed = 100.f;
    float scale{1.f};
    const float scale_min{0.01f}, scale_max{3.f};

    ImDrawList *bgdraw_list{nullptr};
    NodeBuilder node_builder;
    bool open{true};
    bool canvas_panning{false};
    bool resource_dragging{false};
    bool can_move_nodes{true};

    bool line_dragging{false};
    uint32_t line_dragging_node_of_origin;
    glm::vec2 line_dragging_start;
    std::string line_dragging_name;
};