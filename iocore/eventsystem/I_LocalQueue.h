/** @file

  A brief file description

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

#pragma once

#include "tscore/ink_platform.h"
#include "I_Event.h"

class LocalQueue
{
public:
  void enqueue(Event *e);
  void swap_queue();
  Event *dequeue();
  bool is_next_empty();

  Que(Event, link) * curr;
  Que(Event, link) * next;

  Que(Event, link) queue_1;
  Que(Event, link) queue_2;

  LocalQueue();
};

inline LocalQueue::LocalQueue()
{
  curr = &queue_1;
  next = &queue_2;
}

inline void
LocalQueue::enqueue(Event *e)
{
  curr->enqueue(e);
}

inline void
LocalQueue::swap_queue()
{
  Que(Event, link) * temp;
  temp = curr;
  curr = next;
  next = temp;
}

inline Event *
LocalQueue::dequeue()
{
  return curr->dequeue();
}

inline bool
LocalQueue::is_next_empty()
{
  return next->empty();
}
