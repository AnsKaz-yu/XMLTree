#include <iostream>
#include "XMLTree.h"
#include <memory>
int main() {
    unique_xmltree_ptr ptr = XMLTree::create(R"(C:\Users\Lenovo\CLionProjects\Lab2_XMLTree\input.txt)");
    auto v3 = ptr->find("v3");
    shared_rootbase_ptr v3_ptr = *v3;
    ptr->add("new root", 100, v3);
    ptr->erase(v3);
    ptr->save(R"(C:\Users\Lenovo\CLionProjects\Lab2_XMLTree\output.txt)");
    return 0;
}
