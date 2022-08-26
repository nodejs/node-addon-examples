#include <node_api.h>
#include <initializer_list>
#include <utility>

// Empty value so that macros here are able to return NULL or void
#define NODE_API_RETVAL_NOTHING  // Intentionally blank #define

#define GET_AND_THROW_LAST_ERROR(env)                                          \
  do {                                                                         \
    const napi_extended_error_info* error_info;                                \
    napi_get_last_error_info((env), &error_info);                              \
    bool is_pending;                                                           \
    const char* err_message = error_info->error_message;                       \
    napi_is_exception_pending((env), &is_pending);                             \
    /* If an exception is already pending, don't rethrow it */                 \
    if (!is_pending) {                                                         \
      const char* error_message =                                              \
          err_message != NULL ? err_message : "empty error message";           \
      napi_throw_error((env), NULL, error_message);                            \
    }                                                                          \
  } while (0)

#define NODE_API_ASSERT_BASE(env, assertion, message, ret_val)                 \
  do {                                                                         \
    if (!(assertion)) {                                                        \
      napi_throw_error(                                                        \
          (env), NULL, "assertion (" #assertion ") failed: " message);         \
      return ret_val;                                                          \
    }                                                                          \
  } while (0)

// Returns NULL on failed assertion.
// This is meant to be used inside napi_callback methods.
#define NODE_API_ASSERT(env, assertion, message)                               \
  NODE_API_ASSERT_BASE(env, assertion, message, NULL)

// // Returns empty on failed assertion.
// // This is meant to be used inside functions with void return type.
// #define NODE_API_ASSERT_RETURN_VOID(env, assertion, message)             \
//   NODE_API_ASSERT_BASE(env, assertion, message, NODE_API_RETVAL_NOTHING)

#define NODE_API_CALL_BASE(env, the_call, ret_val)                             \
  do {                                                                         \
    if ((the_call) != napi_ok) {                                               \
      GET_AND_THROW_LAST_ERROR((env));                                         \
      return ret_val;                                                          \
    }                                                                          \
  } while (0)

// Returns NULL if the_call doesn't return napi_ok.
#define NODE_API_CALL(env, the_call) NODE_API_CALL_BASE(env, the_call, NULL)

// Returns empty if the_call doesn't return napi_ok.
#define NODE_API_CALL_RETURN_VOID(env, the_call)                               \
  NODE_API_CALL_BASE(env, the_call, NODE_API_RETVAL_NOTHING)

#define CHECK_NAPI(...)                                                        \
  do {                                                                         \
    napi_status res__ = (__VA_ARGS__);                                         \
    if (res__ != napi_ok) {                                                    \
      return res__;                                                            \
    }                                                                          \
  } while (0)

#define NAPI_CALL(expr) NODE_API_CALL(env, expr);

#ifdef __cpp_lib_span
#include <span>
using std::span;
#else
/**
 * @brief A span of values that can be used to pass arguments to function.
 *
 * For C++20 we should consider to replace it with std::span.
 */
template <typename T>
struct span {
  constexpr span(std::initializer_list<T> il) noexcept
      : data_{const_cast<T*>(il.begin())}, size_{il.size()} {}
  constexpr span(T* data, size_t size) noexcept : data_{data}, size_{size} {}
  [[nodiscard]] constexpr T* data() const noexcept { return data_; }
  [[nodiscard]] constexpr size_t size() const noexcept { return size_; }
  [[nodiscard]] constexpr T* begin() const noexcept { return data_; }
  [[nodiscard]] constexpr T* end() const noexcept { return data_ + size_; }
  const T& operator[](size_t index) const noexcept { return *(data_ + index); }

 private:
  T* data_;
  size_t size_;
};
#endif  // __cpp_lib_span

struct RefHolder {
  RefHolder(std::nullptr_t = nullptr) noexcept {}
  explicit RefHolder(napi_env env, napi_value value) : env_(env) {
    // Start with 2 to avoid ever going to 0 that creates a weak ref.
    napi_create_reference(env, value, 2, &ref_);
  }

  // The class is movable.
  RefHolder(RefHolder&& other) noexcept
      : env_(std::exchange(other.env_, nullptr)),
        ref_(std::exchange(other.ref_, nullptr)) {}

  RefHolder& operator=(RefHolder&& other) noexcept {
    if (this != &other) {
      swap(*this, other);
      RefHolder temp(std::move(other));
    }
    return *this;
  }

  // The class is not copyable.
  RefHolder(const RefHolder& other) = delete;
  RefHolder& operator=(const RefHolder& other) = delete;

  ~RefHolder() noexcept {
    if (env_ != nullptr && ref_ != nullptr) {
      uint32_t refCount{};
      napi_reference_unref(env_, ref_, &refCount);
      if (refCount == 1) {
        napi_delete_reference(env_, ref_);
      }
    }
  }

  operator napi_value() const {
    napi_value result{};
    if (ref_ != nullptr) {
      napi_get_reference_value(env_, ref_, &result);
    }
    return result;
  }

  explicit operator bool() const noexcept { return ref_ != nullptr; }

  friend void swap(RefHolder& left, RefHolder& right) noexcept {
    using std::swap;
    swap(left.env_, right.env_);
    swap(left.ref_, right.ref_);
  }

 private:
  napi_env env_{};
  napi_ref ref_{};
};
