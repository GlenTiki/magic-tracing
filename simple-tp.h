
#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER nearform

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./simple-tp.h"

#if !defined(SIMPLE_TP_H) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define SIMPLE_TP_H

#include <lttng/tracepoint.h>

TRACEPOINT_EVENT(
  nearform,
  async_delay,
  TP_ARGS(
    const char*, msg,
    int, delay
  ),
  TP_FIELDS(
    ctf_string(msg, msg)
    ctf_integer(int, delay, delay)
  )
)

TRACEPOINT_EVENT(
  nearform,
  run_cb,
  TP_ARGS(
    const char*, msg
  ),
  TP_FIELDS(
    ctf_string(msg, msg)
  )
)


#endif /* HELLO_TP_H */

#include <lttng/tracepoint-event.h>
