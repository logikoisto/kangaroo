#ifndef _ZOO_KANGAROO_LOGBUFFER_H__
#define _ZOO_KANGAROO_LOGBUFFER_H__

namespace zoo
{
  namespace kangaroo
  {

    template <int size>
    class FixedBuffer
    {

    public:
      FixedBuffer()
          : cur_(data_)
      {
        setCookie(cookieStart);
      }

      ~FixedBuffer()
      {
        setCookie(cookieEnd);
      }

      void append(const char * /*restrict*/ buf, size_t len)
      {
        // FIXME: append partially
        if (implicit_cast<size_t>(avail()) > len)
        {
          memcpy(cur_, buf, len);
          cur_ += len;
        }
      }

      const char *data() const { return data_; }
      int length() const { return static_cast<int>(cur_ - data_); }

      // write to data_ directly
      char *current() { return cur_; }
      int avail() const { return static_cast<int>(end() - cur_); }
      void add(size_t len) { cur_ += len; }

      void reset() { cur_ = data_; }
      void bzero() { ::bzero(data_, sizeof data_); }

      // for used by GDB
      const char *debugString();
      void setCookie(void (*cookie)()) { cookie_ = cookie; }
      // for used by unit test
      string toString() const { return string(data_, length()); }

    private:
      const char *end() const { return data_ + sizeof data_; }
      // Must be outline function for cookies.
      static void cookieStart();
      static void cookieEnd();

      void (*cookie_)();
      char data_[size];
      char *cur_;
    };

    

  } // namespace kangaroo
} // namespace zoo
#endif