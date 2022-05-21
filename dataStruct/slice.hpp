#include <vector>
#include <memory>
#include <initializer_list>
#include <string>
#include <sstream>
#include <assert.h>
#include <iostream>

#ifndef __FURRY_TOY_SLICE_H__
#define __FURRY_TOY_SLICE_H__

namespace furry_toy
{
    /*********************************************************************
     * slice 模仿Go中的切片实现的切片
     * 创建： 可以创建空的切片，也可以使用makeSlice()函数创建切片并指定切片长度
     * slice目标在于尽可能提供与Go中的slice相同的功能，由于C++本身语法的限制
     * 部分功能如[:]无法直接通过重载[]实现，改为重载()实现，
     * 但总体上应该能与Go中的slice表现出一致的行为
     * ******************************************************************/
    template <typename T>
    class slice
    {
    public:
        /***************************************************************
         * 允许创建空的切片
         * ************************************************************/
        explicit slice()
            : m_data(nullptr), m_beginIndex(0), m_len(0)
        {
        }

        explicit slice(const slice<T> &otherslice)
            : m_data(otherslice.m_data), m_beginIndex(otherslice.m_beginIndex),
              m_len(otherslice.m_len)
        {
        }

        explicit slice(slice<T> &otherSlice, size_t beginIndex, size_t endIndex)
            : m_data(otherSlice.m_data), 
            m_beginIndex(otherSlice.m_beginIndex + beginIndex),
            m_len(endIndex - beginIndex)
        {
            assert(m_beginIndex >= 0);
            assert(m_data->size() > m_beginIndex);
            assert(m_data->size() >= (beginIndex + m_len));
        }

        slice(std::initializer_list<T> list)
            : m_data(std::make_shared<std::vector<T>>(list)),
              m_beginIndex(0), m_len(list.size())
        {
        }

        /**********************************************************
         * 根据长度及初值初始化切片
         * 会向切片中插入len个值为value的数据
         * ********************************************************/
        explicit slice(size_t len, T value)
            : m_data(std::make_shared<std::vector<T>>(len, value)),
              m_beginIndex(0), m_len(len)
        {
        }

        /****************************************************************
         * 使用数组初始化切片, 必须指定起始位置及长度
         * 调用者需要保证数据的可用
         * **************************************************************/
        explicit slice(T data[], size_t beginIndex, size_t len)
            : m_beginIndex(0), m_len(len)
        {
            if (len == 0)
            {
                return;
            }
            m_data = std::make_shared<std::vector<T>>();
            m_data->reserve(len);
            m_data->insert(m_data->end(), std::begin(data), std::end(data));
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
            assert(index < m_len);
            return (*m_data)[index + m_beginIndex];
        }
        const T &operator[](size_t index) const
        {
            assert(index < m_len);
            return (*m_data)[index + m_beginIndex];
        }

        // 获取数组的长度
        size_t len()
        {
            return m_len;
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
        bool isEmpty() const
        {
            return m_data == nullptr || m_len == 0;
        }

        slice<T> &append(std::initializer_list<T> list)
        {
            if(m_beginIndex == 0 && len == m_data.size())
            {
                // 位于vector末尾，不需要拷贝数据，直接再末尾添加数据
                m_data->reserve(m_data->size() + list.size());
                m_data->insert(m_data->end(), list.begin(), list.end());
                m_len = m_data->size();
            }
            else
            {
                auto tempData = std::make_shared<T>(std::vector<T>());
                tempData->reserve(m_data->size() + list.size());
                tempData->insert(tempData->end(), m_data->begin(), m_data->end());
                tempData->insert(tempData->end(), list.begin(), list.end());
            }
            
            return *this;
        }

        std::string toString() const
        {
            if (isEmpty())
            {
                return "[]";
            }
            std::stringstream ss;
            ss << "[";
            for (size_t i = 0; i < m_len - 1; i++)
            {
                ss << (*m_data)[i + m_beginIndex] << ", ";
            }
            ss << (*m_data)[m_beginIndex + m_len - 1];
            ss << "]";
            return ss.str();
        }

    private:
        std::shared_ptr<std::vector<T>> m_data;
        size_t m_beginIndex;
        size_t m_len;
    };

    template<typename T>
    std::ostream& operator<<(std::ostream& out, const slice<T>& slice)
    {
        out << slice.toString();
        return out;
    }

    
}

#endif