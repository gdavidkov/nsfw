#ifndef NSFW_H
#define NSFW_H

#include "NativeInterface.h"
#include <nan.h>
#include <uv.h>
#include <vector>

using namespace Nan;

class NSFW : public ObjectWrap {
public:
  static NAN_MODULE_INIT(Init);

  static void cleanupEventCallback(void *arg);
  static void fireEventCallback(uv_async_t *handle);
  static void pollForEvents(void *arg);

  uint32_t mDebounceMS;
  Callback *mEventCallback;
  NativeInterface *mInterface;
  std::string mPath;
  uv_thread_t mPollThread;
  bool mRunning;
private:
  NSFW(uint32_t debounceMS, std::string path, Callback *eventCallback);
  ~NSFW();

  struct EventBaton {
    NSFW *nsfw;
    std::vector<Event *> *events;
  };

  static NAN_METHOD(JSNew);

  static NAN_METHOD(Start);
  class StartWorker : public AsyncWorker {
  public:
    StartWorker(NSFW *nsfw, Callback *callback);
    void Execute();
    void HandleOKCallback();
  private:
    NSFW *mNSFW;
  };

  static NAN_METHOD(Stop);
  class StopWorker : public AsyncWorker {
  public:
    StopWorker(NSFW *nsfw, Callback *callback);
    void Execute();
    void HandleOKCallback();
  private:
    NSFW *mNSFW;
  };

  static Persistent<v8::Function> constructor;
};

#endif
