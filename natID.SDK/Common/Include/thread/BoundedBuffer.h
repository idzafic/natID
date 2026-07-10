// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file BoundedBuffer.h
 * @brief Thread-safe bounded circular buffer with blocking and non-blocking push/pop operations.
 */
#pragma once
//#include <deque>
#include <mutex>
#include <condition_variable>
#include <cnt/CircularBuffer.h>

namespace thread
{
/// @brief Thread-safe bounded buffer backed by a circular buffer; blocks producers when full and consumers when empty.
template <typename T, size_t SIZE=5>
class BoundedBuffer
{
public:
    /// @brief Alias for the underlying circular buffer container type.
    using BaseContainerType = cnt::CircularBuffer<T, SIZE>;
private:
    // for locking
    std::mutex _mutex; ///< Mutex protecting access to the buffer.
    std::condition_variable _cvNotEmpty; ///< Condition variable signalled when the buffer becomes non-empty.
    std::condition_variable _cvNotFull; ///< Condition variable signalled when the buffer becomes non-full.

    // circular buffer
    BaseContainerType _buffer; ///< The underlying circular buffer storage.
    //bool m_finishConsumingWhenEmpty = false;

protected:
    /// @brief Checks whether the buffer has room for at least one more element.
    /// @return true if the buffer is not full, false otherwise.
    inline bool isNotFull() const
    {
        return _buffer.size() < _buffer.capacity();
    }
public:
    /// @brief Default constructor; initialises an empty bounded buffer.
    BoundedBuffer()
        : _buffer()
    {}

    /// @brief Removes all elements from the buffer under lock.
    void clear()
    {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _buffer.reset();
        }
    }

