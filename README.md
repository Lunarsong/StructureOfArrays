# About
This repository contains Structure Of Arrays (SoA) and Mapped Structure Of Arrays (MappedSoA).

The Structure of Arrays maintains multiple arrays of different types while maintaining the same size for all the arrays and a shared index for each attribute created throughout its lifetime.

The Mapped Structure of Arrays contains a Structure of Arrays and also defines a key type used to map to indices of attributes within the arrays.

## Structure Of Arrays
Structure of Arrays (SoA) template in C++.

The template simplifies creating structures of arrays.

### Brief explaination about structures of arrays and arrays of structures.
Structure of Arrays are often used to improve cache efficacy over arrays of structs, for example:

```
struct Particle {
  vec3 position;
  vec3 velocity;
  vec3 direction;
  float mass;
  vec3 aabb_min;
  vec3 aabb_max;
};
Particle particles[1024];
```

The code above declares 1024 particles as an array of a structure. However in many cases the properties are not used together and iteration is slowed down due to cache misses as the CPU is waiting for memory to be fetched.

To improve on that, structures of arrays are created. For example:

```
struct Particles {
  vec3 position[1024];
  vec3 velocity[1024];
  vec3 direction[1024];
  float mass[1024];
  vec3 aabb_min[1024];
  vec3 aabb_ax[1024];
};
```

If each of the particle's properties is accessed without significiant relation to other properties, the example above would be correct. In this case iterating a single property is faster as the rest of the data does take memory in the cache line.

However this often results in a lot of boilterplate code needed in order to manipulate specific elements in the arrays (this is mostly true *if* you require insertion, removal or specific referencing of elements).

### The SoA template.
The SoA template simplifies those operations. Declare elements as such:

```
SoA<vec3, vec3, vec3, float, vec3, vec3> particles;
```

#### Reserve / Resize

```
// Reserve:
particles.reserve(1024);

// Resize:
particles.resize(1024);
```

#### Adding elements

```
particles.push_back(...);
particles.push_back(...);
particles.push_back(...);
particles.push_back(...);
```

#### Accessing elements

```
std::cout << "2nd position is: " << particles.get<0>(2) << std::endl;
```

#### Elements iteration

```
for (int i = 0; i < particles.size(); ++i) {
  std::cout << "Position index: " << i << " is: " << particles.get<0>(i) << std::endl;
  std::cout << "Velocity index: " << i << " is: " << particles.get<1>(i) << std::endl;
}
```

#### Swap elements

```
particles.swap(0, 1);
```

#### Pop back / Erase

Remove end element:

```
particles.pop_back();
```

Erase 2nd element:

```
particles.erase(1);
```

Erase first two elements:

```
particles.erase(0, 2);
```

#### Accessing arrays
Sometimes you may want to access the raw arrays. To do so, use the *array* function.

```
vec3* positions = particles.array<0>();
const vec3* velocities = particles.array<1>();

for (int i = 0; i < particles.size(); ++i) {
  positions[i] += velocities[i] * delta_time;
}
```

## Mapped Structure Of Arrays
A mapped Structure of Arrays (MappedSoA) template in C++.

This class allows you maintain structure of arrays with values referenced by a key.

This is useful for all attributes of an object, such as component data belonging to a game entity, while maintaining the different attributes tightly packed in structure of arrays.

For example:

```
// Some Physics structure.
using Entity = unsigned int;
using PositionType = vec3;
using VelocityType = vec3;
using MassType = float;

MappedSoA<Entity, PositionType, VelocityType, MassType> physics;
physics.add(entity, position, velocity, mass);
// Add more physics entities...

// Remove a specific physics entity.
physics.remove(entity);
```

_Note that the map is not guaranteed to maintain the same order in the arrays after calls to remove and pop. Also all operations that modify the array may invalidate the memory address returned by the array() functions._
