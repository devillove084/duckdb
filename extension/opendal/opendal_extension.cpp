#include "opendal_extension.hpp"
#include "opendal.hpp"

namespace duckdb {
void OpendalExtension::Load(ExtensionLoader &loader) {
	auto op = opendal::Operator("memory");
	std::string data {"1243"};
	op.Write("test", data);
	auto result = op.Read("test");
	std::cerr << "[opendal] smoketest read back: " << result << '\n';
}

std::string OpendalExtension::Name() {
	return "opendal";
}

std::string OpendalExtension::Version() const {
	return "main";
}
} // namespace duckdb
