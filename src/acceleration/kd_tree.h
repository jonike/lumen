#ifndef LUMEN_KDTREE_H
#define LUMEN_KDTREE_H

#include <acceleration_structure.h>
#include <bound.h>
#include <geometry.h>
#include <vector>

namespace lumen {
class kd_tree : public acceleration_structure {
public:
        struct node;
        struct traversal_node;

        kd_tree(const std::vector<geometry_ptr>& geometry, int max_depth, int min_geometry);
        virtual ~kd_tree();
private:
        virtual bool intersect_geometry(nex::ray*, surface*) const;
        virtual bool intersect_shadow(nex::ray*) const;

        node* build(const std::vector<geometry_ptr>& geometry, const bound& node_bound, int depth);
        node* create_internal_node(const std::vector<geometry_ptr>& geometry, const bound& node_bound, int depth);
        node* create_external_node(const std::vector<geometry_ptr>& geometry);

        std::vector<node*> nodes;
        const int max_depth;
        const int min_geometry;
};
}

#endif
