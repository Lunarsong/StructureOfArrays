# StructureOfArrays
Structure of Arrays (SoA) template in C++.

# Description
The template simplifies creating structures of arrays.

## Brief explaination about struectres of arrays and arrays of structures.
Structure of Arrays are often used to improve cache efficacy over arrays of structs, for example:

```
struct Particle {
  vec3 position;
  vec3 velocity;
  vec3 direction;
  float mass;
  vec3 aabb_min;
  vec3 aabb_ax;
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

## The SoA template.
The SoA template simplifies those operations. Declare elements as such:

```
SoA<vec3, vec3, vec3, float, vec3, vec3> particles;
```

### Reserve / Resize

```
// Reserve:
particles.reserve(1024);

// Resize:
particles.resize(1024);
```

### Adding elements

```
particles.push_back(...);
particles.push_back(...);
particles.push_back(...);
particles.push_back(...);
```

### Accessing elements

```
std::cout << "2nd position is: " << particles.Get<vec3, 0>(2) << std::endl;
```

### Elements iteration

```
for (int i = 0; i < particles.size(); ++i) {
  std::cout << "Position index: " << i << " is: " << particles.Get<vec3, 0>(i) << std::endl;
  std::cout << "Velocity index: " << i << " is: " << particles.Get<vec3, 1>(i) << std::endl;
}
```

### Swap elements

```
particles.swap(0, 1);
```

### Pop back / Erase

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

### Accessing arrays
Sometimes you may want to access the raw arrays. To do so, use the *array* function.

```
vec3* positions = particles.array<vec3, 0>();
vec3* velocities = particles.array<vec3, 1>();

for (int i = 0; i < particles.size(); ++i) {
  positions[i] += velocities[i] * delta_time;
}
```
