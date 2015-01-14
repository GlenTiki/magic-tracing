#include <nan.h>

#include <iostream>
#include <cstring>
#include <string>
#include "simple-tp.h"

using namespace v8;

#ifndef _WIN32
  #include <unistd.h>
#endif

// A worker class extending the NanAsyncWorker helper
// class, a simple encapsulation of worker-thread
// logic to make simple tasks easier

class MyWorker : public NanAsyncWorker {
 public:
  // Constructor
  MyWorker(NanCallback *callback, int delay) 
    : NanAsyncWorker(callback), delay(delay) {
      //this->delay = delay;
    }
  
  // Destructor
  ~MyWorker() {}

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on `this`.
  void Execute () {
    // Asynchronous, non-V8 work goes here
    #ifdef _WIN32
      Sleep(delay);
    #else
      usleep(delay * 1000);
    #endif
  }

  // Executed when the async work is complete
  // this function will be run inside the main event loop
  // so it is safe to use V8 again
  void HandleOKCallback () {
    NanScope();
    // NanCallback#Call() does a NanMakeCallback() for us
    callback->Call(0, NULL);
    std::string msg = "Callback called after delay!";
    tracepoint(nearform, async_delay, msg.c_str(), delay);
  }

 private:
  int delay;
};

NAN_METHOD(DelayAsync) {
  NanScope();

  int delay = args[0].As<Number>()->IntegerValue();

  std::string msg = "Callback to be called after delay!";
  tracepoint(nearform, async_delay, msg.c_str(), delay);

  Local<Function> callback = args[1].As<Function>();
  NanCallback* nanCallback = new NanCallback(callback);
  
  MyWorker* worker = new MyWorker(nanCallback, delay);
  
  NanAsyncQueueWorker(worker);

  NanReturnUndefined();
}

NAN_METHOD(DelaySync) {
  NanScope();

  int delay = args[0].As<Number>()->IntegerValue();

  Local<Function> callback = args[1].As<Function>();

  #ifdef _WIN32
    Sleep(delay);
  #else
    usleep(delay * 1000);
  #endif

  callback->Call(Context::GetCurrent()->Global(), 0, NULL);

  NanReturnUndefined();
}

NAN_METHOD(Call) {
  NanScope();

  Local<Function> callback = args[0].As<Function>();

  const unsigned argc = 1;
  Local<Value> argv[argc];
  argv[0] = Local<Value>::New(String::New("hello world"));

  callback->Call(Context::GetCurrent()->Global(), argc, argv);

  std::string msg = "simple callback called!";
  tracepoint(nearform, run_cb, msg.c_str());

  NanReturnUndefined();
}

void Init(Handle<Object> exports, Handle<Object> module) {
  exports->Set(NanNew("delaySync"), NanNew<FunctionTemplate>(DelaySync)->GetFunction());
  exports->Set(NanNew("delayAsync"), NanNew<FunctionTemplate>(DelayAsync)->GetFunction());
  exports->Set(NanNew("call"), NanNew<FunctionTemplate>(Call)->GetFunction());
  //module->Set(String::NewSymbol("exports"), FunctionTemplate::New(RunCallback)->GetFunction());
}

NODE_MODULE(myaddon, Init)
