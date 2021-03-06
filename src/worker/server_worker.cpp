/*
* The MIT License (MIT)
*
* Copyright (c) 2017-2018 youjing@yy.com
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include "server_worker.h"

#ifndef ORPC_USE_FIBER

#include "../util/base_util.h"
#include "../stub/callee_manager.h"
#include "../error_def.h"

_START_ORPC_NAMESPACE_

ServerWorker::ServerWorker()
{
  Start();
}

ServerWorker::~ServerWorker()
{
  Stop();

  std::shared_ptr<CalleePacket> null;
  QueueTask(null);

  Join();
}

static void idle_cb(uv_idle_t *handle) {
  ServerWorker* worker = (ServerWorker*)handle->data;
  worker->_OnIdle(handle);
}

int ServerWorker::_DoWork()
{
  _PrepareWork();

  int r;

  uv_idle_init(&m_loop, &m_idler);
  m_idler.data = this;
  uv_idle_start(&m_idler, idle_cb);

  r = uv_run(&m_loop, UV_RUN_DEFAULT);
  UV_CHECK_RET_1(uv_run, r);

  r = uv_loop_close(&m_loop);
  UV_CHECK_RET_1(uv_loop_close, r);

  return 0;
}

int ServerWorker::_OnAsync(uv_async_t* handle)
{
  ThreadWorker::_OnAsync(handle);
  return 0;
}

int ServerWorker::_OnDestory(uv_handle_t* handle)
{
  uv_close((uv_handle_t*)&m_idler, NULL);
  return 0;
}

int ServerWorker::_OnIdle(uv_idle_t *handle)
{
  std::list<std::shared_ptr<CalleePacket>> clone_request;
  m_taskList.clone(clone_request);

  std::list<std::shared_ptr<CalleePacket>> retry_request;

  auto it = clone_request.begin();
  for (; it != clone_request.end(); ++it)
  {
    if (DoTask(*it) != 0)
      retry_request.push_back(*it);
  }

  auto retry_it = retry_request.rbegin();
  for (; retry_it != retry_request.rend(); ++retry_it)
    m_taskList.push_front(*retry_it);

  m_taskList.try_wait(std::chrono::milliseconds(50));
  return 0;
}

int ServerWorker::QueueTask(const std::shared_ptr<CalleePacket>& task)
{
  m_taskList.push_back(task);
  return 0;
}

int ServerWorker::DoTask(const std::shared_ptr<CalleePacket>& task)
{
  if (!task)
    return 0;

  if (!task->RunImpl())
  {
    ORPC_LOG(ERROR) << "task run failed, method_name:" << task->method_name << " sessionid: " << task->param.session_id;
    task->SendError(ORPC_ERROR_BODY_FAILED);
  }

  //don't retry
  return 0;
}

_END_ORPC_NAMESPACE_

#endif //! #ifndef ORPC_USE_FIBER
