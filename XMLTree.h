#ifndef LAB2_XMLTREE_XMLTREE_H
#define LAB2_XMLTREE_XMLTREE_H
#include "RootBase.h"
#include <memory>
#include <list>

class XMLTree;
using unique_xmltree_ptr = std::unique_ptr<XMLTree>;
using iterator = std::list<shared_rootbase_ptr>::iterator;

class XMLTree : private unique_xmltree_ptr {
public:
    //for iterator
    iterator begin();
    iterator end();

    //fabric function
    static unique_xmltree_ptr create(const std::string& path);

    //working with files
    void save(const std::string& path);
    void load(const std::string& path);

    //changing the tree functions
    iterator find(const std::string& name);
    iterator find(double value);

    bool erase(iterator it);
    iterator add(const std::string& name, double value, iterator parent);

    //destructor
    ~XMLTree();
private:
    class InnerXML; //auxiliary class for calling the constructor in the create function
    XMLTree() = default;
    std::list<shared_rootbase_ptr> MyTree; //list with pointers to the roots of the tree
};

class XMLTree::InnerXML : public XMLTree //thanks to inheritance, a pointer to InnerXML can be passed as a pointer to XMLTree
{
public:
    using XMLTree::XMLTree;
};

#endif //LAB2_XMLTREE_XMLTREE_H
