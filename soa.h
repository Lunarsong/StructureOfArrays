#pragma once

#include <vector>

// This class makes heavy use of template parameter pack.
// http://en.cppreference.com/w/cpp/language/parameter_pack

// Structure of Arrays.
template <typename... Elements>
class SoA {
 public:
  SoA() {
    const size_t num_types = sizeof...(Elements);
    arrays_.resize(num_types);

    // Sizeof each element. Dummy does pack expansion via list-initialization.
    int dummy[] = {(sizeofs_.push_back(sizeof(Elements)), 0)...};
    (void)dummy;  // avoids unused variable compiler warnings.
  }

  virtual ~SoA() = default;

  // Returns the number of elements in the arrays.
  size_t size() const { return size_; }

  // Returns |true| if there are no elements in the arrays, |false| otherwise.
  bool empty() const { return (size_ == 0); }

  // Adds an element to the end of the arrays.
  void push_back(Elements... elements) {
    size_t index = 0;
    int dummy[] = {(push_back(elements, index), 0)...};
    (void)dummy;

    // Increment number of elements in the arrays.
    ++size_;
  }

  // Erases the last element in the arrays.
  void pop_back() {
    if (!empty()) {
      erase(size_ - 1);
    }
  }

  // Erases elements of a given index from the arrays.
  void erase(size_t element_index) {
    const size_t num_arrays = arrays_.size();
    for (size_t array = 0; array < num_arrays; ++array) {
      const size_t data_type_size = sizeofs_[array];
      arrays_[array].erase(
          arrays_[array].begin() + data_type_size * element_index,
          arrays_[array].begin() + data_type_size * (element_index + 1));
    }

    // Decrement number of elements in the arrays.
    --size_;
  }

  // Erases |num_elements| elements from |element_index| from the arrays.
  void erase(size_t element_index, size_t num_elements) {
    const size_t num_arrays = arrays_.size();
    for (size_t array = 0; array < num_arrays; ++array) {
      const size_t data_type_size = sizeofs_[array];
      arrays_[array].erase(
          arrays_[array].begin() + data_type_size * element_index,
          arrays_[array].begin() +
              data_type_size * (element_index + num_elements));
    }

    // Decrement number of elements in the arrays.
    size_ -= num_elements;
  }

  // Swaps two elements inside the array.
  void swap(size_t index0, size_t index1) {
    const size_t num_arrays = arrays_.size();
    for (size_t array = 0; array < num_arrays; ++array) {
      const size_t data_type_size = sizeofs_[array];
      for (size_t i = 0; i < data_type_size; ++i) {
        std::swap(arrays_[array][index0 * data_type_size + i],
                  arrays_[array][index1 * data_type_size + i]);
      }
    }
  }

  // Reserves enough memories in all arrays to contain |reserve_size| elements.
  void reserve(size_t reserve_size) {
    // Reserve arrays. Dummy does pack expansion via list-initialization.
    size_t index = 0;
    int dummy[] = {
        (arrays_[index++].reserve(sizeof(Elements) * reserve_size), 0)...};
    (void)dummy;
  }

  // Resizes the arrays to contain |size| elements;
  void resize(size_t size) {
    // Resize arrays. Dummy does pack expansion via list-initialization.
    size_t index = 0;
    int dummy[] = {(arrays_[index++].resize(sizeof(Elements) * size), 0)...};
    (void)dummy;

    size_ = size;
  }

  // Returns a reference to the |index|th element from the |ArrayIndex|th array
  // as type |ElementType|.
  template <typename ElementType, std::size_t ArrayIndex>
  ElementType& get(size_t index) {
    ElementType* element_pointer = reinterpret_cast<ElementType*>(
        &(arrays_[ArrayIndex][sizeofs_[ArrayIndex] * index]));

    return *element_pointer;
  }

  // Returns a const reference to the |index|th element from the |ArrayIndex|th
  // array as type |ElementType|.
  template <typename ElementType, std::size_t ArrayIndex>
  const ElementType& get(size_t index) const {
    const ElementType* element_pointer = reinterpret_cast<const ElementType*>(
        &(arrays_[ArrayIndex][sizeofs_[ArrayIndex] * index]));

    return *element_pointer;
  }

  // Returns the number of arrays.
  size_t num_arrays() const { return arrays_.size(); }

 private:
  size_t size_ = 0;
  std::vector<std::vector<char>> arrays_;
  std::vector<size_t> sizeofs_;

  template <class Type>
  void push_back(const Type& element, size_t& index) {
    arrays_[index].resize(arrays_[index].size() + sizeof(Type));

    Type* array_data_pointer =
        reinterpret_cast<Type*>(&arrays_[index][size_ * sizeof(Type)]);
    *array_data_pointer = element;

    ++index;
  }
};