    /// @brief Blocks until space is available, then inserts a value into the buffer.
    /// @param v Value to insert.
    void push(T v)
    {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            //cv_not_full_.wait(lock, [this]() { return this->buffer_.size() < buffer_.capacity(); });
            //cv_not_full_.wait(lock, !buffer_.is_full());
            _cvNotFull.wait(lock, [this]() { return _buffer.isNotFull(); });
            _buffer.put(v);
        }
        _cvNotEmpty.notify_one();
    }

    /// @brief Attempts to insert a value without blocking.
    /// @param v Value to insert.
    /// @return true if the value was inserted, false if the buffer was full.
    bool tryPush(T v)
    {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if (_buffer.full())
            {
                return false;
            }
            _buffer.put(v);
        }
        _cvNotEmpty.notify_one();
        return true;
    }

    /// @brief Attempts to insert a value, waiting at most for the specified duration.
    /// @tparam Duration std::chrono duration type.
    /// @param v Value to insert.
    /// @param d Maximum time to wait.
    /// @param item Unused output parameter (reserved for future use).
    /// @return true if the value was inserted before the timeout, false otherwise.
    template <typename Duration>
    bool tryPushFor(T v, Duration&& d, T& item)
    {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            //if (!cv_not_full_.wait_for(lock, d, [this]() { return this->buffer_.size() < buffer_.capacity(); }))
            //if (!cv_not_full_.wait_for(lock, d, !buffer_.is_full()))
            if (!_cvNotFull.wait_for(lock, d, [this]() { return _buffer.isNotFull(); }))
            {
                return false;
            }
            _buffer.put(v);
        }
        _cvNotEmpty.notify_one();
        return true;
    }

    /// @brief Attempts to insert a value, waiting until the specified time point.
    /// @tparam Timepoint std::chrono time_point type.
    /// @param v Value to insert.
    /// @param tp Absolute time point deadline.
    /// @param item Unused output parameter (reserved for future use).
    /// @return true if the value was inserted before the deadline, false otherwise.
    template <typename Timepoint>
    bool tryPushUntil(T v, Timepoint&& tp, T& item)
    {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            //if (!cv_not_full_.wait_until(lock, tp, [this]() { return this->buffer_.size() < buffer_.capacity(); }))
            //if (!cv_not_full_.wait_until(lock, tp, !buffer_.is_full()))
            if (!_cvNotFull.wait_until(lock, tp, [this]() { return _buffer.isNotFull(); }))
            {
                return false;
            }
            _buffer.put(v);
        }
        _cvNotEmpty.notify_one();
        return true;
    }

    /// @brief Blocks until an element is available, then removes and returns it.
    /// @return The removed element.
    T pop()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        //cv_not_empty_.wait(lock, [this]() { return this->buffer_.size() > 0; });
        //cv_not_empty_.wait(lock, !buffer_.is_empty());
        _cvNotEmpty.wait(lock, [this]() { return _buffer.isNotEmpty(); });
        T v = _buffer.get();
        //buffer_.pop_back();
        lock.unlock();
        _cvNotFull.notify_one();
        return v;
    }

    /// @brief Blocks until an element is available, then removes it into the output parameter.
    /// @param val Output parameter that receives the removed element.
    void pop(T& val)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        //cv_not_empty_.wait(lock, [this]() { return this->buffer_.size() > 0; });
        //cv_not_empty_.wait(lock, !buffer_.is_empty());
        _cvNotEmpty.wait(lock, [this]() { return _buffer.isNotEmpty(); });
        val = _buffer.get();
        //buffer_.pop_back();
        lock.unlock();
        _cvNotFull.notify_one();
    }

    /// @brief Attempts to remove an element without blocking.
    /// @param item Output parameter that receives the element if successful.
    /// @return true if an element was removed, false if the buffer was empty.
    bool tryPop(T& item)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (_buffer.isEmpty())
        {
            lock.unlock();
            return false;
        }
        item = _buffer.get();
        lock.unlock();
        _cvNotFull.notify_one();
        return true;
    }

    /// @brief Attempts to remove an element, waiting at most for the specified duration.
    /// @tparam Duration std::chrono duration type.
    /// @param d Maximum time to wait.
    /// @param item Output parameter that receives the removed element if successful.
    /// @return true if an element was removed before the timeout, false otherwise.
    template <typename Duration>
    bool tryPopFor(Duration&& d, T& item)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        //if (!cv_not_empty_.wait_for(lock, d, [this]() { return this->buffer_.size() > 0; }))
        //if (!cv_not_empty_.wait_for(lock, d, !buffer_.is_empty()))
        if (!_cvNotEmpty.wait_for(lock, d, [this]() { return _buffer.isNotEmpty(); }))
        {
            return false;
        }
        item = _buffer.get();
        //buffer_.pop_back();
        lock.unlock();
        _cvNotFull.notify_one();
        return true;
    }

    /// @brief Attempts to remove an element, waiting until the specified time point.
    /// @tparam Timepoint std::chrono time_point type.
    /// @param tp Absolute time point deadline.
    /// @param item Output parameter that receives the removed element if successful.
    /// @return true if an element was removed before the deadline, false otherwise.
    template <typename Timepoint>
    bool tryPopUntil(Timepoint&& tp, T& item)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        //if (!cv_not_empty_.wait_until(lock, tp, [this]() { return this->buffer_.size() > 0; }))
        //if (!cv_not_empty_.wait_until(lock, tp, !buffer_.is_empty()))
        if (!_cvNotEmpty.wait_until(lock, tp, [this]() { return _buffer.isNotEmpty();}))
        {
            return false;
        }
        item = _buffer.back();
        //buffer_.get();
        lock.unlock();
        _cvNotFull.notify_one();
        return true;
    }

    //void finishConsumingOnEmpty()
    //{
    //	{
    //		std::scoped_lock lock(mutex_);
    //		m_finishConsumingWhenEmpty = true;
    //	}
    //	cv_not_empty_.notify_all();
    //}

    /// @brief Checks whether the buffer contains no elements.
    /// @return true if the buffer is empty, false otherwise.
    bool empty() const
    {
        return _buffer.empty();
    }

    /// @brief Returns the number of elements currently in the buffer.
    /// @return Current element count.
    size_t size()
    {
        return _buffer.size();
    }

    /// @brief Returns the maximum number of elements the buffer can hold.
    /// @return Buffer capacity.
    size_t capacity()
    {
        return _buffer.capacity();
    }

    /// @brief Returns a copy of the underlying circular buffer under lock.
    /// @return A snapshot copy of the internal buffer.
    BaseContainerType get_queue()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _buffer;
    }
};

} //namespace thread
