#pragma once

class Node
{
public:
    std::string name;
    Matrix localTransform;  
    Matrix globalTransform;    
    Node* parent;           
    std::vector<Node*> children; 
    std::vector<unsigned int> meshIdxes;

public:
    Node(const std::string& name, const Matrix& transform)
        : name(name), localTransform(transform), parent(nullptr) {
        globalTransform = transform;
    }

    void UpdateTransform()
    {
        if (parent)
            globalTransform = localTransform * parent->globalTransform;
        else
            globalTransform = localTransform;
        for (Node* child : children)
          child->UpdateTransform();
    }

    Node* FindNode(std::string name)
    {
        if (name == this->name)
            return this;

        for (auto child : children)
        {
            Node* result = child->FindNode(name); 
            if (result != nullptr)
                return result; 
        }
        return nullptr; 
    }

    void AddChild(Node* child)
    {
        children.push_back(child);
        child->parent = this;
    }

};

