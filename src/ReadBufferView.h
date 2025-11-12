#ifndef _READ_BUFFER_VIEW_H_
#define _READ_BUFFER_VIEW_H_

#include "ReadBufferInterface.h"
#include <span>
#include <cstdint>

namespace EmbeddedProto
{

  //! Read buffer that wraps an external span of bytes and consumes it.
  /*!
    Construct with a std::span<const uint8_t>. Data is not owned and is treated as read-only.
    The buffer advances an internal read_index_ until the end of the span is reached.
  */
  class ReadBufferView : public ::EmbeddedProto::ReadBufferInterface
  {
  public:
    //! Construct from a span of bytes.
    explicit ReadBufferView(const std::span<const uint8_t>& data_span) noexcept :
      data_span_(data_span),
      read_index_(0)
    {
    }

    ~ReadBufferView() override = default;

    //! Number of unread bytes remaining.
    uint32_t get_size() const override
    {
      return static_cast<uint32_t>(data_span_.size() - read_index_);
    }

    //! The total size of the underlying span.
    uint32_t get_max_size() const override
    {
      return static_cast<uint32_t>(data_span_.size());
    }

    //! Peek the next byte without advancing.
    bool peek(uint8_t& byte) const override
    {
      const bool ok = read_index_ < data_span_.size();
      if (ok)
      {
        byte = data_span_[read_index_];
      }
      return ok;
    }

    //! Advance by one byte.
    bool advance() override
    {
      const bool ok = read_index_ < data_span_.size();
      if (ok)
      {
        ++read_index_;
      }
      return ok;
    }

    //! Advance by N bytes.
    bool advance(const uint32_t N) override
    {
      const uint64_t new_index = static_cast<uint64_t>(read_index_) + N;
      const bool ok = new_index <= data_span_.size();
      if (ok)
      {
        read_index_ = static_cast<size_t>(new_index);
      }
      return ok;
    }

    //! Pop the next byte and advance.
    bool pop(uint8_t& byte) override
    {
      const bool ok = read_index_ < data_span_.size();
      if (ok)
      {
        byte = data_span_[read_index_++];
      }
      return ok;
    }

    //! Return pointer to the underlying data (const).
    const uint8_t* get_data() const noexcept
    {
      return data_span_.data();
    }

    //! Reset the read index to re-consume the span from the start.
    void clear() noexcept
    {
      read_index_ = 0;
    }

  private:
    std::span<const uint8_t> data_span_;
    size_t read_index_;
  };

} // namespace EmbeddedProto

#endif // _READ_BUFFER_DYNAMIC_H_
