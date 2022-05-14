#include <vector>
#include <memory>
#include <initializer_list>
#include <string>
#include <sstream>

namespace furry_toy::ds
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
         * 允许通过其他切片来创建新的切片
         * 允许使用initializer_list创建新切片
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
        explicit slice(std::initializer_list<T> list)
            : m_data(std::make_shared<std::vector<T>>(list)),
              m_beginIndex(0), m_endIndex(list.size())
        {
        }
        /****************************************************************
         * 使用数组初始化切片
         * beginIndex   默认值为0， 表示从数组的第一个元素开始
         * endIndex     默认值为0， 表示到数组的最后一个元素结束
         * **************************************************************/
        explicit slice(T data[], size_t beginIndex = 0, size_t endIndex = 0);

        ~slice()
        {
        }

        /********************************************************
         * 切片截取，由于[]运算符无法接受两个参数，故使用()替代
         * end 为0时表示到最后一个元素
         * ******************************************************/
        slice<T> operator()(size_t begin, size_t end);
        T &operator[](size_t index)
        {
            return (*m_data)[index];
        }
        const T &operator[](size_t index) const
        {
            return (*m_data)[index];
        }

        // 获取数组的长度
        size_t len()
        {
            return m_endIndex - m_endIndex;
        }
        // 获取数组的容量
        size_t cap()
        {
            return m_data->capacity() - m_beginIndex;
        }
        // 是否为空
        bool isEmpty()
        {
            return m_data == nullptr || m_beginIndex == m_endIndex;
        }

        slice<T>& append(std::initializer_list<T> list)
        {
            m_data->reserve(m_data->size() + list.size());
            for(auto it = list.begin(); it != list.end(); it++)
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
                ss << (*m_data)[i] << ",";
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

    template <typename T>
    slice<T> makeSlice(size_t len, size_t cap = 0);

}
