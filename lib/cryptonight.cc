// Copyright (c) 2017 ExcitableAardvark <excitableaardvark@tutanota.de>
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <nan.h>
#include <node_buffer.h>

#include "vendor/hash.h"

using namespace crypto;
using namespace Nan;
using namespace node;
using namespace v8;

NAN_METHOD(Hash) {
	Local<Object> buf;
	uint32_t variant = 0;
	uint64_t height = 0;

	switch (info.Length()) {
		case 3:
			if (info[2]->IsUint32())
			  height = info[2]->Int32Value(Nan::GetCurrentContext()).ToChecked();
			else
				return Nan::ThrowError("Third argument must be a number");			
		case 2:
			if (info[1]->IsUint32()) {
				variant = info[1]->Int32Value(Nan::GetCurrentContext()).ToChecked();
				if (variant >= 4 && info.Length() == 2) {
					return Nan::ThrowError("When variant is 4 or higher, height is required.");
				}
			} else {
				return Nan::ThrowError("Second argument must be a number");
			}
		case 1:
			buf = info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked();
			if (!Buffer::HasInstance(buf))
				return Nan::ThrowError("The first argument must be a buffer");
			if (variant > 0 && Buffer::Length(buf) < 43)
				return Nan::ThrowError("Buffer must be at least 43 bytes when variant > 0 is defined");
			break;
		case 0:
			return Nan::ThrowError("You must provide at least a buffer");
		default:
			return Nan::ThrowError("Invalid number of arguments.");
	}

	hash x;

	cn_slow_hash(Buffer::Data(buf), Buffer::Length(buf), x, variant, height);

	info.GetReturnValue().Set(CopyBuffer(x.data, 32).ToLocalChecked());
}

class CnWorker : public AsyncWorker {
public:
	CnWorker(Callback *callback, char *data, int length, int variant, int height)
		 : AsyncWorker(callback), data(data), length(length), variant(variant), height(height) {}
	~CnWorker() {}

	void Execute() {
		cn_slow_hash(data, length, hresult, variant, height);
	}

	void HandleOKCallback() {
		Local<Value> argv[] = {
			CopyBuffer(hresult.data, 32).ToLocalChecked()
		};

		callback->Call(1, argv);
	}

private:
	char *data;
	int length;
	int variant;
	int height;
	hash hresult;
};

NAN_METHOD(AsyncHash) {
	Local<Object> buf;
	uint32_t variant = 0;
	uint64_t height = 0;
	Callback *callback;

	switch (info.Length()) {
		case 4:
			if (info[3]->IsFunction())
				callback = new Callback(To<Function>(info[3]).ToLocalChecked());
			else
				return Nan::ThrowError("Fourth argument must be a callback");
		case 3:
			if (info.Length() >= 4) {
				if (info[2]->IsUint32())
					height = info[2]->Int32Value(Nan::GetCurrentContext()).ToChecked();
				else
					return Nan::ThrowError("Third argument must be a number");
			} else {
				if (info[2]->IsFunction())
					callback = new Callback(To<Function>(info[2]).ToLocalChecked());
				else
					return Nan::ThrowError("Third argument must be a callback");
			}
		case 2:
			if (info.Length() >= 3) {
				if (info[1]->IsUint32()) {
					variant = info[1]->Int32Value(Nan::GetCurrentContext()).ToChecked();
					if (variant >= 4 && info.Length() == 3) {
						return Nan::ThrowError("When variant is 4 or higher, height is required.");
					}
				} else {
					return Nan::ThrowError("Second argument must be a number");
				}
			} else {
				callback = new Callback(To<Function>(info[1]).ToLocalChecked());
			}
			buf = info[0]->ToObject(Nan::GetCurrentContext()).ToLocalChecked();
			if (!Buffer::HasInstance(buf))
				return Nan::ThrowError("The first argument must be a buffer");
			if (variant > 0 && Buffer::Length(buf) < 43)
				return Nan::ThrowError("Buffer must be at least 43 bytes when variant > 0 is defined");
			break;
		case 1:
		case 0:
			return Nan::ThrowError("You must provide at least a buffer and a callback");
		default:
			return Nan::ThrowError("Invalid number of arguments.");
	}

	char *data = Buffer::Data(buf);
	int len = Buffer::Length(buf);



	AsyncQueueWorker(new CnWorker(callback, data, len, variant, height));
}

NAN_MODULE_INIT(CnInit) {
	Nan::Set(target, New<String>("hash").ToLocalChecked(),
		GetFunction(New<FunctionTemplate>(Hash)).ToLocalChecked());
	Nan::Set(target, New<String>("asyncHash").ToLocalChecked(),
		GetFunction(New<FunctionTemplate>(AsyncHash)).ToLocalChecked());
}

NODE_MODULE(cryptonight, CnInit)
