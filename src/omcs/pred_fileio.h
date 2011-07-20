/*  This file is part of OMCSNetCPP

    OMCSNetCPP is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    OMCSNetCPP is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with OMCSNetCPP; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*! \file */

#ifndef _PRED_FILEIO

#define _PRED_FILEIO

#include <stdio.h>

#include <string>


//!Handles file I/O during binary file loading
class CPredicateFileIO
{
	public:
		//!Opens an input file for reading
		bool OpenInputFile(const std::string &a_path)
		{
			if (m_fp)
			{
				return false;
			}

			m_fp = fopen(a_path.c_str(), "rt");

			if (m_fp)
			{
				return true;
			}

			return false;
		}

		//!Closes the input file
		bool CloseInputFile()
		{
			if (m_fp)
			{
				fclose(m_fp);

				m_fp = NULL;

				return true;
			}

			return false;
		}

		//!Gets a raw predicate from the input file
		std::string GetPredicate(bool *a_flag)
		{
			if (m_fp)
			{
				std::string l_string;

				if (true == p_GetLine(l_string))
				{
					if (a_flag)
					{
						*a_flag = true;
					}

					return l_string;
				}	
			}

			if (a_flag)
			{
				*a_flag = false;
			}

			return "";
		}

		//!Constructor
		CPredicateFileIO()
		{
			m_fp = NULL;
		}

		//!Destructor
		~CPredicateFileIO()
		{
			CloseInputFile();
		}

	private:
		//!Gets a line from the file
		bool p_GetLine(std::string &a_string) const
		{
			if (!m_fp)
				return false;

			while (!feof(m_fp))
			{
				char l_buffer[512];

				l_buffer[0] = 0;

				fgets(l_buffer, 512, m_fp);

				if (strlen(l_buffer) > 0)
				{
					int l_end = strlen(l_buffer);

					while (l_end > 0)
					{
						if (('\r' == l_buffer[l_end - 1]) ||
						    ('\n' == l_buffer[l_end - 1]))
						{
							l_end--;
						}
						else break;
					}

					if ((l_end >  0) &&
					    ('#'   != l_buffer[0]))
					{
						a_string.assign(l_buffer, l_end);

						return true;
					}
				}
			}

			return false;
		}

		//!File handle for current opened file
		FILE *m_fp;
};

#endif

