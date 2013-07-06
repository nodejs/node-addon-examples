#include <node.h>
#include <cstdlib>

using namespace v8;

double Estimate (int points) {
  int i = points;
  int inside = 0;
	unsigned int seed = rand();
  double r1, r2;

  while (i-- > 0) {
    r1 = rand_r(&seed) / (double)RAND_MAX;
    r2 = rand_r(&seed) / (double)RAND_MAX;

    if ((r1 * r1) + (r2 * r2) <= 1)
      inside++;
  }


  return (inside / (double)points) * 4;
}

Handle<Value> CalculateSync(const Arguments& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  int points = args[0]->Uint32Value();
  double est = Estimate(points);

  return scope.Close(Number::New(est));
}

typedef struct AsyncData {
  int points;
  Persistent<Function> callback;
  double estimate;
} AsyncData;

void AsyncWork(uv_work_t *req) {
  AsyncData *asyncData = (AsyncData *)req->data;
  asyncData->estimate = Estimate(asyncData->points);
}

void AsyncAfter(uv_work_t *req) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  AsyncData *asyncData = (AsyncData *)req->data;
  Handle<Value> argv[] = {
    Null(),
    Number::New(isolate, asyncData->estimate)
  };

  TryCatch try_catch;
  asyncData->callback->Call(Context::GetCurrent()->Global(), 2, argv);
  asyncData->callback.Dispose(isolate);
  delete asyncData;
  delete req;
  if (try_catch.HasCaught())
    node::FatalException(try_catch);
}

Handle<Value> CalculateAsync(const Arguments& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  uv_work_t *req = new uv_work_t;
  AsyncData *asyncData = new AsyncData;
  req->data = asyncData;
  asyncData->points = args[0]->Uint32Value();
  Local<Function> callback = Local<Function>::Cast(args[1]);
  asyncData->callback = Persistent<Function>::New(isolate, callback);

  uv_queue_work(
      uv_default_loop()
    , req
    , AsyncWork
    , (uv_after_work_cb)AsyncAfter
  );

  return scope.Close(Undefined());
}

void InitAll(Handle<Object> exports) {
  exports->Set(String::NewSymbol("calculateSync"),
      FunctionTemplate::New(CalculateSync)->GetFunction());

  exports->Set(String::NewSymbol("calculateAsync"),
      FunctionTemplate::New(CalculateAsync)->GetFunction());
}

NODE_MODULE(addon, InitAll)
