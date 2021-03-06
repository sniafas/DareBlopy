//   Copyright 2019-2020 Stanislav Pidhorskyi
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#pragma once
#include <string>
#include "protobuf/example.pb.h"
#include "MemRefFile.h"
#include "common.h"

namespace Records
{
	enum class DataType
	{
		DT_INVALID = 0,
		DT_FLOAT = 1,
		DT_UINT8 = 4,
		DT_STRING = 7,
		DT_INT64 = 9,
	};

	typedef std::vector<size_t> TensorShape;

	class HIDDEN RecordParser
	{
	public:
		struct HIDDEN FixedLenFeature
		{
			FixedLenFeature() = default;

			FixedLenFeature(const TensorShape& shape, DataType dtype): dtype(dtype), shape(shape)
			{
			}

			FixedLenFeature(const TensorShape& shape, DataType dtype, py::object default_value): dtype(dtype), shape(shape), default_value(default_value)
			{
			}

			std::string key;
			TensorShape shape;
			DataType dtype;
			py::object default_value;
		};

		explicit RecordParser(const py::dict& features, bool run_parallel=true, int worker_count=12);

		void ParseSingleExampleInplace(const std::string& serialized, std::vector<py::object>& output, int batch_index);

		py::list ParseExample(const std::vector<std::string>& serialized);

		py::list ParseSingleExample(const std::string& serialized);
	private:
		void ParseSingleExampleImpl(const std::string& serialized, std::vector<void*>& output, int batch_index);

		std::vector<FixedLenFeature> fixed_len_features;
		bool m_run_parallel;
	};
}
