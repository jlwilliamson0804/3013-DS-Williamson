/*
	__ _____ _____ _____
 __|  |   __|     |   | |  JSON for Modern C++
|  |  |__   |  |  | | | |  version 3.7.0
|_____|_____|_____|_|___|  https://github.com/nlohmann/json
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2013-2019 Niels Lohmann <http://nlohmann.me>.
Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef INCLUDE_NLOHMANN_JSON_HPP_
#define INCLUDE_NLOHMANN_JSON_HPP_

#define NLOHMANN_JSON_VERSION_MAJOR 3
#define NLOHMANN_JSON_VERSION_MINOR 7
#define NLOHMANN_JSON_VERSION_PATCH 0

#include <algorithm> // all_of, find, for_each
#include <cassert> // assert
#include <ciso646> // and, not, or
#include <cstddef> // nullptr_t, ptrdiff_t, size_t
#include <functional> // hash, less
#include <initializer_list> // initializer_list
#include <iosfwd> // istream, ostream
#include <iterator> // random_access_iterator_tag
#include <memory> // unique_ptr
#include <numeric> // accumulate
#include <string> // string, stoi, to_string
#include <utility> // declval, forward, move, pair, swap
#include <vector> // vector

// #include <nlohmann/adl_serializer.hpp>


#include <utility>

// #include <nlohmann/detail/conversions/from_json.hpp>


#include <algorithm> // transform
#include <array> // array
#include <ciso646> // and, not
#include <forward_list> // forward_list
#include <iterator> // inserter, front_inserter, end
#include <map> // map
#include <string> // string
#include <tuple> // tuple, make_tuple
#include <type_traits> // is_arithmetic, is_same, is_enum, underlying_type, is_convertible
#include <unordered_map> // unordered_map
#include <utility> // pair, declval
#include <valarray> // valarray

// #include <nlohmann/detail/exceptions.hpp>


#include <exception> // exception
#include <stdexcept> // runtime_error
#include <string> // to_string

// #include <nlohmann/detail/input/position_t.hpp>


#include <cstddef> // size_t

namespace nlohmann
{
	namespace detail
	{
		/// struct to capture the start position of the current token
		struct position_t
		{
			/// the total number of characters read
			std::size_t chars_read_total = 0;
			/// the number of characters read in the current line
			std::size_t chars_read_current_line = 0;
			/// the number of lines read
			std::size_t lines_read = 0;

			/// conversion to size_t to preserve SAX interface
			constexpr operator size_t() const
			{
				return chars_read_total;
			}
		};

	} // namespace detail
} // namespace nlohmann

// #include <nlohmann/detail/macro_scope.hpp>


#include <utility> // pair
// #include <nlohmann/thirdparty/hedley/hedley.hpp>
/* Hedley - https://nemequ.github.io/hedley
 * Created by Evan Nemerson <evan@nemerson.com>
 *
 * To the extent possible under law, the author(s) have dedicated all
 * copyright and related and neighboring rights to this software to
 * the public domain worldwide. This software is distributed without
 * any warranty.
 *
 * For details, see <http://creativecommons.org/publicdomain/zero/1.0/>.
 * SPDX-License-Identifier: CC0-1.0
 */
/**
 * This file is kind of a facade that interfaces with a more complex json
 * class. The file `json.hpp` is needed for this helper to work. This facade
 * strips away 99.9% of the power that the original json.hpp file has but simplifies
 * its usage for our class.
 *
 * The `json.hpp` source code was obtained from https://github.com/nlohmann/json and was
 * authored by: Niels Lohmann
 *
 * For our purposes it assumes an array of objects. I might expand on it later but doubt
 * it. If you are interested, learn how to use the original in many much more powerful ways.
 */
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>

#include "json.hpp"

using namespace std;

using json = nlohmann::json;

class JsonFacade {
private:
	json Data;
	int Size;
	int Index;
	string FileName;

public:
	JsonFacade(string filename) {
		FileName = filename;

		ifstream input(FileName);
		input >> Data;
		input.close();

		Size = Data.size();
		Index = 0;
	}

	json getNext() {
		if (Index < Size) {
			return Data[Index++];
		}
		return NULL;
	}

	json getIth(int i) {
		if (i < Size) {
			return Data[i];
		}
		return NULL;
	}

	/**
	 * pushBack: converts a string to json and pushes on end of the json array
	 *
	 * Params:
	 *     string obj  : a string json object which whould be in the following format:
	 *
	 *                           "{\"key0\":"value0",\"key1\":1,\"keyn\":\"valuen"\}"
	 * Returns:
	 *     void
	 *
	 */
	void pushBack(string obj) {

		json j = json::parse(obj);
		Data.push_back(j);
		cout << Data.dump(4) << endl;
	}

	int getSize() {
		return Size;
	}

	void saveFile(string filename = "") {
		if (filename == "") {
			filename = FileName;
		}

		ofstream out(filename);
		out << std::setw(4) << Data << endl;
		out.close();
	}
};