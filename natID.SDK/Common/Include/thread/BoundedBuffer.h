// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

#pragma once
//#include <deque>
#include <mutex>
#include <condition_variable>
#include <cnt/CircularBuffer.h>

namespace thread
{
template <typename T, size_t SIZE=5>
class BoundedBuffer
{
public:
    using BaseContainerType = cnt::CircularBuffer<T, SIZE>;
private:
    // for locking
    std::mutex _mutex;
    std::condition_variable _cvNotEmpty;
    std::condition_variable _cvNotFull;

    // circular buffer
    BaseContainerType _buffer;
    //bool m_finishConsumingWhenEmpty = false;

protected:
    inline bool isNotFull() const
    {
        return _buffer.size() < _buffer.capacity();
    }
public:
    BoundedBuffer()
        : _buffer()
    {}

    void clear()
    {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _buffer.reset();
        }
    }

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

    bool empty() const
    {
        return _buffer.empty();
    }

    size_t size()
    {
        return _buffer.size();
    }

    size_t capacity()
    {
        return _buffer.capacity();
    }

    BaseContainerType get_queue()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _buffer;
    }
};

} //namespace thread

