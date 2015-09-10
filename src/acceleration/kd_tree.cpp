#include <algorithm>
#include <kd_tree.h>
#include <stack>

namespace lumen {
static const float TRAVERSAL_COST = 1.0f;
static const float INTERSECTION_COST = 64.0f;

struct edge {
        geometry_ptr parent;
        float position;
        bool is_min;
};

struct kd_tree::node {
        std::vector<geometry_ptr> geometry;

        int split_axis;
        float split_pos;

        node* left_child;
        node* right_child;

        bool is_external;
};

struct kd_tree::traversal_node {
        const kd_tree::node* next;
        float min;
        float max;
};

static bool operator<(const edge& lhs, const edge& rhs);

static float calculate_surface_area(const bound& bound);

static void get_bound_edges(const std::vector<geometry_ptr>& geometry,
        int axis, std::vector<edge>* edges);

static void surface_area_heuristic(const std::vector<edge>& edges,
        const bound& node_bound, int axis, float* best_split,
        size_t* best_index, bound* left_bound, bound* right_bound);

static void split_geometry(const std::vector<edge>& edges, size_t split_idx,
        std::vector<geometry_ptr>* left_geometry, std::vector<geometry_ptr>* right_geometry);

static void intersect_internal_node(const kd_tree::node*& next_node,
        const nex::ray& ray, std::stack<kd_tree::traversal_node>& node_stack,
        const float tmin, float* tmax);

static bool intersect_external_node(const kd_tree::node*& next_node,
        nex::ray* ray, surface* surface, bool* intersect);

kd_tree::kd_tree(const std::vector<geometry_ptr>& geometry, int max_depth, int min_geometry) :
        acceleration_structure(geometry),
        nodes(),
        max_depth(max_depth),
        min_geometry(min_geometry)
{
        build(geometry, get_world_bound(), 0);
}

kd_tree::~kd_tree()
{
        for (auto node : nodes) {
                delete node;
        }
}

bool kd_tree::intersect_geometry(nex::ray* ray, surface* surface) const
{
        float tmin = 0.0f;
        float tmax = 0.0f;
        get_world_bound().intersect(*ray, &tmin, &tmax);

        bool intersect = false;

        std::stack<traversal_node> node_stack;

        const node* next_node = nodes[0];

        while (next_node != nullptr) {
                if (ray->max < tmin) {
                        break;
                }

                if (!next_node->is_external) {
                        intersect_internal_node(next_node, *ray, node_stack, tmin, &tmax);
                } else {
                        intersect_external_node(next_node, ray, surface, &intersect);

                        if (node_stack.empty()) {
                                break;
                        } else {
                                traversal_node node = node_stack.top();
                                node_stack.pop();

                                next_node = node.next;
                                tmin = node.min;
                                tmax = node.max;
                        }
                }
        }

        return intersect;
}

bool kd_tree::intersect_shadow(nex::ray* ray) const
{
        float tmin = 0.0f;
        float tmax = 0.0f;
        get_world_bound().intersect(*ray, &tmin, &tmax);

        std::stack<traversal_node> node_stack;

        const node* next_node = nodes[0];

        while (next_node != nullptr) {
                if (ray->max < tmin) {
                        break;
                }

                if (!next_node->is_external) {
                        intersect_internal_node(next_node, *ray, node_stack, tmin, &tmax);
                } else {
                        if (intersect_external_node(next_node, ray, nullptr, nullptr)) {
                                return true;
                        }

                        if (node_stack.empty()) {
                                break;
                        } else {
                                traversal_node node = node_stack.top();
                                node_stack.pop();

                                next_node = node.next;
                                tmin = node.min;
                                tmax = node.max;
                        }
                }
        }

        return false;
}

kd_tree::node* kd_tree::build(const std::vector<geometry_ptr>& geometry, const bound& node_bound, int depth)
{
        if ((depth <= max_depth) && (geometry.size() >= static_cast<size_t>(min_geometry))) {
                return create_internal_node(geometry, node_bound, depth);
        } else {
                return create_external_node(geometry);
        }
}

kd_tree::node* kd_tree::create_internal_node(const std::vector<geometry_ptr>& geometry,
        const bound& node_bound, int depth)
{
        // get the next split plane
        int axis = depth % 3;

        // get the bounding volume edges of all the geometry
        std::vector<edge> edges;
        get_bound_edges(geometry, axis, &edges);

        // calculate the best split position
        float best_split = FLT_MAX;
        size_t best_index = -1;
        bound left_bound;
        bound right_bound;

        surface_area_heuristic(edges, node_bound, axis, &best_split, &best_index, &left_bound, &right_bound);

        // create a new internal node
        node* internal_node = new node;
        nodes.push_back(internal_node);

        // if there was a valid split point, then create an internal node;
        // however, if no split position was found, then just create an external node
        if (best_index != -1) {
                std::vector<geometry_ptr> left_geometry;
                std::vector<geometry_ptr> right_geometry;
                split_geometry(edges, best_index, &left_geometry, &right_geometry);

                internal_node->split_axis = axis;
                internal_node->split_pos = best_split;
                internal_node->left_child = build(left_geometry, left_bound, depth + 1);
                internal_node->right_child = build(right_geometry, right_bound, depth + 1);
                internal_node->is_external = false;
        } else {
                internal_node->geometry = geometry;
                internal_node->split_axis = 0;
                internal_node->split_pos = 0.0f;
                internal_node->left_child = nullptr;
                internal_node->right_child = nullptr;
                internal_node->is_external = true;
        }

        return internal_node;
}

kd_tree::node* kd_tree::create_external_node(const std::vector<geometry_ptr>& geometry)
{
        node* external_node = new node;
        nodes.push_back(external_node);

        external_node->geometry    = geometry;
        external_node->split_axis  = 0;
        external_node->split_pos   = 0.0f;
        external_node->left_child  = nullptr;
        external_node->right_child = nullptr;
        external_node->is_external = true;

        return external_node;
}

static bool operator<(const edge& lhs, const edge& rhs)
{
        return fabs(lhs.position - rhs.position) < 0.0001f
                ? lhs.is_min < rhs.is_min
                : lhs.position < rhs.position;
}

static float calculate_surface_area(const bound& bound)
{
        const nex::point& min = bound.min;
        const nex::point& max = bound.max;

        float width  = max.x - min.x;
        float height = max.y - min.y;
        float depth  = max.z - min.z;

        return 2.0f * (width * height + width * depth + height * depth);
}

static void get_bound_edges(const std::vector<geometry_ptr>& geometry,
        int axis, std::vector<edge>* edges)
{
        // get the min and max bounding edges for all geometry
        edges->reserve(2 * geometry.size());

        for (const auto& g : geometry) {
                const nex::point& min = g->world_bound().min;
                const nex::point& max = g->world_bound().max;

                edge min_edge;
                min_edge.parent = g;
                min_edge.position = min[axis];
                min_edge.is_min = true;

                edge max_edge;
                max_edge.parent = g;
                max_edge.position = max[axis];
                max_edge.is_min = false;

                edges->push_back(min_edge);
                edges->push_back(max_edge);
        }

        std::sort(edges->begin(), edges->end());
}

static void surface_area_heuristic(const std::vector<edge>& edges,
        const bound& node_bound, int axis, float* best_split,
        size_t* best_index, bound* left_bound, bound* right_bound)
{
        float node_area = calculate_surface_area(node_bound);

        // find the optimal split position
        float best_cost = FLT_MAX;
        size_t num_left = 0;
        size_t num_right = edges.size() / 2;

        for (size_t i = 0; i < edges.size(); ++i) {
                const edge& edge = edges[i];

                float pos = edge.position;

                if (!edge.is_min) {
                        --num_right;
                }

                // don't consider edges on the ends since we want to split
                // somewhere inside the node
                if ((pos > node_bound.min[axis]) && (pos < node_bound.max[axis])) {
                        // split the parent bound in half
                        bound lbound = node_bound;
                        lbound.max[axis] = pos;

                        bound rbound = node_bound;
                        rbound.min[axis] = pos;

                        // calculate the splitting cost for the edge
                        float left_area = calculate_surface_area(lbound);
                        float right_area = calculate_surface_area(rbound);

                        float left_prob = left_area * num_left / node_area;
                        float right_prob = right_area * num_right / node_area;

                        float cur_cost = TRAVERSAL_COST + INTERSECTION_COST * (left_prob + right_prob);

                        if (cur_cost < best_cost) {
                                best_cost = cur_cost;

                                *best_split = pos;
                                *best_index = i;
                                *left_bound = lbound;
                                *right_bound = rbound;
                        }
                }

                if (edge.is_min) {
                        ++num_left;
                }
        }
}

static void split_geometry(const std::vector<edge>& edges, size_t split_idx,
        std::vector<geometry_ptr>* left_geometry, std::vector<geometry_ptr>* right_geometry)
{
        // split the geometry into the left and right nodes
        size_t num_geometry = edges.size() / 2;

        left_geometry->reserve(num_geometry);
        right_geometry->reserve(num_geometry);

        // if the edge is a minimum then it intersects the left child
        for (size_t i = 0; i < split_idx; ++i) {
                if (edges[i].is_min) {
                        left_geometry->push_back(edges[i].parent);
                }
        }

        // if the edge is a maximum then it intersects the right child
        for (size_t j = split_idx + 1; j < edges.size(); ++j) {
                if (!edges[j].is_min) {
                        right_geometry->push_back(edges[j].parent);
                }
        }
}

static void intersect_internal_node(const kd_tree::node*& next_node,
        const nex::ray& ray, std::stack<kd_tree::traversal_node>& node_stack,
        const float tmin, float* tmax)
{
        // find the distance to the split plane (ray-plane intersection)
        int axis = next_node->split_axis;

        float t = (next_node->split_pos - ray.origin[axis]) / ray.direction[axis];

        kd_tree::node* first  = nullptr;
        kd_tree::node* second = nullptr;

        // the left node needs to be checked first if the ray origin lies to the
        // left of the split position or if it lies on the split plane and
        // points into the left node
        bool left_first = (ray.origin[axis] < next_node->split_pos) ||
                ((fabs(ray.origin[axis] - next_node->split_pos) < 0.0001f) &&
                 (ray.direction[axis] <= 0.0f));

        if (left_first) {
                first = next_node->left_child;
                second = next_node->right_child;
        } else {
                first = next_node->right_child;
                second = next_node->left_child;
        }

        if ((t > *tmax) || (t <= 0.0f)) {
                next_node = first;
        } else if (t < tmin) {
                next_node = second;
        } else {
                kd_tree::traversal_node node;
                node.next = second;
                node.min = t;
                node.max = *tmax;

                node_stack.push(node);

                next_node = first;
                *tmax = t;
        }
}

static bool intersect_external_node(const kd_tree::node*& next_node,
        nex::ray* ray, surface* surface, bool* intersect)
{
        if (surface) {
                for (const auto& g : next_node->geometry) {
                        if (g->intersect(ray, surface)) {
                                *intersect = true;
                        }
                }

                return *intersect;
        } else {
                for (const auto& g : next_node->geometry) {
                        if (g->intersect(ray)) {
                                return true;
                        }
                }

                return false;
        }
}
}
