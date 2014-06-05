#ifndef COLORTAIL_MULTICOLOURED_STRING_HPP
#define COLORTAIL_MULTICOLOURED_STRING_HPP
#pragma once

#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class multicoloured_string_t;
struct coloured_region_t;

typedef coloured_region_t colour_region_t;
typedef vector<colour_region_t> colour_region_list_t;

struct coloured_region_t {
	string::const_iterator begin;
	string::const_iterator end;
	string colour;

	bool operator<(coloured_region_t const& rhs) const
		{ return begin < rhs.begin || (begin == rhs.begin && end < rhs.end); }

	bool operator==(coloured_region_t const& rhs) const
		{ return begin == rhs.begin; }
};

class multicoloured_string_t {
public:
	multicoloured_string_t() { }
	multicoloured_string_t(string const& str) : str_(str) { }
	multicoloured_string_t(char const* str) : str_(str) { }

	template<typename Itr>
	multicoloured_string_t(string const& str, Itr col_begin, Itr col_end)
		: str_(str), regions_(col_end - col_begin)
		{ copy(col_begin, col_end, regions_.begin()); }

	template<typename Itr>
	multicoloured_string_t(char const* str, Itr col_begin, Itr col_end)
		: str_(str), regions_(col_end - col_begin)
		{ copy(col_begin, col_end, regions_.begin()); }

	multicoloured_string_t(multicoloured_string_t const& oth)
		: str_(oth.str_), regions_(oth.regions_) { }

	~multicoloured_string_t() { }

	friend void swap(multicoloured_string_t& lhs, multicoloured_string_t& rhs) {
		using std::swap;
		swap(lhs.str_, rhs.str_);
		swap(lhs.regions_, rhs.regions_);
	}

	multicoloured_string_t& operator=(multicoloured_string_t rhs) 
		{ using std::swap; swap(*this, rhs); return *this; }

	bool operator==(multicoloured_string_t const& rhs) const;

	bool operator==(string const& rhs) const
		{ return str_ == rhs; }

	bool operator==(char const* rhs) const 
		{ return str_ == rhs; }

	bool operator!=(multicoloured_string_t const& rhs) const;

	bool operator!=(string const& rhs) const
		{ return !(*this == rhs); }

	bool operator!=(char const* rhs) const 
		{ return !(*this == rhs); }

	bool operator<(multicoloured_string_t const& rhs) const
		{ return str_ < rhs.str_; }

	operator char const*() const
		{ return str_.c_str(); }

	string::const_iterator begin() const
		{ return str_.begin(); }

	string::const_iterator end() const
		{ return str_.end(); }

	colour_region_list_t const& regions() const 
		{ return regions_; }

	string const& str() const
		{ return str_; }

	char const* c_str() const 
		{ return str_.c_str(); }

	multicoloured_string_t& reset(string const& val);
	multicoloured_string_t& reset(char const* val);

	multicoloured_string_t& operator+=(colour_region_t const& rhs)
		{
			colour_region_list_t::iterator i = lower_bound(regions_.begin(), regions_.end(), rhs);
			regions_.insert(i, rhs); 
			return *this; 
		}

	multicoloured_string_t operator+(colour_region_t const& rhs)
		{ return *this + rhs; }

public:
private:
	string	str_;
	colour_region_list_t regions_;
};

//bool operator==(string::const_iterator const& lhs, colour_region_t const& rhs) {
//	return rhs.begin() == lhs;
//}

colour_region_list_t::const_iterator
find_colour(
	colour_region_list_t::const_iterator first, 
	colour_region_list_t::const_iterator last, 
	string::const_iterator val ) {

	colour_region_list_t::const_iterator i = first;
	colour_region_list_t::const_iterator v = last;
	for(; i != last; ++i) {
		if( distance(val, i->end) <= distance(i->begin, i->end) ) {
			v = i;
		}
	}

	return v;
}

ostream& operator<<(ostream& os, multicoloured_string_t const& val) {

	//	If we've an empty regions list
	//	we can just blast the string to the
	//	output stream and return
	if(val.regions().begin() == val.regions().end()) {
		return os << val.str();
	}

	string last_colour;

	for(
		string::const_iterator i = val.str().begin();
		i != val.str().end();
		++i) {

		colour_region_list_t::const_iterator col_test = 
			find_colour(val.regions().begin(), val.regions().end(), i);

		if(col_test != val.regions().end() && col_test->colour != last_colour) {
			os << col_test->colour;
			last_colour = col_test->colour;
		}
		else if(col_test == val.regions().end()) {
			os << ANSI_RESET_STR;
			last_colour = ANSI_RESET_STR;
		}

		os.put(*i);
	}

	if(last_colour != ANSI_RESET_STR) {
		os << ANSI_RESET_STR; 
	}
	
	return os;
}

coloured_region_t
region(string::const_iterator start, string::const_iterator end, string const& col) {
	coloured_region_t v = { start, end, col };
	return v;
}

#endif //COLORTAIL_MULTICOLOURED_STRING_HPP
