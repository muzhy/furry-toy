#ifndef __FURRY_TOY_BASE_H__
#define __FURRY_TOY_BASE_H__

namespace furry_toy{

class UnAssignable
{
public:
    UnAssignable() {}
    virtual ~UnAssignable(){}
private:
    // 禁止拷贝
    UnAssignable(const UnAssignable& ) = delete;
    UnAssignable& operator= (const UnAssignable&) = delete;
};

}

#endif