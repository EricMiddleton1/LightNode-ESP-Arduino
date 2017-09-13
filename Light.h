#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h"

class Light {
public:
  enum class Type {
    Analog = 0,
    Digital,
    Matrix
  };

  Light(Type type, int count);

  Type getType() const;
  int getCount() const;
  
  virtual void set(const Color[]) = 0;
  virtual void setAll(Color) = 0;
  
  
protected:
  Type type;
  int count;
};


#endif
