/** @file

  an example plugin showing off how to use versioning

  @section license License

  Licensed to the Apache Software Foundation (ASF) under one
  or more contributor license agreements.  See the NOTICE file
  distributed with this work for additional information
  regarding copyright ownership.  The ASF licenses this file
  to you under the Apache License, Version 2.0 (the
  "License"); you may not use this file except in compliance
  with the License.  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 */

#include <ts/ts.h>

#define PLUGIN_NAME "hook-trace"

static DbgCtl dbg_ctl{PLUGIN_NAME};

template <typename T, unsigned N>
static inline unsigned
countof(const T (&)[N])
{
  return N;
}

static int
HttpHookTracer(TSCont /* contp ATS_UNUSED */, TSEvent event, void *edata)
{
  union {
    TSHttpTxn     txn;
    TSHttpSsn     ssn;
    TSHttpAltInfo alt;
    void         *ptr;
  } ev;

  ev.ptr = edata;

  switch (event) {
  case TS_EVENT_HTTP_SSN_START:
    Dbg(dbg_ctl, "Received SSN_START on session %p", ev.ssn);
    TSHttpSsnReenable(ev.ssn, TS_EVENT_HTTP_CONTINUE);
    break;

  case TS_EVENT_HTTP_SSN_CLOSE:
    Dbg(dbg_ctl, "Received SSN_CLOSE on session %p", ev.ssn);
    TSHttpSsnReenable(ev.ssn, TS_EVENT_HTTP_CONTINUE);
    break;

  case TS_EVENT_HTTP_SELECT_ALT:
    Dbg(dbg_ctl, "Received SELECT_ALT on altinfo %p", ev.alt);
    break;

  case TS_EVENT_HTTP_READ_REQUEST_HDR:
    Dbg(dbg_ctl, "Received READ_REQUEST_HDR on transaction %p", ev.txn);
    TSHttpTxnReenable(ev.txn, TS_EVENT_HTTP_CONTINUE);
    break;

  case TS_EVENT_HTTP_OS_DNS:
    Dbg(dbg_ctl, "Received OS_DNS on transaction %p", ev.txn);
    TSHttpTxnReenable(ev.txn, TS_EVENT_HTTP_CONTINUE);
    break;

  case TS_EVENT_HTTP_SEND_REQUEST_HDR:
    Dbg(dbg_ctl, "Received SEND_REQUEST_HDR on transaction %p", ev.txn);
    TSHttpTxnReenable(ev.txn, TS_EVENT_HTTP_CONTINUE);
    break;

  case TS_EVENT_HTTP_READ_CACHE_HDR:
    Dbg(dbg_ctl, "Received READ_CACHE_HDR on transaction %p", ev.txn);
    TSHttpTxnReenable(ev.txn, TS_EVENT_HTTP_CONTINUE);
    break;

  case TS_EVENT_HTTP_READ_RESPONSE_HDR:
    Dbg(dbg_ctl, "Received READ_RESPONSE_HDR on transaction %p", ev.txn);
    TSHttpTxnReenable(ev.txn, TS_EVENT_HTTP_CONTINUE);
    break;

  case TS_EVENT_HTTP_SEND_RESPONSE_HDR:
    Dbg(dbg_ctl, "Received SEND_RESPONSE_HDR on transaction %p", ev.txn);
    TSHttpTxnReenable(ev.txn, TS_EVENT_HTTP_CONTINUE);
    break;

  case TS_EVENT_HTTP_TXN_START:
    Dbg(dbg_ctl, "Received TXN_START on transaction %p", ev.txn);
    TSHttpTxnReenable(ev.txn, TS_EVENT_HTTP_CONTINUE);
    break;

  case TS_EVENT_HTTP_TXN_CLOSE:
    Dbg(dbg_ctl, "Received TXN_CLOSE on transaction %p", ev.txn);
    TSHttpTxnReenable(ev.txn, TS_EVENT_HTTP_CONTINUE);
    break;

  case TS_EVENT_HTTP_CACHE_LOOKUP_COMPLETE:
    Dbg(dbg_ctl, "Received CACHE_LOOKUP_COMPLETE on transaction %p", ev.txn);
    TSHttpTxnReenable(ev.txn, TS_EVENT_HTTP_CONTINUE);
    break;

  case TS_EVENT_HTTP_PRE_REMAP:
    Dbg(dbg_ctl, "Received PRE_REMAP on transaction %p", ev.txn);
    TSHttpTxnReenable(ev.txn, TS_EVENT_HTTP_CONTINUE);
    break;

  case TS_EVENT_HTTP_POST_REMAP:
    Dbg(dbg_ctl, "Received POST_REMAP on transaction %p", ev.txn);
    TSHttpTxnReenable(ev.txn, TS_EVENT_HTTP_CONTINUE);
    break;

  default:
    break;
  }

  return TS_EVENT_NONE;
}

