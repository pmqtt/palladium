#include "VMType.h"

VMStruct::VMStruct(VMPrimitive size)
    : _fields(std::get<std::size_t>(size)), _currentIndex(0) {}

auto VMStruct::max_index() const -> std::size_t { return _fields.size(); }
void VMStruct::add_field(const VMStructTypes &type) {
  _fields[_currentIndex] = type;
  _currentIndex += 1;
}
void VMStruct::set_field(std::size_t index, const VMStructTypes &value) {
  auto pp = [&](const VMPrimitive &v, const VMPrimitive &field) {
    if (field.index() == v.index()) {
      _fields[index] = v;
    } else if (field.index() < get_index<bool, VMPrimitive>::value &&
               v.index() <= field.index()) {
      _fields[index] = v;

    } else {
      // TODO Error handling
    }
  };
  auto psp = [](const VMPrimitive &v, const VMStructPtr &field) {
    UNUSED(v);
    UNUSED(field);
  };
  auto spsp = [](const VMStructPtr &v, const VMStructPtr &field) {
    UNUSED(v);
    UNUSED(field);
  };
  auto spp = [](const VMStructPtr &v, const VMPrimitive &field) {
    UNUSED(v);
    UNUSED(field);
  };

  std::visit(overloaded{pp, psp, spsp, spp}, value, _fields[index]);
}
auto VMStruct::get_field(std::size_t index) -> VMStructTypes & {
  return _fields[index];
}
