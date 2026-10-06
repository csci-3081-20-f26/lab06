#include "Node.h"

void NodeComponent::remove() {
    getNode().removeComponent(*this);
}