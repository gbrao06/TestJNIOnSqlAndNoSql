#pragma once
class V8Context
{
public:
	V8Context();
	~V8Context();
};

/*
2
Copyright (C) 2008 TPSi. All Rights Reserved.
3
Alex Iskander
4

5
Redistribution and use in source and binary forms, with or without
6
modification, are permitted provided that the following conditions
7
are met:
8
1. Redistributions of source code must retain the above copyright
9
notice, this list of conditions and the following disclaimer.
10
2. Redistributions in binary form must reproduce the above copyright
11
notice, this list of conditions and the following disclaimer in the
12
documentation and/or other materials provided with the distribution.
13
3. Neither the name of Technical Programming Services, Inc. ("TPSi") nor the
14
names of its contributors may be used to endorse or promote products
15
derived from this software without specific prior written permission.
16

17
THIS SOFTWARE IS PROVIDED BY TPSi ``AS IS'' AND ANY
18
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
19
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
20
PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL TPSi OR
21
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
22
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
23
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
24
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
25
OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
26
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
27
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
28

29
*/

/*
30
31
1 by Alex Iskander
Initial Commit.
32
#include "V8Context.h"
33
31.1.2 by Alex Iskander
Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
34
#include "../../SObjectConverters.h"
35
#include "../../Objects/SNumber.h"
36
#include "../../Objects/SString.h"
37
#include "../../Objects/SUndefined.h"
82 by Alex Iskander
CHANGE : Instead of Selector Results returning temporary objects,
	38
#include "../../Objects/SError.h"
	31.1.2 by Alex Iskander
	Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
	39
#include "../../Environments/Native/Objects/SNativeObject.h"
	40
#include "Objects/V8Function.h"
	40 by Alex Iskander
	A bug fix regarding the Native / v8 object map, and a couple scriptable components.
	41
#include "Objects/V8Object.h"
	31.1.4 by Alex Iskander
	Changes to Bindable, and further progress in Script Engine.
	42
#include "../../Contexts/GlobalContext.h"
	1 by Alex Iskander
	Initial Commit.
	43
	82 by Alex Iskander
	CHANGE : Instead of Selector Results returning temporary objects,
	44
#include <sstream>
	45
	1 by Alex Iskander
	Initial Commit.
	46
	using namespace v8;
47
using namespace Script;
48
49
V8Context::V8Context()
50
{
	31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		51
		//open a handle scope
		1 by Alex Iskander
		Initial Commit.
		52
		HandleScope handle_scope;
	53

		31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		54
		//create a context
		55
		v8::Handle<v8::Context> context = v8::Context::New();
	56

		57
		//load it into the persistent handle
		58
		this->context = v8::Persistent<v8::Context>::New(context);
	59

		124 by Alex Iskander
		Closer and closer...
		60
		//create a new context scope.
		61
		v8::Context::Scope context_scope(context);
	62

		63
		//set "global" object
		64
		this->context->Global()->Set(v8::String::New("global"), this->context->Global());
	31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		65
}
66
67
v8::Handle<v8::Context> V8Context::getContext()
68
{
	69
		return this->context;
	1 by Alex Iskander
		Initial Commit.
		70
}
71
41 by Alex Iskander
Changed tons of files so that we deal almost completely in smart pointers throughout the Create framework.Note that there are likely some memory leaks due to circular references now; I removed the worst of them, but some still need to be addressed.In brief : pointers to parents should be normal pointers.
72
SObject V8Context::runScript(
	73
	std::string scriptSource,
	74
	std::string fromFile,
	75
	int line,
	76
	int column
	77
)
1 by Alex Iskander
Initial Commit.
78
{
	79
		HandleScope handle_scope;
	80

		41 by Alex Iskander
		Changed tons of files so that we deal almost completely in smart pointers throughout the Create framework.Note that there are likely some memory leaks due to circular references now; I removed the worst of them, but some still need to be addressed.In brief : pointers to parents should be normal pointers.
		81
		TryCatch try_catch;
	82

		31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		83
		Handle<v8::Context> context = this->context;
	1 by Alex Iskander
		Initial Commit.
		84
		//start a context scope
		31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		85
		v8::Context::Scope context_scope(context);
	1 by Alex Iskander
		Initial Commit.
		86

		87
		//read in source
		88
		Handle<String> source = String::New(scriptSource.data());
	89

		41 by Alex Iskander
		Changed tons of files so that we deal almost completely in smart pointers throughout the Create framework.Note that there are likely some memory leaks due to circular references now; I removed the worst of them, but some still need to be addressed.In brief : pointers to parents should be normal pointers.
		90
		//create a script origin
		91
		ScriptOrigin *origin = new ScriptOrigin(
			92
			String::New(fromFile.data()),
			93
			Integer::New(line),
			94
			Integer::New(column)
			95
		);
	96

		97

		1 by Alex Iskander
		Initial Commit.
		98
		//create a script
		41 by Alex Iskander
		Changed tons of files so that we deal almost completely in smart pointers throughout the Create framework.Note that there are likely some memory leaks due to circular references now; I removed the worst of them, but some still need to be addressed.In brief : pointers to parents should be normal pointers.
		99
		Handle<v8::Script> script = v8::Script::Compile(source, origin);
	100
		if (script.IsEmpty())
			101
		{
			102
				if (try_catch.HasCaught())
					103
				{
					82 by Alex Iskander
						CHANGE : Instead of Selector Results returning temporary objects,
						104
						//get variables
						41 by Alex Iskander
						Changed tons of files so that we deal almost completely in smart pointers throughout the Create framework.Note that there are likely some memory leaks due to circular references now; I removed the worst of them, but some still need to be addressed.In brief : pointers to parents should be normal pointers.
						105
						SObject message = wrap(try_catch.Message()->Get());
					106
						SObject line = wrap(try_catch.Message()->GetSourceLine());
					107
						SObject res = wrap(try_catch.Message()->GetScriptResourceName());
					108

						82 by Alex Iskander
						CHANGE : Instead of Selector Results returning temporary objects,
						109
						//create error
						110
						SError error = new SErrorObject(
							111
							"JavaScript Error",
							112
							message->stringValue(),
							113
							line->numberValue()
							114
						);
					115

						116
						//return error
						117
						return error;
					41 by Alex Iskander
						Changed tons of files so that we deal almost completely in smart pointers throughout the Create framework.Note that there are likely some memory leaks due to circular references now; I removed the worst of them, but some still need to be addressed.In brief : pointers to parents should be normal pointers.
						118
				}
			119

				120
				return SUndefined();
			121
		}
	122

		123

		124
		//destroy origin
		125
		delete origin;
	1 by Alex Iskander
		Initial Commit.
		126

		127
		//run script
		128
		Handle<Value> result = script->Run();
	129

		41 by Alex Iskander
		Changed tons of files so that we deal almost completely in smart pointers throughout the Create framework.Note that there are likely some memory leaks due to circular references now; I removed the worst of them, but some still need to be addressed.In brief : pointers to parents should be normal pointers.
		130
		if (result.IsEmpty())
			131
		{
			132
				if (try_catch.HasCaught())
					133
				{
					134
						SObject message = wrap(try_catch.Message()->Get());
					135
						SObject line = wrap(try_catch.Message()->GetSourceLine());
					136
						SObject res = wrap(try_catch.Message()->GetScriptResourceName());
					137

						138
						std::cout << "Crude Error Handling: "
						139
						<< ((int)(line->numberValue() + .5))
						140
						<< ": " << message->stringValue()
						141
						<< "\n In: " << res->stringValue();
					142
				}
			143

				144
				return SUndefined();
			145
		}
	146

		1 by Alex Iskander
		Initial Commit.
		147
		//wrap result
		148
		return this->wrap(result);
	149
}
150
151
SObject V8Context::wrap(v8::Handle<v8::Value> value)
152
{
	31.1.4 by Alex Iskander
		Changes to Bindable, and further progress in Script Engine.
		153
		if (value.IsEmpty())
			154
			return SUndefined();
	155

		31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		156
		//if it is a number, it is really simple.
		1 by Alex Iskander
		Initial Commit.
		157
		if (value->IsNumber())
			158
		{
			31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				159
				return Scriptable(value->NumberValue());
			1 by Alex Iskander
				Initial Commit.
				160
		}
	31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		161

		41 by Alex Iskander
		Changed tons of files so that we deal almost completely in smart pointers throughout the Create framework.Note that there are likely some memory leaks due to circular references now; I removed the worst of them, but some still need to be addressed.In brief : pointers to parents should be normal pointers.
		162
		else if (value->IsInt32())
		163
		{
			164
				return Scriptable((double)value->IntegerValue());
			165
		}
	166

		31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		167
		//if it is a string, it is still simple
		1 by Alex Iskander
		Initial Commit.
		168
		else if (value->IsString())
		169
		{
			76 by Alex Iskander
				BUGFIX : PDF was failing to write out decimal point(.) for numbers
				170
				return Scriptable(*v8::String::Utf8Value(value->ToString()));
			31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				171
		}
	172

		40 by Alex Iskander
		A bug fix regarding the Native / v8 object map, and a couple scriptable components.
		173
		//if it is a boolean, it is still still simple
		174
		else if (value->IsBoolean())
		175
		{
			176
				return Scriptable(value->BooleanValue());
			177
		}
	178

		31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		179
		//if it is a function, it is a bit less simple, but still pretty simple.
		180
		else if (value->IsFunction())
		181
		{
			40 by Alex Iskander
				A bug fix regarding the Native / v8 object map, and a couple scriptable components.
				182
				return new V8FunctionObject(this, v8::Handle<v8::Function>::Cast(value));
			31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				183
		}
	184

		82 by Alex Iskander
		CHANGE : Instead of Selector Results returning temporary objects,
		185
		//if it is an array, it is even less simple... but still doable
		186
		else if (value->IsArray())
		187
		{
			188
				v8::Handle<v8::Array> array = v8::Handle<v8::Array>::Cast(value);
			129 by Alex Iskander
				The Shell now does almost everything the engine does.It is just missing
				189
				long length = array->Length();
			190

				191
				SArray result = new SArrayObject();
			192
				for (int i = 0; i < length; i++)
					82 by Alex Iskander
					CHANGE : Instead of Selector Results returning temporary objects,
					193
				{
					129 by Alex Iskander
						The Shell now does almost everything the engine does.It is just missing
						194
						result->add(wrap(array->Get(v8::Integer::New(i))));
					82 by Alex Iskander
						CHANGE : Instead of Selector Results returning temporary objects,
						195
				}
			129 by Alex Iskander
				The Shell now does almost everything the engine does.It is just missing
				196
				return result;
			82 by Alex Iskander
				CHANGE : Instead of Selector Results returning temporary objects,
				197
		}
	198

		31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		199
		//if it is an object
		200
		else if (value->IsObject())
		201
		{
			202
				//start a handle scope
				203
				HandleScope handle_scope;
			204

				205
				//get the object
				206
				v8::Handle<v8::Object> object = value->ToObject();
			207

				208
				//see if it has an internal field
				209
				if (object->InternalFieldCount() > 0)
					210
				{
					211
						//if it does, get it
						212
						Local<Value> field = object->GetInternalField(0);
					213

						214
						//see if it is external
						215
						if (field->IsExternal())
							216
						{
							217
								//if it is, get the object
								218
								v8::Local<v8::External> object = v8::Local<v8::External>::Cast(field);
							219

								220
								//cast it to a native type
								31.1.4 by Alex Iskander
								Changes to Bindable, and further progress in Script Engine.
								221
								SNative *value = static_cast<SNative *> (object->Value());
							31.1.2 by Alex Iskander
								Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
								222

								223
								//return the native
								31.1.4 by Alex Iskander
								Changes to Bindable, and further progress in Script Engine.
								224
								return *value;
							31.1.2 by Alex Iskander
								Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
								225
						}
					226
				}
			40 by Alex Iskander
				A bug fix regarding the Native / v8 object map, and a couple scriptable components.
				227
				else
				228
				{
					229
						//a normal object. Create a V8Object.
						230
						return new V8ObjectBase(this, v8::Handle<v8::Object>::Cast(value));
					231
				}
			31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				232
		}
	233

		234
		//if we didn't recognize it, then return undefined.
		235
		return new SUndefinedObject();
	236
}
237
238
v8::Handle<v8::Value> V8Context::unwrap(SObject value)
239
{
	240
		v8::HandleScope handle_scope;
	31.1.6 by Alex Iskander
		The new script engine is implemented.
		241

		242
		//handle null values
		243
		if (!value)
			244
			return v8::Undefined();
	245

		31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		246
		//if it is a string, just make a v8 string.
		247
		if (value->isString())
			248
		{
			249
				return handle_scope.Close(v8::String::New(value->stringValue().data()));
			250
		}
	251

		252
		//if it is a number, make a v8 number
		253
		else if (value->isNumber())
		254
		{
			255
				return handle_scope.Close(v8::Number::New(value->numberValue()));
			256
		}
	257

		31.1.6 by Alex Iskander
		The new script engine is implemented.
		258
		//if it is a boolean, make a v8 boolean
		259
		else if (value->isBoolean())
		260
		{
			261
				return handle_scope.Close(v8::Boolean::New(value->booleanValue()));
			262
		}
	263

		264
		//if it is an array, make a v8 array
		265
		else if (value->isArray())
		266
		{
			82 by Alex Iskander
				CHANGE : Instead of Selector Results returning temporary objects,
				267
				//create an object
				268
				SArray array = value->arrayValue();
			269
				int length = array->length();
			270

				271
				v8::Handle<v8::Array> v8array = v8::Array::New(length);
			272
				for (int i = 0; i < length; i++)
					273
				{
					274
						v8array->Set(v8::Number::New(i), unwrap(array->get(i)));
					275
				}
			276

				277
				return handle_scope.Close(v8array);
			31.1.6 by Alex Iskander
				The new script engine is implemented.
				278
		}
	279

		31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		280
		//if it is a function, go through an ordeal
		281
		else if (value->isFunction())
		282
		{
			283
				// Prepare a function holder \
				284
				//create a function holder
				285
				FunctionHolder *holder = new FunctionHolder();
			286

				287
				//set the function
				288
				holder->function = value->functionValue();
			289

				290
				//set the context (to ourself!)
				291
				holder->context = this;
			292

				293
				//Prepare an External represetntation 
				31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				294
				//create a handle to the holder
				295
				v8::Handle<v8::External> functionObject = v8::External::New(holder);
			31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				296

				297
				 Prepare a function template 
				298
				//pass the functionObject (which wraps the function holder) so it knows
				299
				//what function to call.
				300
				v8::Local<v8::FunctionTemplate> functionTemplate =
				301
				v8::FunctionTemplate::New(functionCall, functionObject);
			302

				31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				303
				//get the function
				129 by Alex Iskander
				The Shell now does almost everything the engine does.It is just missing
				304
				v8::TryCatch try_catch;
			31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				305
				v8::Persistent<v8::Function> function =
				306
				v8::Persistent<v8::Function>::New(functionTemplate->GetFunction());
			307

				129 by Alex Iskander
				The Shell now does almost everything the engine does.It is just missing
				308
				if (function.IsEmpty())
					309
				{
					310
						throw "Could not create function.";
					311
				}
			312

				31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				313
				//make it weak
				314
				function.MakeWeak(holder, &FunctionHolderOutOfScope);
			315

				31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				316
				//return the function.
				31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				317
				return function;
			31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				318
		}
	43 by Alex Iskander
		More on Calendar Example.
		319
		else if (value->isObject())
		320
		{
			321
				//see if it is a v8 object
				322
				V8ObjectBase *object;
			323
				if (object = dynamic_cast<V8ObjectBase *> (*value))
					324
				{
					325
						return handle_scope.Close(object->getV8Object());
					326
				}
			327

				328

				329
				//otherwise, we cannot do this properly (perhaps someday?)
				330
		}
	31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		331
		else if (value->isNative())
		332
		{
			333
				//get the native
				334
				SNative native = value->nativeValue();
			335

				31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				336
				//see if we have it already
				337
				if (this->objects.count(native->getID()) > 0)
					338
					return this->objects[native->getID()];
			339

				31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				340
				//get the prototype
				31.1.4 by Alex Iskander
				Changes to Bindable, and further progress in Script Engine.
				341
				SPrototype prototype = native->getPrototype();
			342

				343
				//if the prototype is NULL, get and set
				344
				if (!prototype)
					345
				{
					41 by Alex Iskander
						Changed tons of files so that we deal almost completely in smart pointers throughout the Create framework.Note that there are likely some memory leaks due to circular references now; I removed the worst of them, but some still need to be addressed.In brief : pointers to parents should be normal pointers.
						346
						//set prototype
						31.1.4 by Alex Iskander
						Changes to Bindable, and further progress in Script Engine.
						347
						native->setPrototype(this->globalContext->getPrototype(native));
					41 by Alex Iskander
						Changed tons of files so that we deal almost completely in smart pointers throughout the Create framework.Note that there are likely some memory leaks due to circular references now; I removed the worst of them, but some still need to be addressed.In brief : pointers to parents should be normal pointers.
						348

						349
						//set our local prototype variable
						31.1.4 by Alex Iskander
						Changes to Bindable, and further progress in Script Engine.
						350
						prototype = native->getPrototype();
					41 by Alex Iskander
						Changed tons of files so that we deal almost completely in smart pointers throughout the Create framework.Note that there are likely some memory leaks due to circular references now; I removed the worst of them, but some still need to be addressed.In brief : pointers to parents should be normal pointers.
						351

						352
						//now that we know the prototype, we can call construct on native
						353
						prototype->construct(native);
					31.1.4 by Alex Iskander
						Changes to Bindable, and further progress in Script Engine.
						354
				}
			355
				356
				//return if no prototype
				357
				if (!prototype)
					358
					return v8::Undefined();
			359

				360

				361
				//get and wrap native
				362
				SNative *nptr = new SNative(native);
			363
				Handle<External> nativeWrapper = External::New(nptr);
			31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				364

				31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				365
				//create a holder
				366
				NativeHolder *holder = new NativeHolder();
			367
				holder->object = nptr;
			368
				holder->context = this;
			369

				31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				370
				 Do we need a new instance? 
				371
				//get the function template
				372
				v8::Handle<v8::FunctionTemplate> functionTemplate = getTemplate(prototype);
			31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				373
				v8::Handle<v8::Function> function = functionTemplate->GetFunction();
			31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				374

				375
				//get the object template
				376
				v8::Handle<v8::ObjectTemplate> objectTemplate = functionTemplate->InstanceTemplate();
			31.1.4 by Alex Iskander
				Changes to Bindable, and further progress in Script Engine.
				377
				Persistent<Object> object =
				31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				378
				Persistent<Object>::New(function->NewInstance());
			31.1.4 by Alex Iskander
				Changes to Bindable, and further progress in Script Engine.
				379

				380
				//make weak
				31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				381
				object.MakeWeak(holder, &ObjectOutOfScope);
			31.1.4 by Alex Iskander
				Changes to Bindable, and further progress in Script Engine.
				382

				31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				383
				//prepare object
				31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				384
				object->SetInternalField(0, nativeWrapper);
			31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				385

				386
				//add to object map
				387
				this->objects[native->getID()] = object;
			40 by Alex Iskander
				A bug fix regarding the Native / v8 object map, and a couple scriptable components.
				388
				native->alert(this);
			31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				389

				390
				//add to V8 vm size
				391
				v8::V8::AdjustAmountOfExternalAllocatedMemory(
					55 by Alex Iskander
					Changelog :
			392
				(sizeof(SNative) +
					31.1.5 by Alex Iskander
					Script engine is almost feature complete\!
					393
					sizeof(SNativeObject) +
					55 by Alex Iskander
					Changelog :
			394
				native->estimateNativeSize()) * 1024
				31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				395
				);
			396

				397
				//return it
				398
				return object;
			31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				399
		}
	400

		401
		//if we couldn't do anything, return undefined.
		402
		return v8::Undefined();
	403
}
404
405
v8::Handle<v8::FunctionTemplate> V8Context::getTemplate(SPrototype prototype)
406
{
	407
		v8::HandleScope handle_scope;
	408
		if (this->templates.count(prototype) > 0)
			409
		{
			410
				return this->templates[prototype];
			411
		}
	412
		else
		413
		{
			414
				v8::Handle<v8::FunctionTemplate> f = v8::FunctionTemplate::New();
			41 by Alex Iskander
				Changed tons of files so that we deal almost completely in smart pointers throughout the Create framework.Note that there are likely some memory leaks due to circular references now; I removed the worst of them, but some still need to be addressed.In brief : pointers to parents should be normal pointers.
				415

				416
				v8::Handle<v8::ObjectTemplate> p = f->PrototypeTemplate();
			31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				417
				v8::Handle<v8::ObjectTemplate> i = f->InstanceTemplate();
			418
				i->SetInternalFieldCount(1);
			419

				31.1.4 by Alex Iskander
				Changes to Bindable, and further progress in Script Engine.
				420
				//class name
				421
				f->SetClassName(v8::String::New(prototype->getClassName().data()));
			422

				31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				423
				//load prototype
				424
				std::map<std::string, SObject> &fields = prototype->getFields();
			425
				std::map<std::string, SObject>::iterator key;
			426
				for (key = fields.begin(); key != fields.end(); key++)
					427
				{
					428
						std::string field = (*key).first;
					429
						SObject value = (*key).second;
					430

						31.1.5 by Alex Iskander
						Script engine is almost feature complete\!
						431
						//if it is a property, do it the harder way
						432
						if (value->isProperty())
							433
						{
							434
								PropertyHolder *holder = new PropertyHolder();
							435
								holder->property = value->propertyValue();
							436
								holder->context = this;
							437
								v8::Handle<v8::Value> property = v8::External::New(holder);
							438

								439
								i->SetAccessor(
									440
									v8::String::New(field.data()),
									441
									GetterCall,
									442
									SetterCall,
									443
									property
									444
								);
							445
						}
					446

						447
						//do it the easy way otherwise
						448
						else
						449
						{
							41 by Alex Iskander
								Changed tons of files so that we deal almost completely in smart pointers throughout the Create framework.Note that there are likely some memory leaks due to circular references now; I removed the worst of them, but some still need to be addressed.In brief : pointers to parents should be normal pointers.
								450
								//add functions to the prototype...
								451
								if (value->isFunction())
									452
								{
									453
										p->Set(v8::String::New(field.data()), unwrap(value));
									454
								}
							455

								456
								//add everything else to instance...
								457
								else
								458
								{
									459
										i->Set(v8::String::New(field.data()), unwrap(value));
									460
								}
							461
								31.1.5 by Alex Iskander
								Script engine is almost feature complete\!
								462
						}
					31.1.4 by Alex Iskander
						Changes to Bindable, and further progress in Script Engine.
						463
				}
			464

				31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				465
				this->templates[prototype] = v8::Persistent<v8::FunctionTemplate>::New(f);
			466
				return handle_scope.Close(f);
			467
		}
	468
}
469
31.1.4 by Alex Iskander
Changes to Bindable, and further progress in Script Engine.
470
void V8Context::addConstructor(std::string name, SPrototype prototype)
471
{
	31.1.5 by Alex Iskander
		Script engine is almost feature complete\!
		472
		if (!prototype->isConstructable())
			473
			return;
	474

		31.1.4 by Alex Iskander
		Changes to Bindable, and further progress in Script Engine.
		475
		v8::HandleScope handle_scope;
	476

		477
		//get context
		478
		Handle<v8::Context> context = this->context;
	479

		480
		//start a context scope
		481
		v8::Context::Scope context_scope(context);
	482

		483
		//get global
		484
		v8::Handle<v8::Object> global = context->Global();
	31.1.5 by Alex Iskander
		Script engine is almost feature complete\!
		485
		486
		//create a holder
		487
		PrototypeHolder *holder = new PrototypeHolder();
	488
		holder->context = this;
	489
		holder->prototype = prototype;
	490

		491
		v8::Persistent<v8::External> prototypeObject =
		492
		v8::Persistent<v8::External>::New(v8::External::New(holder));
	493
		prototypeObject.MakeWeak(holder, PrototypeHolderOutOfScope);
	494

		495
		//create function template
		496
		v8::Handle<v8::FunctionTemplate> function =
		497
		v8::FunctionTemplate::New(constructorCall, prototypeObject);
	498

		124 by Alex Iskander
		Closer and closer...
		499
		//set _is_native
		500
		v8::Local<v8::Function> func = function->GetFunction();
	501
		func->Set(v8::String::New("_is_native"), v8::Boolean::New(true));
	31.1.4 by Alex Iskander
		Changes to Bindable, and further progress in Script Engine.
		502

		503
		//set
		124 by Alex Iskander
		Closer and closer...
		504
		global->Set(v8::String::New(name.data()), func);
	31.1.4 by Alex Iskander
		Changes to Bindable, and further progress in Script Engine.
		505
}
506
31.1.2 by Alex Iskander
Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
507
void V8Context::FunctionHolderOutOfScope(
	508
	v8::Persistent<v8::Value> object,
	509
	void *parameter
	510
)
511
{
	512
		//dispose the object to get rid of that memory...
		513
		object.Dispose();
	31.1.5 by Alex Iskander
		Script engine is almost feature complete\!
		514
		object.Clear();
	31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		515

		516
		//and destroy the function holder
		517
		delete static_cast<FunctionHolder *> (parameter);
	518
}
31.1.4 by Alex Iskander
Changes to Bindable, and further progress in Script Engine.
519
31.1.5 by Alex Iskander
Script engine is almost feature complete\!
520
void V8Context::PrototypeHolderOutOfScope(
	521
	v8::Persistent<v8::Value> object,
	522
	void *parameter
	523
)
524
{
	525
		PrototypeHolder *holder = static_cast<PrototypeHolder *> (parameter);
	526
		delete holder;
	527

		528
		object.Dispose();
	529
		object.Clear();
	530
}
531
31.1.4 by Alex Iskander
Changes to Bindable, and further progress in Script Engine.
532
void V8Context::ObjectOutOfScope(
	533
	v8::Persistent<v8::Value> object,
	534
	void *parameter
	535
)
536
{
	31.1.5 by Alex Iskander
		Script engine is almost feature complete\!
		537
		NativeHolder *holder = static_cast<NativeHolder *> (parameter);
	538
		SNative *native = holder->object;
	539

		540
		//remove from object map
		541
		if (holder->context->objects.count((*native)->getID()) > 0)
			40 by Alex Iskander
			A bug fix regarding the Native / v8 object map, and a couple scriptable components.
			542
		{
			31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				543
				holder->context->objects.erase((*native)->getID());
			40 by Alex Iskander
				A bug fix regarding the Native / v8 object map, and a couple scriptable components.
				544
				(*native)->stopAlert(holder->context);
			545
		}
	546
		31.1.5 by Alex Iskander
		Script engine is almost feature complete\!
		547

		548

		31.1.4 by Alex Iskander
		Changes to Bindable, and further progress in Script Engine.
		549
		//dispose the object to get rid of that memory...
		550
		object.Dispose();
	551
		object.Clear();
	552

		31.1.5 by Alex Iskander
		Script engine is almost feature complete\!
		553
		//and destroy the native and its holder
		554
		delete native;
	555
		delete holder;
	556

		31.1.4 by Alex Iskander
		Changes to Bindable, and further progress in Script Engine.
		557

		558
		//adjust v8 memory measurement -- we don't actually know the size, do we?
		55 by Alex Iskander
		Changelog :
	559
		V8::AdjustAmountOfExternalAllocatedMemory((0 - sizeof(SNativeObject) - sizeof(SPrototypeObject)) * 1024);
	31.1.4 by Alex Iskander
		Changes to Bindable, and further progress in Script Engine.
		560
}
31.1.2 by Alex Iskander
Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
561

562
v8::Handle<v8::Value> V8Context::functionCall(const Arguments &args)
563
{
	564
		v8::HandleScope handle_scope;
	565
		//extract the function object
		566
		Local<v8::External> functionObject = Local<External>::Cast(args.Data());
	567

		568
		//and from it, the function holder
		569
		FunctionHolder *holder =
		570
		static_cast<FunctionHolder *>(functionObject->Value());
	571

		572
		//get the function from the holder
		573
		SFunction function = holder->function;
	574

		575
		//prepare an arguments array
		31.1.5 by Alex Iskander
		Script engine is almost feature complete\!
		576
		SArguments relay;
	31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		577

		578
		//loop through arguments
		579
		for (int i = 0; i < args.Length(); i++)
			580
		{
			581
				//wrap and add the argument to the arguments array
				31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				582
				relay.add(holder->context->wrap(args[i]));
			31.1.2 by Alex Iskander
				Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
				583
		}
	584

		31.1.5 by Alex Iskander
		Script engine is almost feature complete\!
		585
		//get this
		586
		SObject self = holder->context->wrap(args.This());
	587

		31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		588
		//return an unwrapped result
		126 by Alex Iskander
		Now you can make custom dC directives from JavaScript!
		589
		v8::Handle<v8::Value> result;
	590
		try
		591
	{
		592
			result = holder->context->unwrap(function->apply(self, relay));
		593
	}
	594
		//a thrown error should be an SObject
		595
		catch (SObject &object)
		596
	{
		597
			result = v8::ThrowException(holder->context->unwrap(object));
		598
	}
	599

		600

		601
		return handle_scope.Close(result);
	31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		602
}
603
31.1.4 by Alex Iskander
Changes to Bindable, and further progress in Script Engine.
604
v8::Handle<v8::Value> V8Context::constructorCall(const Arguments &args)
605
{
	606
		// throw if called without `new'
		607
		if (!args.IsConstructCall())
			608
			return ThrowException(String::New("Cannot call constructor as function"));
	609

		610
		v8::HandleScope handle_scope;
	611
		//extract the function object
		612
		Local<v8::External> prototypeObject = Local<External>::Cast(args.Data());
	613

		614
		//and from it, the function holder
		615
		PrototypeHolder *holder =
		616
		static_cast<PrototypeHolder *>(prototypeObject->Value());
	617

		618
		//get the function from the holder
		619
		SPrototype prototype = holder->prototype;
	620

		621
		//prepare an arguments array
		31.1.5 by Alex Iskander
		Script engine is almost feature complete\!
		622
		SArguments relay;
	31.1.4 by Alex Iskander
		Changes to Bindable, and further progress in Script Engine.
		623

		624
		//loop through arguments
		625
		for (int i = 0; i < args.Length(); i++)
			626
		{
			627
				//wrap and add the argument to the arguments array
				31.1.5 by Alex Iskander
				Script engine is almost feature complete\!
				628
				relay.add(holder->context->wrap(args[i]));
			31.1.4 by Alex Iskander
				Changes to Bindable, and further progress in Script Engine.
				629
		}
	630

		631
		//generate native
		632
		SNative native = prototype->create(relay);
	633

		634
		//set its prototype
		635
		native->setPrototype(prototype);
	636

		41 by Alex Iskander
		Changed tons of files so that we deal almost completely in smart pointers throughout the Create framework.Note that there are likely some memory leaks due to circular references now; I removed the worst of them, but some still need to be addressed.In brief : pointers to parents should be normal pointers.
		637
		//construct
		638
		prototype->construct(native);
	639

		31.1.5 by Alex Iskander
		Script engine is almost feature complete\!
		640
		return holder->context->unwrap(native);
	641

		642
}
643
644
v8::Handle<v8::Value> V8Context::GetterCall(
	645
	v8::Local<v8::String> name,
	646
	const v8::AccessorInfo &info
	647
)
648
{
	649
		v8::HandleScope handle_scope;
	650
		//extract the function object
		651
		Local<v8::External> propertyObject = Local<External>::Cast(info.Data());
	652

		653
		//and from it, the function holder
		654
		PropertyHolder *holder =
		655
		static_cast<PropertyHolder *>(propertyObject->Value());
	656

		657
		//get the function from the holder
		658
		SProperty property = holder->property;
	659

		660
		//get this
		661
		SObject self = holder->context->wrap(info.This());
	662
		31.1.4 by Alex Iskander
		Changes to Bindable, and further progress in Script Engine.
		663
		//return an unwrapped result
		82 by Alex Iskander
		CHANGE : Instead of Selector Results returning temporary objects,
		664
		SObject result = property->get(self);
	665
		return handle_scope.Close(holder->context->unwrap(result));
	31.1.5 by Alex Iskander
		Script engine is almost feature complete\!
		666
}
667
668
669
void V8Context::SetterCall(
	670
	v8::Local<v8::String> name,
	671
	v8::Local<v8::Value> value,
	672
	const v8::AccessorInfo &info
	673
)
674
{
	82 by Alex Iskander
		CHANGE : Instead of Selector Results returning temporary objects,
		675
		v8::TryCatch try_catch;
	31.1.5 by Alex Iskander
		Script engine is almost feature complete\!
		676
		v8::HandleScope handle_scope;
	82 by Alex Iskander
		CHANGE : Instead of Selector Results returning temporary objects,
		677

		31.1.5 by Alex Iskander
		Script engine is almost feature complete\!
		678
		//extract the function object
		679
		Local<v8::External> propertyObject = Local<External>::Cast(info.Data());
	680

		681
		//and from it, the function holder
		682
		PropertyHolder *holder =
		683
		static_cast<PropertyHolder *>(propertyObject->Value());
	684

		685
		//get the function from the holder
		686
		SProperty property = holder->property;
	687

		688
		//get this
		689
		SObject self = holder->context->wrap(info.This());
	690

		691
		//get that
		692
		SObject to = holder->context->wrap(value);
	693

		694
		//set
		695
		property->set(self, to);
	696
}
697
698
31.1.4 by Alex Iskander
Changes to Bindable, and further progress in Script Engine.
699
31.1.2 by Alex Iskander
Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
700
SObject V8Context::get(std::string key)
701
{
	702
		v8::HandleScope handle_scope;
	703
		704
		//get context
		705
		Handle<v8::Context> context = this->context;
	706

		707
		//start a context scope
		708
		v8::Context::Scope context_scope(context);
	709

		710
		//get global
		711
		v8::Handle<v8::Object> global = context->Global();
	712

		713
		//get value, if any
		714
		v8::Handle<v8::Value> value = global->Get(v8::String::New(key.data()));
	715

		716
		//if we had no errors
		717
		if (!value.IsEmpty())
			718
		{
			719
				//return wrapped
				720
				return wrap(value);
			721
		}
	722

		723
		//return undefined
		724
		return new SUndefinedObject();
	725
}
726
727
void V8Context::set(std::string key, SObject value)
728
{
	31.1.5 by Alex Iskander
		Script engine is almost feature complete\!
		729
		//if it is not a property, use normal set
		730
		if (!value->isProperty())
			731
		{
			732
				v8::HandleScope handle_scope;
			733

				734
				//get context
				735
				Handle<v8::Context> context = this->context;
			736

				737
				//start a context scope
				738
				v8::Context::Scope context_scope(context);
			739

				740
				//get global
				741
				v8::Handle<v8::Object> global = context->Global();
			742

				743
				//set
				744
				global->Set(v8::String::New(key.data()), unwrap(value));
			745
		}
	746
		else
		747
		{
			748
				//use another function to do it.
				749
				this->addProperty(key, value->propertyValue());
			750
		}
	31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		751
		752
}
753
31.1.5 by Alex Iskander
Script engine is almost feature complete\!
754
void V8Context::addProperty(std::string key, SProperty property)
31.1.2 by Alex Iskander
Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
755
{
	31.1.5 by Alex Iskander
		Script engine is almost feature complete\!
		756
		 Guess what? IT IS NOT SUPPORTED!!! 
		31.1.2 by Alex Iskander
		Forgot to commit the changes.They aren't perfect (some renames turned into delete/adds) but it works. So, here's the first bits of the new script engine.
		757
}
758
759
v8::Handle<v8::Object> V8Context::getGlobalObject()
760
{
	761
		v8::HandleScope handle_scope;
	762

		763
		//get context
		764
		Handle<v8::Context> context = this->context;
	765

		766
		//start a context scope
		767
		v8::Context::Scope context_scope(context);
	768

		769
		//get global
		770
		v8::Handle<v8::Object> global = context->Global();
	771

		772
		//return
		773
		return global;
	774
}
775
40 by Alex Iskander
A bug fix regarding the Native / v8 object map, and a couple scriptable components.
776
void V8Context::nativeDestroyed(SNative native, void *id)
777
{
	778
		if (objects.count(id) > 0)
			779
			objects.erase(id);
	780
}
781
58 by Alex Iskander
NEW : Shell, a project that is currently just a consumer of Script, but
	782
	V8Context::~V8Context()
	783
{
	784
		this->context.Dispose();
	785
		this->context.Clear();
	786

}
787

*/