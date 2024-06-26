#######################
#
#  Licensed to the Apache Software Foundation (ASF) under one or more contributor license
#  agreements.  See the NOTICE file distributed with this work for additional information regarding
#  copyright ownership.  The ASF licenses this file to you under the Apache License, Version 2.0
#  (the "License"); you may not use this file except in compliance with the License.  You may obtain
#  a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software distributed under the License
#  is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
#  or implied. See the License for the specific language governing permissions and limitations under
#  the License.
#
#######################

project(uri_signing)

add_atsplugin(
  uri_signing
  common.cc
  config.cc
  cookie.cc
  jwt.cc
  match.cc
  normalize.cc
  parse.cc
  timing.cc
  uri_signing.cc
)

target_link_libraries(
  uri_signing
  PRIVATE cjose::cjose
  PUBLIC jansson::jansson
)

verify_remap_plugin(uri_signing)

if(BUILD_TESTING)
  add_subdirectory(unit_tests)
endif()
