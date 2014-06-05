/*
    colortail -- output last part of file(s) in color.
    Copyright (C) 2009  Joakim Andersson <ja@joakimandersson.se>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "Colorizer.h"
#include "CfgFileParser.h"
#include "TailFile.h"
#include "multicoloured_string.hpp"

#include <assert.h>
#include <iostream>
#include <string.h>

using namespace std;

Colorizer::Colorizer()
{
   // default constructor
   
   m_items_list = NULL;
}

void Colorizer::free_items()
{
   // frees the memory that the items uses

   // check if there is a list
   if (m_items_list)
   {
      SearchData *tmp;

      // go through all the elements in the list,
      // and free each SearchData instance
      while (!m_items_list->is_empty())
      {
         tmp = m_items_list->first_element();
         m_items_list->remove_first();
         delete tmp;
      }

      // delete the list
      delete m_items_list;
      m_items_list = NULL;
   }
}


Colorizer::Colorizer(const char *cfg_file)
{
   // other constructor
   
   // sends the cfg_file to the CfgFileParser
   // gets the items_list from the CfgFileParser and saves it

   // check that cfg_file isn't NULL
   assert (cfg_file != NULL);

   CfgFileParser parser;

   // parse the cfg file
   int n = parser.parse(cfg_file);

   // get the items list
   m_items_list = parser.get_items_list();
}

Colorizer::~Colorizer()
{
   // destructor

   // free the memory the search data items use
   free_items();
}

string Colorizer::colorize(const char *str)
{
		// colorize the string, returns a new string containing
		// the colorized version of str
		// RETURN: new string with result

		// check that a list exists
	assert (m_items_list != NULL);

	multicoloured_string_t val(str);
		
	regmatch_t pmatch[10];
	int found = 0, submatch = 0, j;

	// make an iterator
	ListIterator<SearchData*> itr(*m_items_list);

	// will contain the new string
	ostringstream newstr;
	
	SearchData *current;
	int i = 0;
	// go through all the elements in the list
	for (itr.init() ; !itr ; ++itr, i++)
	{
		 current = itr();

		 // check for match
		 if (regexec(current->m_preg, val.c_str(), 10, pmatch, 0) == 0)
		 {
	 // TODO: check for callback function
	 // TODO: call callback function
	 
			found = 1;

		 // Check to see if this is a substring match
			 if (pmatch[1].rm_so != -1)
			 {
					submatch = 1;
					// Note: start at offset 1 to get only submatches

					for (j = 1 ; pmatch[j].rm_so != -1 && j <= 10 ; j++)
					{
						val += region(
							val.str().begin() + pmatch[j].rm_so, 
							val.str().begin() + pmatch[j].rm_eo, 
							current->m_ansi_color_code
						);
					}
			 }
			 else
			 {
					// not a substring match, colorize whole string.
					// stop looking for other matches as well

					submatch = 0;
					val += region(val.str().begin(), val.str().end(), current->m_ansi_color_code);

			 }
		 }
	}

	newstr << val;
	return newstr.str();

}
