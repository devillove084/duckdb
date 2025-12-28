#include "opendal_scan_function.hpp"
#include "duckdb/common/helper.hpp"
#include "duckdb/common/types.hpp"
#include "duckdb/common/unique_ptr.hpp"
#include "duckdb/function/function.hpp"
#include "duckdb/function/table_function.hpp"

namespace duckdb {

OpendalScanFunction::OpendalScanFunction()
    : TableFunction("opendal_scan", {LogicalType::VARCHAR, LogicalType::VARCHAR}, Scan, Bind, InitGlobal, InitLocal) {
}

unique_ptr<FunctionData> OpendalScanFunction::Bind(ClientContext &context, TableFunctionBindInput &input,
                                                   vector<LogicalType> &return_types, vector<string> &names) {
	auto result = make_uniq<OpendalScanData>();

	if (input.inputs.size() >= 1) {
		result->backend = input.inputs[0].GetValue<string>();
	}

	if (input.inputs.size() >= 2) {
		result->backend = input.inputs[1].GetValue<string>();
	}

	names = {"path", "size", "content", "backend"};

	return_types = {
	    LogicalType::VARCHAR, // path
	    LogicalType::BIGINT,  // size
	    LogicalType::VARCHAR, // content
	    LogicalType::VARCHAR  // backend
	};

	return std::move(result);
}

unique_ptr<GlobalTableFunctionState> OpendalScanFunction::InitGlobal(ClientContext &context,
                                                                     TableFunctionInitInput &input) {
	auto &bind_data = input.bind_data->Cast<OpendalScanData>();
	auto result = make_uniq<GlobalTableFunctionState>();

	return std::move(result);
}

unique_ptr<LocalTableFunctionState> OpendalScanFunction::InitLocal(ExecutionContext &context,
                                                                   TableFunctionInitInput &input,
                                                                   GlobalTableFunctionState *global_state) {
	// todo
	return make_uniq<LocalTableFunctionState>();
}

} // namespace duckdb
