// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <spdlog/details/file_helper.h>
#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/synchronous_factory.h>

#include <mutex>
#include <string>

namespace spdlog {
namespace sinks {
/*
 * Multi-process compatible file sink with single file as target
 */
template<typename Mutex>
class multi_process_file_sink final : public base_sink<Mutex>
{
public:
    explicit multi_process_file_sink(const filename_t &filename, bool truncate = false);
    const filename_t &filename() const;

protected:
    void sink_it_(const details::log_msg &msg) override;
    void flush_() override;

private:
    details::file_helper file_helper_;
};

using multi_process_file_sink_mt = multi_process_file_sink<std::mutex>;
using multi_process_file_sink_st = multi_process_file_sink<details::null_mutex>;

} // namespace sinks

//
// factory functions
//
template<typename Factory = spdlog::synchronous_factory>
inline std::shared_ptr<logger> multi_process_logger_mt(const std::string &logger_name, const filename_t &filename, bool truncate = false)
{
    return Factory::template create<sinks::multi_process_file_sink_mt>(logger_name, filename, truncate);
}

template<typename Factory = spdlog::synchronous_factory>
inline std::shared_ptr<logger> multi_process_logger_st(const std::string &logger_name, const filename_t &filename, bool truncate = false)
{
    return Factory::template create<sinks::multi_process_file_sink_st>(logger_name, filename, truncate);
}

} // namespace spdlog

#ifdef SPDLOG_HEADER_ONLY
#include "basic_file_sink-inl.h"
#endif