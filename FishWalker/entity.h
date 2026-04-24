#ifndef ENTITY_H
#define ENTITY_H

#include <string>

class Entity {
 public:
  virtual ~Entity();

  Entity(const std::string& name, int id);

  virtual void update() = 0;

  int getId() const;
  const std::string& getName() const;

 protected:
  std::string name;
  int id;
};

#endif
