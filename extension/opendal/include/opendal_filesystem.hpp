//===----------------------------------------------------------------------===//
//                         DuckDB
//
// opendal_filesystem.hpp
//
//
//===----------------------------------------------------------------------===/

#pragma once

#include "duckdb/common/file_open_flags.hpp"
#include "duckdb/common/file_opener.hpp"
#include "duckdb/common/file_system.hpp"
#include "duckdb/common/unique_ptr.hpp"
#include "duckdb/common/optional_ptr.hpp"

namespace duckdb {

class OpendalFileSystem : public FileSystem {
	unique_ptr<FileHandle> OpenFile(const string &path, FileOpenFlags flags,
	                                optional_ptr<FileOpener> opener = nullptr) override;
};

} // namespace duckdb
