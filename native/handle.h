#ifndef __HANDLE_H__
#define __HANDLE_H__

#include "base.h"
#include "callback.h"

namespace native
{
	namespace detail
	{
		void delete_handle(uv_handle_t* h);

		class handle
		{
		public:
			template<typename T>
			handle(T* x)
				: uv_handle_(reinterpret_cast<uv_handle_t*>(x))
			{
				assert(uv_handle_);

				uv_handle_->data = new callbacks(uv_cid_max);
				assert(uv_handle_->data);
			}

			virtual ~handle()
			{
				uv_handle_ = nullptr;
			}

			handle(const handle& h)
				: uv_handle_(h.uv_handle_)
			{
			}

		public:
			template<typename T=uv_handle_t>
			T* get() { return reinterpret_cast<T*>(uv_handle_); }

			template<typename T=uv_handle_t>
			const T* get() const { return reinterpret_cast<T*>(uv_handle_); }

			bool is_active() { return uv_is_active(get()) != 0; }

			void close()
			{
			    uv_close(get(), [](uv_handle_t* h) { delete_handle(h); });
			}

			void close(std::function<void()> callback)
			{
				callbacks::store(get()->data, uv_cid_close, callback);
				uv_close(get(),
					[](uv_handle_t* h) {
						callbacks::invoke<decltype(callback)>(h->data, uv_cid_close);
						delete_handle(h);
					});
			}

			handle& operator =(const handle& h)
			{
				uv_handle_ = h.uv_handle_;
				return *this;
			}

		protected:
			uv_handle_t* uv_handle_;
		};

		void delete_handle(uv_handle_t* h)
		{
			assert(h);

			// clean up SCM
			if(h->data)
			{
				delete reinterpret_cast<callbacks*>(h->data);
				h->data = nullptr;
			}

			switch(h->type)
			{
				case UV_TCP: delete reinterpret_cast<uv_tcp_t*>(h); break;
				default: assert(0); break;
			}
		}
	}
}

#endif
