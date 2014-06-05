#include "multicoloured_string.hpp"

multicoloured_string_t&
multicoloured_string_t::reset(string const& val) {
	str_ = val;
	regions_ = colour_region_list_t();
	return *this;
}

multicoloured_string_t&
multicoloured_string_t::reset(char const* val) {
	str_ = val;
	regions_ = colour_region_list_t();
	return *this;
}

bool
multicoloured_string_t::operator==(multicoloured_string_t const& rhs) const {
	//	TODO:
	//	We really should do a like-for-like compare
	return str_ == rhs.str_;
}

bool
multicoloured_string_t::operator!=(multicoloured_string_t const& rhs) const {
	//	TODO:
	//	We really should do a like-for-like compare
	return !(*this == rhs);
}
