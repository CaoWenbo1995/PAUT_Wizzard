#include "PADefinition.h"

#include <vector>

#define PAUT_VECTOR_POD_IMPLEMENT(name, type) \
struct name::Impl { std::vector<type> vec; }; \
name::name() { m_pImpl = new Impl; } \
name::~name() { if(m_pImpl)delete m_pImpl; } \
name::name(const name& another) { m_pImpl = new Impl; m_pImpl->vec = another.m_pImpl->vec; } \
name::name(name&& another) { m_pImpl = another.m_pImpl; another.m_pImpl = nullptr; } \
name& name::operator=(const name& another) { m_pImpl->vec = another.m_pImpl->vec; return *this; } \
name& name::operator=(name&& another) { delete m_pImpl; m_pImpl = another.m_pImpl; another.m_pImpl = nullptr; return *this; } \
void name::clear() { m_pImpl->vec.clear(); } \
void name::push_back(const type& val) { m_pImpl->vec.push_back(val); } \
void name::resize(int num) { m_pImpl->vec.resize(num); } \
int name::size() { return (int)m_pImpl->vec.size(); } \
const int name::size() const { return (int)m_pImpl->vec.size(); } \
type& name::operator[](int index) { return m_pImpl->vec[index]; } \
const type name::operator[](int index) const { return m_pImpl->vec[index]; } \
type* name::begin() { return &(m_pImpl->vec[0]); } \
type* name::end() { return begin() + size(); }


#define PAUT_VECTOR_NOT_POD_IMPLEMENT(name, type) \
struct name::Impl { std::vector<type> vec; }; \
name::name() { m_pImpl = new Impl; } \
name::~name() { if(m_pImpl)delete m_pImpl; } \
name::name(const name& another) { m_pImpl = new Impl; m_pImpl->vec = another.m_pImpl->vec; } \
name::name(name&& another) { m_pImpl = another.m_pImpl; another.m_pImpl = nullptr; } \
name& name::operator=(const name& another) { m_pImpl->vec = another.m_pImpl->vec; return *this; } \
name& name::operator=(name&& another) { delete m_pImpl; m_pImpl = another.m_pImpl; another.m_pImpl = nullptr; return *this; } \
void name::clear() { m_pImpl->vec.clear(); } \
void name::push_back(const type& val) { m_pImpl->vec.push_back(val); } \
void name::push_back(const type&& val) { m_pImpl->vec.push_back(std::move(val)); } \
void name::resize(int num) { m_pImpl->vec.resize(num); } \
int name::size() { return (int)m_pImpl->vec.size(); } \
const int name::size() const { return (int)m_pImpl->vec.size(); } \
type& name::operator[](int index) { return m_pImpl->vec[index]; } \
const type name::operator[](int index) const { return m_pImpl->vec[index]; } \
type* name::begin() { return &(m_pImpl->vec[0]); } \
type* name::end() { return begin() + size(); }

PAUT_VECTOR_POD_IMPLEMENT(PAUTIntVector, int)
PAUT_VECTOR_POD_IMPLEMENT(PAUTDoubleVector, double)
PAUT_VECTOR_NOT_POD_IMPLEMENT(PAUTDouble2DVector, PAUTDoubleVector)
PAUT_VECTOR_NOT_POD_IMPLEMENT(PAUTElementInfoVector, ElementInfo)
PAUT_VECTOR_NOT_POD_IMPLEMENT(PAUTFocalLawInfoVector, FocalLawInfo)
PAUT_VECTOR_NOT_POD_IMPLEMENT(PAUTHardwareGateInfoVector, HardwareGateInfo)
//PAUT_VECTOR_NOT_POD_IMPLEMENT(PAUTChannelInfoVector, ChannelInfo)