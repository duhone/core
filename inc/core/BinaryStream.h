#pragma once

#include "FileHandle.h"

#include "core/Log.h"

#include <cstddef>
#include <type_traits>
#include <vector>

namespace CR::Core {
	template<typename T>
	void Write(std::vector<std::byte>& a_stream, const T& a_arg) {
		static_assert(std::is_standard_layout_v<T>, "template writer for binary streams only supports pod types");
		auto offset = a_stream.size();
		a_stream.resize(a_stream.size() + sizeof(T));
		memcpy(a_stream.data() + offset, &a_arg, sizeof(T));
	}

	template<typename T>
	void Write(std::vector<std::byte>& a_stream, const std::vector<T>& a_arg) {
		static_assert(std::is_standard_layout_v<T>,
		              "template writer for binary streams only supports vectors of pod types");
		Write(a_stream, (uint32_t)a_arg.size());
		auto offset = a_stream.size();
		a_stream.resize(a_stream.size() + a_arg.size() * sizeof(T));
		memcpy(a_stream.data() + offset, a_arg.data(), a_arg.size() * sizeof(T));
	}

	template<typename T>
	void Write(FileHandle& a_file, const T& a_arg) {
		static_assert(std::is_standard_layout_v<T>, "template writer for binary streams only supports pod types");
		fwrite(&a_arg, sizeof(T), 1, a_file);
	}

	template<typename T>
	void Write(FileHandle& a_file, const std::vector<T>& a_arg) {
		static_assert(std::is_standard_layout_v<T>,
		              "template writer for binary streams only supports vectors of pod types");
		Write(a_file, (uint32_t)a_arg.size());
		fwrite(a_arg.data(), sizeof(T), a_arg.size(), a_file);
	}

	// Reading should always come from a memory mapped file, so less help is needed.
	struct BinaryReader final {
		BinaryReader() = default;
		// input stream must outlive the reader
		BinaryReader(const std::vector<std::byte>& a_stream) : Data(a_stream.data()), Size((uint32_t)a_stream.size()) {}

		const std::byte* Data{nullptr};
		uint32_t Offset{0};
		uint32_t Size{0};
	};

	template<typename T>
	void Read(BinaryReader& a_stream, T& a_out) {
		static_assert(std::is_standard_layout_v<T>, "template writer for binary streams only supports pod types");
		if(a_stream.Offset + sizeof(T) > a_stream.Size) {
			CR::Core::Log::Fail("Tried to read past the end of the buffer");
		}
		memcpy(&a_out, a_stream.Data + a_stream.Offset, sizeof(T));
		a_stream.Offset += sizeof(T);
	}

	template<typename T>
	void Read(BinaryReader& a_stream, std::vector<T>& a_out) {
		uint32_t outSize = 0;
		Read(a_stream, outSize);

		if(a_stream.Offset + outSize * sizeof(T) > a_stream.Size) {
			CR::Core::Log::Fail("Tried to read past the end of the buffer");
		}

		a_out.resize(outSize);
		memcpy(a_out.data(), a_stream.Data + a_stream.Offset, outSize * sizeof(T));
		a_stream.Offset += outSize * sizeof(T);
	}
}    // namespace CR::Core
