// No HTTP on this build (the Switch port without ORACLES_SWITCH_SYNC): sync shows as unavailable.
#include "sync/http.h"
#include <stdio.h>
#include <string.h>

bool http_available(void) { return false; }

bool http_request(const char *method, const char *url, const char *headers, const uint8_t *body, size_t size, HttpResponse *res) {
  (void)method; (void)url; (void)headers; (void)body; (void)size;
  memset(res, 0, sizeof *res);
  snprintf(res->error, sizeof res->error, "sync is not built into this port");
  return false;
}
