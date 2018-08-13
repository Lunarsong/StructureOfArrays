#pragma once

#include <unordered_map>

#include <iostream>
#include "soa.h"

template <typename KeyType, typename... Elements>
class MappedSoA {
 public:
  MappedSoA() {}
  ~MappedSoA() = default;

  void add(const KeyType& key) {
    if (has(key)) {
      assert(false);

      return;
    }

    const size_t index = index_to_key_.size();
    index_to_key_.push_back(key);
    soa_.resize(soa_.size() + 1);
    key_to_index_[key] = index;
  }

  void add(const KeyType& key, const Elements&... elements) {
    if (has(key)) {
      assert(false);

      return;
    }

    const size_t index = index_to_key_.size();
    index_to_key_.push_back(key);
    soa_.push_back(elements...);
    key_to_index_[key] = index;
  }

  void add(const KeyType& key, Elements&&... elements) {
    if (has(key)) {
      assert(false);

      return;
    }

    const size_t index = index_to_key_.size();
    index_to_key_.push_back(key);
    soa_.push(std::forward<Elements>(elements)...);
    key_to_index_[key] = index;
  }

  void set(const KeyType& key, Elements&&... elements) {
    if (has(key)) {
      const size_t index = get_index(key);
      soa_.replace(index, std::forward<Elements>(elements)...);
    } else {
      add(key, std::forward<Elements>(elements)...);
    }
  }

  void set(const KeyType& key, const Elements&... elements) {
    if (has(key)) {
      const size_t index = get_index(key);
      soa_.replace(index, elements...);
    } else {
      add(key, elements...);
    }
  }

  void remove(const KeyType& key) {
    const auto it = key_to_index_.find(key);
    if (it == key_to_index_.end()) {
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

  void copy(size_t index_from, size_t index_to) {
    soa_.copy(index_from, index_to);
  }

  void swap(size_t index0, size_t index1) {
    const size_t size_val = size();
    if (size_val <= index0 || size_val <= index1) {
      assert(false);

      return;
    }

    if (index0 == index1) {
      return;
    }

    const KeyType key0 = index_to_key_[index0];
    const KeyType key1 = index_to_key_[index1];

    using std::swap;
    swap(key_to_index_[key0], key_to_index_[key1]);
    swap(index_to_key_[index0], index_to_key_[index1]);

    soa_.swap(index0, index1);
  }

  void reserve(size_t reserve_size) { soa_.reserve(reserve_size); }

  void resize(size_t size) { soa_.resize(size); }

  size_t size() const { return soa_.size(); }

  bool empty() const { return soa_.empty(); }

  const KeyType* get_keys() const { return index_to_key_.data(); }

  // Returns a pointer to the |ArrayIndex|th array.
  template <std::size_t ArrayIndex>
  auto* array() {
    return soa_.template array<ArrayIndex>();
  }

  // Returns a const pointer to the |ArrayIndex|th array.
  template <std::size_t ArrayIndex>
  const auto* array() const {
    return soa_.template array<ArrayIndex>();
  }

  template <std::size_t ArrayIndex>
  auto& get(const KeyType& key) {
    return soa_.template get<ArrayIndex>(get_index(key));
  }

  template <std::size_t ArrayIndex>
  const auto& get(const KeyType& key) const {
    return soa_.template get<ArrayIndex>(get_index(key));
  }

  template <std::size_t ArrayIndex>
  auto& at(const size_t& index) {
    return soa_.template get<ArrayIndex>(index);
  }

  template <std::size_t ArrayIndex>
  const auto& at(const size_t& index) const {
    return soa_.template get<ArrayIndex>(index);
  }

  size_t get_index(const KeyType& key) const {
    const auto it = key_to_index_.find(key);
    if (it == key_to_index_.end()) {
      assert(false);

      return -1;
    }

    return it->second;
  }

  bool has(const KeyType& key) const { return (key_to_index_.count(key) != 0); }

 private:
  using KeyToIndexMap = std::unordered_map<KeyType, size_t>;
  using IndexToKey = std::vector<KeyType>;

  SoA<Elements...> soa_;
  KeyToIndexMap key_to_index_;
  IndexToKey index_to_key_;
};
