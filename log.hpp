#include <spdlog/spdlog.h>

namespace ic_server
{

#define IC_SERVER_LOG_ALIGN_VALUE "15"
#define IC_SERVER_LOG_ALIGN_LEFT "<"
#define IC_SERVER_LOG_ALIGN_RIGHT ">"

#define IC_SERVER_LOG_ALIGN IC_SERVER_LOG_ALIGN_RIGHT

#define ic_server_log(fmt, ...) spdlog::info("[{:" IC_SERVER_LOG_ALIGN IC_SERVER_LOG_ALIGN_VALUE "}] " fmt, __VA_ARGS__)
//namespace log = spdlog;

}
