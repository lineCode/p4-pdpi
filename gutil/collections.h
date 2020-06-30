// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef PDPI_UTILS_COLLECTIONS_H
#define PDPI_UTILS_COLLECTIONS_H

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"
#include "gutil/status.h"

namespace gutil {

// Returns a const copy of the value associated with a given key if it exists,
// or a status failure if it does not.
//
// WARNING: prefer FindOrNull if the value can be large to avoid the copy.
template <typename M>
StatusOr<const typename M::mapped_type> FindOrStatus(
    const M &m, const typename M::key_type &k) {
  auto it = m.find(k);
  if (it != m.end()) return it->second;
  return absl::NotFoundError("Key not found.");
}

// Returns a non-const non-null pointer of the value associated with a given key
// if it exists, or a status failure if it does not.
template <typename M>
StatusOr<typename M::mapped_type *> FindPtrOrStatus(
    M &m, const typename M::key_type &k) {
  auto it = m.find(k);
  if (it != m.end()) return &it->second;
  return absl::NotFoundError("Key not found.");
}

// Returns a const pointer of the value associated with a given key if it
// exists, or a nullptr if it does not.
template <typename M>
const typename M::mapped_type *FindOrNull(const M &m,
                                          const typename M::key_type &k) {
  const auto it = m.find(k);
  if (it != m.end()) return &(it->second);
  return nullptr;
}

// Returns a non-const pointer of the value associated with a given key if it
// exists, or a nullptr if it does not.
template <typename M>
typename M::mapped_type *FindOrNull(M &m, const typename M::key_type &k) {
  auto it = m.find(k);
  if (it != m.end()) return &(it->second);
  return nullptr;
}

// Returns a reference of the value associated with a given key if it exists,
// crashes if it does not.
template <typename M>
typename M::mapped_type &FindOrDie(const M &map,
                                   const typename M::key_type &key) {
  auto iter = map.find(key);
  CHECK(iter != map.end()) << "Could not find key.";
  return iter->second;
}

// Checks if the id is unique in set.
template <typename M>
absl::Status InsertIfUnique(absl::flat_hash_set<M> &set, const M &id,
                            const std::string &error_message) {
  const auto it = set.insert(id);
  if (!it.second) {
    return absl::Status(absl::StatusCode::kInvalidArgument, error_message);
  }

  return absl::OkStatus();
}

template <typename K, typename V>
absl::Status InsertIfUnique(absl::flat_hash_map<K, V> &map, K key, const V &val,
                            const std::string &error_message) {
  auto it = map.insert({key, val});
  if (!it.second) {
    return absl::Status(absl::StatusCode::kInvalidArgument, error_message);
  }

  return absl::OkStatus();
}

template <typename K, typename V>
absl::Status InsertIfUnique(google::protobuf::Map<K, V> *map, K key,
                            const V &val, const std::string &error_message) {
  auto it = map->insert({key, val});
  if (!it.second) {
    return absl::Status(absl::StatusCode::kInvalidArgument, error_message);
  }

  return absl::OkStatus();
}

// Returns map[key] if key exists in map.
template <typename K, typename V>
StatusOr<V> FindElement(const absl::flat_hash_map<K, V> &map, K key,
                        const std::string &error_message) {
  auto it = map.find(key);
  if (it == map.end()) {
    return absl::Status(absl::StatusCode::kInvalidArgument, error_message);
  }
  return it->second;
}

template <typename K, typename V>
StatusOr<V> FindElement(const google::protobuf::Map<K, V> &map, K key,
                        const std::string &error_message) {
  auto it = map.find(key);
  if (it == map.end()) {
    return absl::Status(absl::StatusCode::kInvalidArgument, error_message);
  }
  return it->second;
}

}  // namespace gutil

#endif  // PDPI_UTILS_COLLECTIONS_H
