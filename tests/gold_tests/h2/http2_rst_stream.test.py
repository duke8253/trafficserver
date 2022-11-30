'''
Abort HTTP/2 connection using RST_STREAM frame.
'''
# @file
#
# Copyright 2022, Verizon Media
# SPDX-License-Identifier: Apache-2.0
#

Test.Summary = '''
Abort HTTP/2 connection using RST_STREAM frame.
'''

Test.SkipUnless(
    Condition.HasOpenSSLVersion('1.1.1')
)

#
# Client sends RST_STREAM after DATA frame
#
ts = Test.MakeATSProcess("ts0", enable_tls=True, enable_cache=False)
replay_file = "replay/http2_rst_stream_client_after_data.yaml"
server = Test.MakeVerifierServerProcess("server0", replay_file)
ts.addDefaultSSLFiles()
ts.Disk.records_config.update({
    'proxy.config.ssl.server.cert.path': f'{ts.Variables.SSLDir}',
    'proxy.config.ssl.server.private_key.path': f'{ts.Variables.SSLDir}',
    'proxy.config.ssl.client.verify.server.policy': 'PERMISSIVE',
    'proxy.config.diags.debug.enabled': 1,
    'proxy.config.diags.debug.tags': 'http',
})
ts.Disk.remap_config.AddLine(
    'map / https://127.0.0.1:{0}'.format(server.Variables.ssl_port)
)
ts.Disk.ssl_multicert_config.AddLine(
    'dest_ip=* ssl_cert_name=server.pem ssl_key_name=server.key'
)

tr = Test.AddTestRun('Client sends RST_STREAM after DATA frame')
tr.Processes.Default.StartBefore(server)
tr.Processes.Default.StartBefore(ts)
tr.AddVerifierClientProcess("client0", replay_file, http_ports=[ts.Variables.port], https_ports=[ts.Variables.ssl_port])

tr.Processes.Default.Streams.stdout += Testers.ContainsExpression(
    'Submitting RST_STREAM frame for key 1 after DATA frame with error code INTERNAL_ERROR.',
    'Detect client abort flag.')

tr.Processes.Default.Streams.stdout += Testers.ContainsExpression(
    'Sent frame for key 1: RST_STREAM',
    'Send RST_STREAM frame.')

server.Streams.stdout += Testers.ExcludesExpression(
    'RST_STREAM',
    'Server is not affected.')

ts.Streams.stdout += Testers.ContainsExpression(
    'Received RST_STREAM frame with error code INTERNAL_ERROR',
    'Received RST_STREAM frame.')

ts.Streams.stdout += Testers.ContainsExpression(
    'Frame sequence from client: HEADERS, DATA, RST_STREAM',
    'Frame sequence.')

# #
# # Test 2: Client sends RST_STREAM after HEADERS frame
# #
# r = Test.AddTestRun('Client sends RST_STREAM after HEADERS frame')
# client = r.AddClientProcess('client2', 'replay_files/client_rst_stream_after_headers.yaml')
# server = r.AddServerProcess('server2', 'replay_files/client_rst_stream_after_headers.yaml')
# proxy = r.AddProxyProcess('proxy2', listen_port=client.Variables.https_port,
#                           server_port=server.Variables.https_port,
#                           use_ssl=True, use_http2_to_2=True)

# client.Streams.stdout += Testers.ContainsExpression(
#     'Submitting RST_STREAM frame for key 1 after HEADERS frame with error code STREAM_CLOSED.',
#     'Detect client abort flag.')

# client.Streams.stdout += Testers.ContainsExpression(
#     'Sent frame for key 1: RST_STREAM',
#     'Send RST_STREAM frame.')

# server.Streams.stdout += Testers.ExcludesExpression(
#     'RST_STREAM',
#     'Server is not affected.')

# proxy.Streams.stdout += Testers.ContainsExpression(
#     'Received RST_STREAM frame with error code STREAM_CLOSED',
#     'Received RST_STREAM frame.')

# proxy.Streams.stdout += Testers.ContainsExpression(
#     'Frame sequence from client: HEADERS, RST_STREAM',
#     'Frame sequence.')

# #
# # Test 3: Server sends RST_STREAM after HEADERS frame
# #
# r = Test.AddTestRun('Server sends RST_STREAM after HEADERS frame')
# client = r.AddClientProcess('client3', 'replay_files/server_rst_stream_after_headers.yaml')
# server = r.AddServerProcess('server3', 'replay_files/server_rst_stream_after_headers.yaml')
# proxy = r.AddProxyProcess('proxy3', listen_port=client.Variables.https_port,
#                           server_port=server.Variables.https_port,
#                           use_ssl=True, use_http2_to_2=True)

# client.Streams.stdout += Testers.ExcludesExpression(
#     'RST_STREAM',
#     'Client is not affected.')

# server.Streams.stdout = "gold/server_after_headers.gold"

# proxy.Streams.stdout += Testers.ContainsExpression(
#     'httpcore.RemoteProtocolError:',
#     'Received RST_STREAM frame.')

# proxy.Streams.stdout += Testers.ContainsExpression(
#     'error_code:ErrorCodes.ENHANCE_YOUR_CALM, remote_reset:True',
#     'Received RST_STREAM frame.')
