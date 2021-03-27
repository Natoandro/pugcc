#ifndef _LIBPUGCC_UTILS_RECORD_H_
#define _LIBPUGCC_UTILS_RECORD_H_

#include <string>
#include <utility>
#include <vector>

#include "algo.h"

namespace pug::utils {

  template <typename ValueType>
  struct RecordEntry {
    using Value = ValueType;

    const std::string name;
    ValueType value;

    template <typename... Args>
    RecordEntry(std::string _name, Args&&... args):
      name{ _name }, value{ std::forward<Args>(args)... }
    {}
  };

  template <typename ValueType>
  class Record {
  public:
    using Value = ValueType;
    using Entry = RecordEntry<Value>;
    using Vector = std::vector<Entry>;
    using Iterator = typename Vector::iterator;
    using ConstIterator = typename Vector::const_iterator;

  private:
    class MatchEntryName {
    public:
      MatchEntryName(std::string_view name): name_{ name } {}
      bool operator()(const Entry& value) const { return value.name == name_; }

    private:
      const std::string_view name_;
    };

  public:
    void Reserve(std::size_t count) { vector_.reserve(count); }

    /* insert or update - return true if insertion */
    bool Set(Entry field) { return Set(field.name, std::move(field.value)); }

    /* insert or update - return true if insertion */
    bool Set(std::string_view name, Value value)
    {
      if (auto* p_val = Get(name); p_val != nullptr) {
        *p_val = std::move(value);
        return false;
      } else {
        Insert(std::string{ name }, std::move(value));
        return true;
      }
    }

    Value* Get(std::string_view name) { return GetEntryValue(GetEntry(name)); }
    const Value* Get(std::string_view name) const
    {
      return GetEntryValue(GetEntry(name));
    }

    Entry* GetEntry(std::string_view name) { return GetEntryFrom(*this, name); }

    const Entry* GetEntry(std::string_view name) const
    {
      return GetEntryFrom(*this, name);
    }

    void Insert(Entry field) { vector_.push_back(std::move(field)); }

    void Insert(std::string name, Value value)
    {
      Insert({ std::move(name), std::move(value) });
    }

    auto begin() { return vector_.begin(); }
    auto end() { return vector_.end(); }
    auto begin() const { return vector_.begin(); }
    auto end() const { return vector_.end(); }

  private:
    template <typename Rec>
    static auto GetEntryFrom(Rec& record, std::string_view name)
      -> decltype(record.vector_.data())
    {
      if (auto it = record.Find(name); it != record.end()) {
        return &(*it);
      } else {
        return nullptr;
      }
    }

    // non-member??
    template <typename Entry>
    static auto GetEntryValue(Entry* entry) -> decltype(&entry->value)
    {
      return entry != nullptr ? &entry->value : nullptr;
    }

    auto Find(std::string_view name)
    {
      return utils::Find(vector_, MatchEntryName{ name });
    }

  private:
    Vector vector_{};
  };

} // namespace pug::utils

#endif // _LIBPUGCC_UTILS_RECORD_H_
