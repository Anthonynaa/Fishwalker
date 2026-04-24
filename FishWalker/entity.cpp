#include "entity.h"

Entity::Entity(const std::string& name, int id) : id(id), name(name) {}

int Entity::getId() const { return id; }
const std::string& Entity::getName() const { return name; }

Entity::~Entity() {}
