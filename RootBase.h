#ifndef LAB2_XMLTREE_ROOTBASE_H
#define LAB2_XMLTREE_ROOTBASE_H
#include <memory>
#include <string>
#include <vector>
#include <functional>
class RootBase;
using weak_rootbase_ptr = std::weak_ptr<RootBase>;
using shared_rootbase_ptr = std::shared_ptr<RootBase>;
using vec_shared_rootbase_ptr = std::vector<shared_rootbase_ptr>;
using function_with_shared_ptr = std::function<void(const shared_rootbase_ptr&)>;

class RootBase {
    vec_shared_rootbase_ptr m_children;
    weak_rootbase_ptr m_parent;
    std::string m_name;
    double m_value;
public:
    RootBase(std::string name, double value, weak_rootbase_ptr parent);

    void setParent(weak_rootbase_ptr parent);
    weak_rootbase_ptr getParent();

    [[nodiscard]] std::string getName() const;
    [[nodiscard]] double getValue() const;

    void addChild(const shared_rootbase_ptr& child);
    void addChildren(vec_shared_rootbase_ptr vec);
    void setNewParentForChildren(const weak_rootbase_ptr& parent);
    vec_shared_rootbase_ptr getAllChildren();

    void deleteChild(const shared_rootbase_ptr& child);

    static void for_each(const function_with_shared_ptr& first, const function_with_shared_ptr& last, const shared_rootbase_ptr& t);
    ~RootBase() = default;
};




#endif //LAB2_XMLTREE_ROOTBASE_H
