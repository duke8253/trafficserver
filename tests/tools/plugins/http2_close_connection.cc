/** @file
  Test adding continuation from same hook point
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
#include <string.h>

#define PLUGIN_NAME "http2_close_connection"

static DbgCtl dbg_ctl_tag{PLUGIN_NAME};

static int
txn_handler(TSCont /* contp */, TSEvent event, void *edata)
{
  Dbg(dbg_ctl_tag, "txn_handler event: %d", event);

  TSHttpTxn txnp = static_cast<TSHttpTxn>(edata);

  if (TSHttp2GraceShutdown(txnp) == TS_SUCCESS) {
    Dbg(dbg_ctl_tag, "TSHttp2GraceShutdown success: %p", txnp);
  } else {
    Dbg(dbg_ctl_tag, "TSHttp2GraceShutdown failed: %p", txnp);
  }

  TSHttpTxnReenable(txnp, TS_EVENT_HTTP_CONTINUE);
  return TS_EVENT_NONE;
}

void
TSPluginInit(int argc, const char **argv)
{
  TSPluginRegistrationInfo info;

  info.plugin_name   = const_cast<char *>(PLUGIN_NAME);
  info.support_email = const_cast<char *>("feid@yahooinc.com");
  info.vendor_name   = const_cast<char *>("Yahoo");

  if (TSPluginRegister(&info) != TS_SUCCESS) {
    TSError("[" PLUGIN_NAME "] plugin registration failed\n");
    return;
  }

  Dbg(dbg_ctl_tag, "plugin registered");
  TSCont txn_cont = TSContCreate(txn_handler, nullptr);
  TSHttpHookAdd(TS_HTTP_READ_RESPONSE_HDR_HOOK, txn_cont);
}
