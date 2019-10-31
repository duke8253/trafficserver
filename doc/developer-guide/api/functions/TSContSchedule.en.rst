.. Licensed to the Apache Software Foundation (ASF) under one or more
   contributor license agreements.  See the NOTICE file distributed
   with this work for additional information regarding copyright
   ownership.  The ASF licenses this file to you under the Apache
   License, Version 2.0 (the "License"); you may not use this file
   except in compliance with the License.  You may obtain a copy of
   the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
   implied.  See the License for the specific language governing
   permissions and limitations under the License.

.. include:: ../../../common.defs

.. default-domain:: c

TSContSchedule
**************

Synopsis
========

.. code-block:: cpp

    #include <ts/ts.h>

.. function:: TSAction TSContSchedule(TSCont contp, TSHRTime timeout)

Description
===========

Schedules :arg:`contp` to run :arg:`timeout` milliseconds in the future. This is approximate. The delay
will be at least :arg:`timeout` but possibly more. Resolutions finer than roughly 5 milliseconds will
not be effective. :arg:`contp` is required to have a mutex, which is provided to :func:`TSContCreate`.

When scheduling with a 0 :arg:`timeout`, it is implied that the continuation should be handled
immediately, i.e. the :doc:`eventloop.en` will not wait for the next loop to handle the it.

The return value can be used to cancel the scheduled event via :func:`TSActionCancel`. This is
effective until the continuation :arg:`contp` is being dispatched. However, if it is scheduled on
another thread this can be problematic to be correctly timed. The return value can be checked with
:func:`TSActionDone` to see if the continuation ran before the return, which is possible if
:arg:`timeout` is `0`. Returns ``nullptr`` if thread affinity was cleared.

See Also
========

:doc:`TSContScheduleOnPool.en`
:doc:`TSContScheduleOnThread.en`
