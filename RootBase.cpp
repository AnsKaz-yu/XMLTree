#include "RootBase.h"
#include <utility>
#include <algorithm>

RootBase::RootBase(std::string name, double value, weak_rootbase_ptr parent) {
    m_name = std::move(name);
    m_value = value;
    m_parent = std::move(parent);
}

void RootBase::setParent(weak_rootbase_ptr parent) {
    m_parent = std::move(parent);
}

std::string RootBase::getName() const {
    return m_name;
}

double RootBase::getValue() const {
    return m_value;
}

void RootBase::addChild(const shared_rootbase_ptr& child) {
    m_children.push_back(child);
}

void RootBase::addChildren(vec_shared_rootbase_ptr vec) {
    for (auto& u: vec){
        m_children.push_back(u);
    }
}

void RootBase::deleteChild(const shared_rootbase_ptr &child) {
    auto it = m_children.begin();
    while (it != m_children.end()){
        if (*it == child){
            m_children.erase(it);
            break;
        }
        it++;
    }
}

weak_rootbase_ptr RootBase::getParent() {
    return m_parent;
}

vec_shared_rootbase_ptr RootBase::getAllChildren() {
    return m_children;
}

void RootBase::for_each(const function_with_shared_ptr& first, const function_with_shared_ptr& last, const shared_rootbase_ptr &t) {
    first(t);
    for (auto& u: t->m_children){
        u->for_each(first, last, u);
    }
    last(t);
}

void RootBase::setNewParentForChildren(const weak_rootbase_ptr& parent) {
    for (auto& u : m_children){
        u->setParent(parent);
    }
}
