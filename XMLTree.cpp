#include "XMLTree.h"
#include <fstream>
#include <sstream>
#include <stack>
#include <iostream>
#include <functional>
unique_xmltree_ptr XMLTree::create(const std::string& path) {
    //InnerXML inherited from XMLTree,
    //this way their pointers will be of the same type.
    //And if we create a pointer to InnerXml, we can pass it as a pointer to XmlTree.
    //The InnerXml class is declared as private in the XML Tree class,
    //so the user will not be able to access it directly.
    //This way we will bypass the ban on calling the XML Tree constructor,
    //which make_unique otherwise encounters.

    unique_xmltree_ptr ptr = std::make_unique<InnerXML>();

    try{
        ptr->load(path);
    }
    catch(const std::exception& e){
        std::cout<<"LOAD ERROR"<<std::endl;
    }

    return std::move(ptr);
}

void XMLTree::save(const std::string& path) {
    std::ofstream file;
    file.open(path, std::ios::out); //file connection
    if (!file.is_open()){
        throw std::exception();
    }

    function_with_shared_ptr func1 = [&file](const shared_rootbase_ptr& ptr){
        file<<"<"<<ptr->getName()<<"> "<<ptr->getValue()<<std::endl;
    };
    function_with_shared_ptr func2 = [&file](const shared_rootbase_ptr& ptr){
        file<<"<\\"<<ptr->getName()<<"> "<<std::endl;
    };

    RootBase::for_each(func1, func2, MyTree.back());
}

void XMLTree::load(const std::string& path) {
    MyTree.clear();

    std::ifstream file;
    file.open(path, std::ios::in); //file connection

    if (!file.is_open()){
       throw std::exception();
    }
    std::string line;
    std::stack<iterator> addingStack;

    //this empty vertex will be removed after loading the tree
    //it is needed only for the correct addition of the remaining vertices to the tree
    weak_rootbase_ptr base_parent;
    shared_rootbase_ptr base = std::make_shared<RootBase>(" ", 0, base_parent);

    MyTree.push_back(base);
    addingStack.push(MyTree.begin());

    while (std::getline(file, line)) {
        std::stringstream buffer;
        buffer << line;
        std::string command;
        while (buffer >> command) {
            if (command[0] == '<' && command[command.size() - 1] == '>') { //<root> or <\root>
                if (command[1] == '\\') { //<\root>
                    command.erase(0, 2);
                    command.erase(command.end() - 1);

                    if (addingStack.empty() || command != (*addingStack.top())->getName()) { //incorrect input
                        throw std::exception();
                    }

                    addingStack.pop();
                    continue;
                } else { //<root>
                    command.erase(0, 1);
                    command.erase(command.end() - 1);
                    double value;

                    if (!(buffer >> value)) { //incorrect input
                        throw std::exception();
                    }

                    auto root = this->add(command, value, addingStack.top());
                    addingStack.push(root);
                }
            } else {
                throw std::exception(); //incorrect input
            }
        }
    }
    if (addingStack.size() > 1){
        throw std::exception();
    }
    MyTree.erase(addingStack.top()); //removing the auxiliary empty vertex
    addingStack.pop();
}



bool XMLTree::erase(iterator it) {
    shared_rootbase_ptr ptr = *it;
    weak_rootbase_ptr parent = ptr->getParent();

    if (parent.expired()){
        std::cout<<"deleting error"<<std::endl; //we can't delete the main root
        return false;
    }

    try{
        MyTree.erase(it);
        parent.lock()->deleteChild(ptr);
        parent.lock()->addChildren(ptr->getAllChildren());
        ptr->setNewParentForChildren(parent);
        return true;
    }
    catch(...){
        std::cout<<"deleting error"<<std::endl;
    }
    return false;
}

iterator XMLTree::find(const std::string& name) {
    auto it = this->begin();
    while (it != this->end()){
        if ((*it)->getName() == name){
            break;
        }
        it++;
    }
    return it;
}

iterator XMLTree::find(double value) {
    auto it = this->begin();
    while (it != this->end()){
        if ((*it)->getValue() == value){
            break;
        }
    }
    return it;
}

iterator XMLTree::add(const std::string& name, double value, iterator parent) {
    weak_rootbase_ptr parent_ptr = *parent;
    shared_rootbase_ptr ptr = std::make_shared<RootBase>(name, value, parent_ptr);
    iterator addingIt;
    try{
        (*parent)->addChild(ptr);
        addingIt = MyTree.insert(parent, ptr);
    }
    catch(...){
        std::cout<<"adding error"<<std::endl;
    }
    return addingIt;
}

XMLTree::~XMLTree() {
    MyTree.clear();
}

iterator XMLTree::begin() {
    return MyTree.begin();
}

iterator XMLTree::end() {
    return MyTree.end();
}

