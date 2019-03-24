#pragma once

#define PAUT_VECTOR_POD_DECLARE(name, type) \
class name \
{ \
public: \
	void clear(); \
	void push_back(const type& val); \
	void resize(int num); \
	int size(); \
	const int size() const; \
	type& operator[](int index); \
	const type operator[](int index) const; \
	type* begin(); \
	type* end(); \
	name(); \
	~name(); \
	name(const name& another); \
	name(name&& another); \
	name& operator=(const name& another); \
	name& operator=(name&& another); \
private: \
	struct Impl; \
	Impl* m_pImpl; \
};

#define PAUT_VECTOR_NOT_POD_DECLARE(name, type) \
class name \
{ \
public: \
	void clear(); \
	void push_back(const type& val); \
	void push_back(const type&& val); \
	void resize(int num); \
	int size(); \
	const int size() const; \
	type& operator[](int index); \
	const type operator[](int index) const; \
	type* begin(); \
	type* end(); \
	name(); \
	~name(); \
	name(const name& another); \
	name(name&& another); \
	name& operator=(const name& another); \
	name& operator=(name&& another); \
private: \
	struct Impl; \
	Impl* m_pImpl; \
};