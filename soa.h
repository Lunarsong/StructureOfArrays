#pragma once

#include <vector>

// This class makes heavy use of template parameter pack.
// http://en.cppreference.com/w/cpp/language/parameter_pack

// Structure of Arrays.
template <typename... Elements>
class SoA {
 public:
  SoA() {
    size_t array_index = 0;
    int dummy_init[] = {
        (arrays_[array_index++] =
             reinterpret_cast<std::vector<char> *>(new std::vector<Elements>()),
         0)...};
    (void)dummy_init;  // avoids unused variable compiler warnings.
  }

  virtual ~SoA() {
    size_t array_index = 0;
    int dummy[] = {(delete reinterpret_cast<std::vector<Elements> *>(
                        arrays_[array_index++]),
                    0)...};
    (void)dummy;
  }

  // Returns the number of elements in the arrays.
  size_t size() const { return size_; }

  // Returns |true| if there are no elements in the arrays, |false| otherwise.
  bool empty() const { return (size_ == 0); }

  // Adds an element to the end of the arrays via rvalue.
  void push_back(Elements &&... elements) {
    size_t array_index = 0;
    int dummy[] = {
        (push_back_impl(std::forward<Elements>(elements), array_index), 0)...};
    (void)dummy;

    // Increment number of elements in the arrays.
    ++size_;
  }

  // Adds an element to the end of the arrays, as a copy of the const ref value.
  void push_back(const Elements &... elements) {
    size_t array_index = 0;
    int dummy[] = {(push_back_impl(elements, array_index), 0)...};
    (void)dummy;

    // Increment number of elements in the arrays.
    ++size_;
  }

  // Erases the last element in the arrays.
  void pop_back() {
    size_t array_index = 0;
    int dummy[] = {(pop_back_impl<Elements>(array_index), 0)...};
    (void)dummy;

    // Decrement number of elements in the arrays.
    --size_;
  }

  // Erases elements of a given index from the arrays.
  void erase(size_t element_index) {
    size_t array_index = 0;
    int dummy[] = {(erase_impl<Elements>(element_index, array_index), 0)...};
    (void)dummy;

    // Decrement number of elements in the arrays.
    --size_;
  }

  // Erases |num_elements| elements from |start_index| from the arrays.
  void erase(size_t start_index, size_t num_elements) {
    size_t array_index = 0;
    int dummy[] = {
        (erase_impl<Elements>(start_index, num_elements, array_index), 0)...};
    (void)dummy;

    // Decrement number of elements in the arrays.
    size_ -= num_elements;
  }

  // Swaps two elements inside the array.
  void swap(size_t index0, size_t index1) {
    size_t array_index = 0;
    int dummy[] = {(swap_impl<Elements>(index0, index1, array_index), 0)...};
    (void)dummy;
  }

  // Reserves enough memories in all arrays to contain |reserve_size| elements.
  void reserve(size_t reserve_size) {
    // Reserve arrays. Dummy does pack expansion via list-initialization.
    size_t array_index = 0;
    int dummy[] = {(reserve_impl<Elements>(reserve_size, array_index), 0)...};
    (void)dummy;
  }

  // Resizes the arrays to contain |size| elements;
  void resize(size_t size) {
    size_t array_index = 0;
    int dummy[] = {(resize_impl<Elements>(size, array_index), 0)...};
    (void)dummy;

    size_ = size;
  }

  // Returns a pointer to the |ArrayIndex|th array.
  template <typename ElementType, std::size_t ArrayIndex>
  ElementType *array() {
    std::vector<ElementType> *array =
        reinterpret_cast<std::vector<ElementType> *>(arrays_[ArrayIndex]);

    return array->data();
  }

  // Returns a const pointer to the |ArrayIndex|th array.
  template <typename ElementType, std::size_t ArrayIndex>
  const ElementType *array() const {
    std::vector<ElementType> *array =
        reinterpret_cast<std::vector<ElementType> *>(arrays_[ArrayIndex]);

    return array->data();
  }

  // Returns a reference to the |index|th element from the |ArrayIndex|th array
  // as type |ElementType|.
  template <typename ElementType, std::size_t ArrayIndex>
  ElementType &get(size_t index) {
    std::vector<ElementType> *array =
        reinterpret_cast<std::vector<ElementType> *>(arrays_[ArrayIndex]);

    return (*array)[index];
  }

  // Returns a const reference to the |index|th element from the |ArrayIndex|th
  // array as type |ElementType|.
  template <typename ElementType, std::size_t ArrayIndex>
  const ElementType &get(size_t index) const {
    std::vector<ElementType> *array =
        reinterpret_cast<std::vector<ElementType> *>(arrays_[ArrayIndex]);

    return (*array)[index];
  }

  // Returns the number of arrays.
  size_t num_arrays() const { return sizeof...(Elements); }

 private:
  size_t size_ = 0;
  std::vector<char> *arrays_[sizeof...(Elements)];

  template <class Type>
  void push_back_impl(Type &&element, size_t &index) {
    std::vector<Type> *array =
        reinterpret_cast<std::vector<Type> *>(arrays_[index]);

    array->push_back(std::forward<Type>(element));

    ++index;
  }

  template <class Type>
  void push_back_impl(const Type &element, size_t &array_index) {
    std::vector<Type> *array =
        reinterpret_cast<std::vector<Type> *>(arrays_[array_index]);

    array->push_back(element);

    ++array_index;
  }

  template <class Type>
  void pop_back_impl(size_t &array_index) {
    std::vector<Type> *array =
        reinterpret_cast<std::vector<Type> *>(arrays_[array_index]);

    array->pop_back();

    ++array_index;
  }

  template <class Type>
  void erase_impl(size_t element_index, size_t &array_index) {
    std::vector<Type> *array =
        reinterpret_cast<std::vector<Type> *>(arrays_[array_index]);

    array->erase(array->begin() + element_index);

    ++array_index;
  }

  template <class Type>
  void erase_impl(size_t element_index, size_t num_elements,
                  size_t &array_index) {
    std::vector<Type> *array =
        reinterpret_cast<std::vector<Type> *>(arrays_[array_index]);

    array->erase(array->begin() + element_index,
                 array->begin() + element_index + num_elements);

    ++array_index;
  }

  template <class Type>
  void swap_impl(size_t index0, size_t index1, size_t &array_index) {
    std::vector<Type> *array =
        reinterpret_cast<std::vector<Type> *>(arrays_[array_index]);

    using std::swap;
    swap((*array)[index0], (*array)[index1]);

    ++array_index;
  }

  template <class Type>
  void reserve_impl(size_t size, size_t &array_index) {
    std::vector<Type> *array =
        reinterpret_cast<std::vector<Type> *>(arrays_[array_index]);

    array->reserve(size);

    ++array_index;
  }

  template <class Type>
  void resize_impl(size_t size, size_t &array_index) {
    std::vector<Type> *array =
        reinterpret_cast<std::vector<Type> *>(arrays_[array_index]);

    array->resize(size);

    ++array_index;
  }
};