static int
LifecycleHookTracer(TSCont /* contp ATS_UNUSED */, TSEvent event, void *edata)
{
  switch (event) {
  case TS_EVENT_LIFECYCLE_PORTS_INITIALIZED:
    Dbg(dbg_ctl, "Received LIFECYCLE_PORTS_INITIALIZED data %p", edata);
    break;

  case TS_EVENT_LIFECYCLE_PORTS_READY:
    Dbg(dbg_ctl, "Received LIFECYCLE_PORTS_READY data %p", edata);
    break;

  case TS_EVENT_LIFECYCLE_CACHE_READY:
    Dbg(dbg_ctl, "Received LIFECYCLE_CACHE_READY data %p", edata);
    break;

  case TS_EVENT_LIFECYCLE_SERVER_SSL_CTX_INITIALIZED:
    Dbg(dbg_ctl, "Received LIFECYCLE_SERVER_SSL_INITIALIZED data %p", edata);
    break;

  case TS_EVENT_LIFECYCLE_CLIENT_SSL_CTX_INITIALIZED:
    Dbg(dbg_ctl, "Received LIFECYCLE_CLIENT_SSL_CTX_INITIALIZED data %p", edata);
    break;

  case TS_EVENT_LIFECYCLE_MSG:
    Dbg(dbg_ctl, "Received LIFECYCLE_MSG data %p", edata);
    break;

  default:
    Dbg(dbg_ctl, "Received unsupported lifecycle event %d data %p", event, edata);
    break;
  }

  return TS_EVENT_NONE;
}

void
TSPluginInit(int argc, const char *argv[])
{
  // clang-format off
  static const TSHttpHookID http[] = {
    TS_HTTP_READ_REQUEST_HDR_HOOK,
    TS_HTTP_OS_DNS_HOOK,
    TS_HTTP_SEND_REQUEST_HDR_HOOK,
    TS_HTTP_READ_CACHE_HDR_HOOK,
    TS_HTTP_READ_RESPONSE_HDR_HOOK,
    TS_HTTP_SEND_RESPONSE_HDR_HOOK,
    TS_HTTP_SELECT_ALT_HOOK,
    TS_HTTP_TXN_START_HOOK,
    TS_HTTP_TXN_CLOSE_HOOK,
    TS_HTTP_SSN_START_HOOK,
    TS_HTTP_SSN_CLOSE_HOOK,
    TS_HTTP_CACHE_LOOKUP_COMPLETE_HOOK,
    TS_HTTP_PRE_REMAP_HOOK,
    TS_HTTP_POST_REMAP_HOOK,
  };

  static const TSLifecycleHookID lifecycle[] = {
    TS_LIFECYCLE_PORTS_INITIALIZED_HOOK,
    TS_LIFECYCLE_PORTS_READY_HOOK,
    TS_LIFECYCLE_CACHE_READY_HOOK,
    TS_LIFECYCLE_SERVER_SSL_CTX_INITIALIZED_HOOK,
    TS_LIFECYCLE_CLIENT_SSL_CTX_INITIALIZED_HOOK,
    TS_LIFECYCLE_MSG_HOOK,
  };

  // clang-format on

  (void)argc; // unused
  (void)argv; // unused

  TSPluginRegistrationInfo info;

  info.plugin_name   = (char *)PLUGIN_NAME;
  info.vendor_name   = (char *)"Apache Software Foundation";
  info.support_email = (char *)"dev@trafficserver.apache.org";

  for (unsigned i = 0; i < countof(http); ++i) {
    TSHttpHookAdd(http[i], TSContCreate(HttpHookTracer, TSMutexCreate()));
  }

  for (unsigned i = 0; i < countof(lifecycle); ++i) {
    TSLifecycleHookAdd(lifecycle[i], TSContCreate(LifecycleHookTracer, TSMutexCreate()));
  }

  TSReleaseAssert(TSPluginRegister(&info) == TS_SUCCESS);
}
