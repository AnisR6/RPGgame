#pragma once
#include <unordered_map>

template <typename T>
class Database {
  private:
	std::unordered_map<unsigned, T> entries;

  public:
	void add(unsigned id, T entry);
	const T *get(unsigned id) const;
	bool exists(unsigned id) const;
};