#ifndef NODE_H_
#define NODE_H_

#include <vector>
#include <memory>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Node;
class NodeComponent;

class NodeObserver {
public:
    virtual ~NodeObserver() {}
    virtual void onComponentAdded(Node& node, NodeComponent& component) {}
    virtual void onComponentRemoved(Node& node, NodeComponent& component) {}
    virtual void onChildAdded(Node& node, Node& child) {}
    virtual void onChildRemoved(Node& node, Node& child) {}
};

class NodeComponent : public NodeObserver {
friend class Node;
public:
    NodeComponent() : node(nullptr) {}
    virtual ~NodeComponent() {}
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void remove();

protected:
    virtual void setNode(Node& node) { this->node = &node; }
    virtual Node& getNode() { return *node; }

private:
    Node* node;
};

class Node {
public:
    Node() : parentToLocal(1.0f), parent(nullptr), parentToLocalChanged(true) {
    }

    void updateWorldToLocal() {
        if (parentToLocalChanged && parent) {
            worldToLocal = parent->worldToLocal*parentToLocal;
        }

        for (int i = 0; i < children.size(); i++) {
            children[i]->updateWorldToLocal();
        }
    }

    void update() {
        for (int i = 0; i < components.size(); i++) {
            components[i]->update();
        }

        for (int i = 0; i < children.size(); i++) {
            children[i]->update();
        }
    }

    void render() {
        for (int i = 0; i < components.size(); i++) {
            components[i]->render();
        }

        for (int i = 0; i < children.size(); i++) {
            children[i]->render();
        }
    }

    void setParentToLocal(glm::mat4 parentToLocal) { 
        this->parentToLocal = parentToLocal;
        parentToLocalChanged = true;
    }

    const glm::mat4& getParentToLocal() const { return parentToLocal; }
    
    const glm::mat4& getWorldToLocal() const { return worldToLocal; }

    void addComponent(NodeComponent* component) {
        component->setNode(*this);
        components.push_back(std::unique_ptr<NodeComponent>(component));
        
        for (int i = 0; i < observers.size(); i++) {
            observers[i]->onComponentAdded(*this, *(components[components.size()-1]));
        }

        observers.push_back(component);
    }

    void removeComponent(const NodeComponent& component) {
        std::vector<std::unique_ptr<NodeComponent> >::iterator it = std::remove_if(components.begin(), components.end(), 
            [&component](std::unique_ptr<NodeComponent>& c) {
      			return c.get() == &component;
            });
        components.erase(it, components.end());
    }

    void add(Node* child) {
        child->parent = this;
        children.push_back(std::unique_ptr<Node>(child));

        for (int i = 0; i < observers.size(); i++) {
            observers[i]->onChildAdded(*this, *(children[children.size()-1]));
        }
    }

    void remove(Node& child) {
        std::vector<std::unique_ptr<Node> >::iterator it = std::remove_if(children.begin(), children.end(), 
            [&child](std::unique_ptr<Node>& node) {
      			return node.get() == &child;
            });
        children.erase(it, children.end());
        /*if (it != children.end()) {
            children.erase(it);
        }*/
    }

    void addObserver(NodeObserver& observer) {
        observers.push_back(&observer);
    }

    void removeObserver(NodeObserver& observer) {
        std::vector<NodeObserver*>::iterator it = std::find(observers.begin(), observers.end(), &observer);
        if (it != observers.end()) {
            observers.erase(it);
        }

    }


    template <typename T>
    T* findComponent() {
        for (int i = 0; i < components.size(); i++) {
            T* component = dynamic_cast<T*>(components[i]);
            if (component) {
                return component;
            }
        }

        return nullptr;
    }

private:
    std::vector< std::unique_ptr<NodeComponent> > components;
    std::vector< std::unique_ptr<Node> > children;
    glm::mat4 parentToLocal;
    bool parentToLocalChanged;
    glm::mat4 worldToLocal;
    Node* parent;
    std::vector<NodeObserver*> observers;
};


#endif