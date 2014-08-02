/*
 * PdfWorker.h
 * Copyright (C) 2014 tox <tox@rootkit>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PDFWORKER_H
#define PDFWORKER_H

#include <nan.h>
#include <uv.h>
#include <list>
#include "../PdfController.h"


/**
 * @brief Base class for defining background processes
 */
template <class T>
class PdfWorker : public NanAsyncWorker {
private:
	T *_controller;
	uv_async_t *async;
	uv_mutex_t mutex;
	std::list<void *> intermediate;

	/**
	 * @brief handles intermediate events
	 *
	 * receives an uv_async_t handle from the background process.
	 * runs on main thread
	 */
	static void handleIntermediate(uv_async_t *handle) {
		PdfWorker *self = (PdfWorker *)handle->data;
		void *data;

		while(true) {
			uv_mutex_lock(&self->mutex);
			if(self->intermediate.empty()) {
				uv_mutex_unlock(&self->mutex);
				break;
			} else {
				data = self->intermediate.front();
				self->intermediate.pop_front();
				uv_mutex_unlock(&self->mutex);
			}
			self->HandleIntermediate(data);
		}
	}

	/**
	 * @brief deletes async handle
	 *
	 * the deconstructor trys to close the async handle asynchronously. When
	 * closing is done, this method is called.
	 */
	static void deleteAsync(uv_handle_t* handle) {
		// handle points to the same location as this->async. but at this point
		// the object is already freed, so we only dereference the handle here.
		delete (uv_async_t *)handle;
	}

public:
	PdfWorker(T *controller, NanCallback *callback) : NanAsyncWorker(callback) {
		_controller = controller;

		if(callback) {
			uv_loop_t *loop = uv_default_loop();
			this->async = new uv_async_t;
			uv_async_init(loop, this->async, reinterpret_cast<uv_async_cb>(handleIntermediate));
			this->async->data = this;
			uv_mutex_init(&mutex);
		}
	}

	~PdfWorker() {
		if(this->callback == NULL)
			return;
		uv_close((uv_handle_t*)this->async, deleteAsync);
		uv_mutex_destroy(&this->mutex);
		this->async = NULL;
	}

	T *controller() {
		return _controller;
	}

	void CallIntermediate(void *data) {
		uv_mutex_lock(&this->mutex);
		intermediate.push_back(data);
		uv_mutex_unlock(&this->mutex);
	}

	virtual void HandleIntermediate(void *data) {}
};

#endif /* !PDFWORKER_H */
