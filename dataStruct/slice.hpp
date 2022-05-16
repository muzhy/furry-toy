#include <vector>
#include <memory>
#include <initializer_list>
#include <string>
#include <sstream>
#include <assert.h>

#ifndef __FURRY_TOY_SLICE_H__
#define __FURRY_TOY_SLICE_H__

namespace furry_toy
{
    /*********************************************************************
     * slice 模仿Go中的切片实现的切片
     * 创建： 可以创建空的切片，也可以使用makeSlice()函数创建切片并指定切片长度
     * ******************************************************************/
    template <typename T>
    class slice
    {
    public:
        /***************************************************************
         * 允许创建空的切片
         * ************************************************************/
        explicit slice()
            : m_data(nullptr), m_beginIndex(0), m_endIndex(0)
        {
        }

        explicit slice(const slice<T> &otherslice)
            : m_data(otherslice.m_data), m_beginIndex(otherslice.m_beginIndex),
              m_endIndex(otherslice.m_endIndex)
        {
        }

        explicit slice(slice<T> &otherSlice, size_t beginIndex, size_t endIndex)
            : m_data(otherSlice.m_data), 
            m_beginIndex(otherSlice.m_beginIndex + beginIndex),
            m_endIndex(otherSlice.m_endIndex + endIndex)
        {
            assert(m_data->size() >= m_endIndex);
            assert(m_data->size() > m_beginIndex);
            assert(m_beginIndex >= 0);
        }

        explicit slice(std::initializer_list<T> list)
            : m_data(std::make_shared<std::vector<T>>(list)),
              m_beginIndex(0), m_endIndex(list.size())
        {
        }

        /**********************************************************
         * 根据长度及初值初始化切片
         * 会向切片中插入len个值为value的数据
         * ********************************************************/
        explicit slice(size_t len, T value)
            : m_data(std::make_shared<std::vector<T>>(len, value)),
              m_beginIndex(0), m_endIndex(len)
        {
        }

        /****************************************************************
         * 使用数组初始化切片, 必须指定起始位置及长度
         * 调用者需要保证数据的可用
         * **************************************************************/
        explicit slice(T data[], size_t beginIndex, size_t len)
            : m_beginIndex(0), m_endIndex(len)
        {
            if (len == 0)
            {
                return;
            }
            m_data = std::make_shared<std::vector<T>>();
            m_data->reserve(len);
            for (size_t i = 0; i < len; i++)
            {
                m_data->push_back(data[i + beginIndex]);
            }
        }

        ~slice()
        {
        }

        /********************************************************
         * 切片截取，由于[]运算符无法接受两个参数，故使用()替代
         * end 为0时表示到最后一个元素
         * ******************************************************/
        slice<T> operator()(size_t begin, size_t end)
        {
            return slice(*this, begin, end);
        }
        
        /**********************************************************
         * 下标运算，获取切片中的数据
         * ********************************************************/
        T &operator[](size_t index)
        {
            assert(index + m_beginIndex < m_endIndex);
            return (*m_data)[index + m_beginIndex];
        }
        const T &operator[](size_t index) const
        {
            assert(index + m_beginIndex < m_endIndex);
            return (*m_data)[index + m_beginIndex];
        }

        // 获取数组的长度
        size_t len()
        {
            return m_endIndex - m_beginIndex;
        }
        size_t size()
        {
            return len();
        }

        // 获取数组的容量
        size_t cap()
        {
            if(m_data == nullptr)
            {
                return 0;
            }
            return m_data->capacity() - m_beginIndex;
        }
        // 是否为空
        bool isEmpty()
        {
            return m_data == nullptr || m_beginIndex == m_endIndex;
        }

        slice<T> &append(std::initializer_list<T> list)
        {
            m_data->reserve(m_data->size() + list.size());
            for (auto it = list.begin(); it != list.end(); it++)
            {
                m_data->push_back(*it);
            }
            m_endIndex += list.size();
            return *this;
        }

        std::string toString()
        {
            if (isEmpty())
            {
                return "[]";
            }
            std::stringstream ss;
            ss << "[";
            for (size_t i = m_beginIndex; i < m_endIndex - 1; i++)
            {
                ss << (*m_data)[i] << ", ";
            }
            ss << (*m_data)[m_endIndex - 1];
            ss << "]";
            return ss.str();
        }

    private:
        std::shared_ptr<std::vector<T>> m_data;
        size_t m_beginIndex;
        size_t m_endIndex;
    };

}

#endif