#pragma once

#include <unordered_map>

#include "soa.h"

template <typename KeyType, typename... Elements> class MappedSoA {
public:
  MappedSoA() {}
  ~MappedSoA() = default;

  void add(const KeyType &key) {
    if (has(key)) {
      assert(false);

      return;
    }

    const size_t index = index_to_key_.size();
    index_to_key_.push_back(key);
    soa_.resize(soa_.size() + 1);
    key_to_index_[key] = index;
  }

  void add(const KeyType &key, const Elements &... elements) {
    if (has(key)) {
      assert(false);

      return;
    }

    const size_t index = index_to_key_.size();
    index_to_key_.push_back(key);
    soa_.push_back(elements...);
    key_to_index_[key] = index;
  }

  void add(const KeyType &key, Elements &&... elements) {
    if (has(key)) {
      assert(false);

      return;
    }

    const size_t index = index_to_key_.size();
    index_to_key_.push_back(key);
    soa_.push_back(std::move(elements...));
    key_to_index_[key] = index;
  }

  void remove(const KeyType &key) {
    const auto it = key_to_index_.find(key);
    if (it == key_to_index_.end()) {
      assert(false);

      return;
    }

    const size_t index = key_to_index_[key];
    const size_t end_index = index_to_key_.size() - 1;
    if (index < end_index) {
      swap(index, end_index);
    }

    index_to_key_.pop_back();
    soa_.pop_back();
    key_to_index_.erase(key);
  }

  void swap(size_t index0, size_t index1) {
    const size_t size_val = size();
    if (size_val <= index0 || size_val <= index1) {
      assert(false);

      return;
    }

    const KeyType key0 = index_to_key_[index0];
    const KeyType key1 = index_to_key_[index1];

    key_to_index_[key0] = index1;
    key_to_index_[key1] = index0;

    index_to_key_[index0] = key1;
    index_to_key_[index1] = key0;

    soa_.swap(index0, index1);
  }

  void reserve(size_t reserve_size) { soa_.reserve(reserve_size); }

  void resize(size_t size) { soa_.resize(size); }

  size_t size() const { return soa_.size(); }

  // Returns a pointer to the |ArrayIndex|th array.
  template <typename ElementType, std::size_t ArrayIndex> ElementType *array() {
    return soa_.template array<ElementType, ArrayIndex>();
  }

  // Returns a const pointer to the |ArrayIndex|th array.
  template <typename ElementType, std::size_t ArrayIndex>
  const ElementType *array() const {
    return soa_.template array<ElementType, ArrayIndex>();
  }

  template <typename ElementType, std::size_t ArrayIndex>
  ElementType &get(const KeyType &key) {
    return soa_.template get<ElementType, ArrayIndex>(get_index(key));
  }

  template <typename ElementType, std::size_t ArrayIndex>
  const ElementType &get(const KeyType &key) const {
    return soa_.template get<ElementType, ArrayIndex>(get_index(key));
  }

  size_t get_index(const KeyType &key) const {
    const auto it = key_to_index_.find(key);
    if (it == key_to_index_.end()) {
      assert(true);

      return -1;
    }

    return it->second;
  }

  bool has(const KeyType &key) const {
    const auto it = key_to_index_.find(key);
    if (it == key_to_index_.end()) {
      return false;
    }

    return true;
  }

private:
  using KeyToIndexMap = std::unordered_map<KeyType, size_t>;
  using IndexToKey = std::vector<KeyType>;

  SoA<Elements...> soa_;
  KeyToIndexMap key_to_index_;
  IndexToKey index_to_key_;
};
